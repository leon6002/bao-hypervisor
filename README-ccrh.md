# CC-RH 移植（暂停中）

把 Renesas CC-RH 工具链接到**上游 rh850 移植**之上的尝试。目的是让商业线和开源线共用同一份最新代码，
而不是像现在这样分叉在相差 15 个月的两个基底上。

**当前状态：全树编译通过，链接未通过。** 卡在一处需要仔细设计的改动上，见下文。

**这条线不是主线。** 主线是 `upstream/rh850-u2a-gcc`——上游 main + v850-elf-gcc，
已在 RH850/U2A 实机验证（串口 + CAN 正常，复位后稳定，镜像 37 184 B）。
本分支基于它，且**全程保持它可构建**，产物不受影响。

最后更新：2026-08-04

---

## 怎么编

```bash
# CC-RH（本分支的目标，目前止于链接）
export PATH=/usr/local/Renesas/CC-RH/V2.08.00/bin:$PATH
make PLATFORM=rh850-u2a16 CONFIG=autosar-vm0+vm1 CROSS_COMPILE=ccrh

# GCC（本分支同样可用，产物与主线一致）
export PATH=<v850-elf-gcc>/bin:$PATH
make PLATFORM=rh850-u2a16 CONFIG=autosar-vm0+vm1 CROSS_COMPILE=v850-elf-
```

`CROSS_COMPILE` 里含 `ccrh` 就切到 CC-RH 分支，和现有的 gcc / clang 判别并列。

## 已完成

| 项 | 做法 |
|---|---|
| 构建系统 | Makefile 第三条工具链分支：ccrh / asrh / rlink，无 objcopy/objdump/readelf/strip，`.asm` 汇编源，rlink subcommand 文件代替链接脚本 |
| 依赖生成 | CC-RH 无 `-MM`，改用宿主 gcc |
| 宿主生成器 | 从 `HOST_CPPFLAGS` 滤掉 `-DCC_IS_RHCC`，否则宿主 gcc 会看到 `#pragma inline_asm` 函数体 |
| 81 个系统寄存器访问器 | `_Pragma` + `inline_asm`，**保留宏生成形式** |
| `fences.h`、`cpu.c`、`snooze` | 单指令 `#pragma inline_asm` |
| 自旋锁 | 独立的 `spinlock.asm`，单一定义 |
| PTE 权限常量 | 具名 `MPAT_*` 位宏组合 `.raw` |
| `struct shmem`、`vm_mem_region` | 展平匿名 union / struct |
| `boot.S` → `boot.asm` | asrh 语法，509 行汇编的一半 |
| `exceptions.S` → `exceptions.asm` | 同上 |
| `asm_defs.h` 生成 | `ASMDEF_*` 常量 + 从汇编输出提取 |
| `CPU_MSG_HANDLER` | `_Pragma("section ...")` |
| `_Alignof`、`FALLTHROUGH` | C99 等价实现 / 空宏 |

## 唯一的阻塞点：链接器符号

链接报 8 个未定义符号，全部来自 GNU 链接脚本：

```
_image_start   _image_end   _image_load_end   _image_noload_start
_data_vma_start   _data_lma_start
_ipi_cpumsg_handlers   _ipi_cpumsg_handlers_id_start/_end
```

rlink 没有链接脚本，也就没有这些符号。**但地址是拿得到的**——实测确认 rlink 会解析
`__s<段名>` / `__e<段名>`，例如 `__s.data`、`__e.bss.R`。

问题在语义不匹配：C 代码写的是 `&_image_start`，**符号地址本身就是值**；而汇编能提供的是
"一个存着该地址的变量"。直接接上会得到错误地址。

这需要改 `core/mem.c`、`core/mpu/mem.c`、`core/cpu.c` 取这些符号的方式（约 8 处）。
改动不大，但它决定 **ROM→RAM 拷贝范围和 .bss 清零范围**——写错了板子会带着损坏的数据启动，
症状很难查。所以停在这里，没有赶工烧板。

三条路：

1. **改 C 侧取值方式**——那 8 处用 `#ifdef CC_IS_RHCC` 读变量而非取地址。最直接，对上游 diff 变大
2. **让 rlink 产出真正的绝对符号**——更干净，但要先确认 rlink 有没有这个手段
3. **先按 1 做通、烧板验证，再优化成 2**

## 另一处未完成：结构体对齐

`ATTRIB_ALIGN()` 在 CC-RH 下**展开为空**，`bao.h` 里有 TODO 标注。

CC-RH 的 `__attribute__((aligned(n)))` 只支持到 **4 字节**，结构体级完全不支持，
所以 `struct cpu` / `struct cpuif` 的 PAGE_SIZE(64) 对齐在 C 层面无法表达。

这对 `cpu()` 无影响（它读 FEWR 拿指针），但 **MPU 区域必须按粒度对齐**，
所以正式使用前必须从链接器层面补回来（专用 section + rlink 段对齐）。

## 这次摸清楚的 CC-RH 行为

都是实测结论，省得以后重推：

| 行为 | 结论 |
|---|---|
| `_Pragma` + `inline_asm` | **可用**，宏能生成 pragma。前提是函数体只有一条指令（宏展开成一个逻辑行） |
| `inline_asm` 里的标签 | 每次内联展开都重新发射，多处调用即重名报错。有循环的必须做成独立汇编函数 |
| `__attribute__((aligned(n)))` | 成员上最多 4 字节，结构体级不支持 |
| designated initializer | 匿名 struct/union 的成员一律拒绝（`E0523134`） |
| `-D` 的值 | **引号会被剥掉**，字符串宏要在 C 侧 stringify |
| `.asm` 文件 | **不经过预处理**，`#` 开头被当注释忽略 |
| asrh 的 `.macro` | 有，但宏内标签重复展开会冲突 |
| 取符号地址 | `mov sym, reg` / `mov #sym, reg` 都行。C 宏里别写 `#sym`，那是字符串化 |
| 段边界符号 | rlink 解析 `__s<段名>` / `__e<段名>` |
| GNU 扩展内联汇编 | 完全不支持，`DEFINE_OFFSET` 那套要另找出路 |
| 授权 | 当前是试用，剩余 60 天。`flash/installers/` 里有 license-manager |

## 相关

- `upstream/rh850-u2a-gcc` — 主线，实机验证过
- `osyx/rh850-u2a-ccrh208` — 旧私有基底 + CC-RH，也实机验证过，但落后上游 15 个月
- `flash/README.md` — 烧录步骤与工具
- `.claude/analysis/porting/` — 移植分析与实机 bring-up 记录（不在版本控制内）
