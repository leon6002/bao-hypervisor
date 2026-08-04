# RH850/U2A 烧录包

自带烧录所需的全部东西：工具、guest 镜像、现成的 bao 镜像。拿到这个目录就能把双 VM demo 烧上板子，
不需要先编译，也不需要另外装 rfp-cli。

## 仓库里没有的东西

为了不让仓库背上上百 MB 的二进制，`tools/`、`installers/`、`guests/`、`images/`
**都不纳入版本控制**，只有脚本和这份说明在 git 里。克隆下来后需要自己把它们放回原位：

| 目录 | 内容 | 从哪来 |
|---|---|---|
| `tools/rfp-cli/` | Renesas Flash Programmer CLI V1.16（76 MB） | `bao-benchmarks-portable/Tools/linux-x64/` 整个拷过来 |
| `tools/99-renesas-e2.rules`<br>`tools/install-udev-rule.sh` | E2 的 udev 规则 | 同上仓库的 `config/`，或照本文末尾自己写 |
| `installers/` | CC-RH V2.08.00 + license-manager（93 MB） | 瑞萨官网下载，商业授权 |
| `guests/` | 两个 guest hex（908 KB） | GHS MULTI 编译产出，见 `autoshow/ghs_u2a16_mcal` |
| `images/` | 三个 bao 镜像（130 KB） | `make` 产出，或从备份取 |

镜像也可以自己编：CC-RH 版见下文，上游 gcc 版在 `upstream/rh850-u2a-gcc` 分支。

## 快速开始

```bash
# 首次：装 E2 的 udev 规则，然后把 E2 拔插一次
./tools/install-udev-rule.sh

# 烧现成镜像做 demo（不用编译）
./flash.sh demo

# 看串口
picocom -b 500000 /dev/ttyACM0
```

正常的话串口会持续输出，约 386 B/s，同时 CAN 上有持续帧。

## 命令

| 命令 | 作用 |
|---|---|
| `./flash.sh` | 烧 guest + **自己编译的** bao，然后释放复位 |
| `./flash.sh demo` | 烧 guest + `images/bao-ccrh208-autosar.bin`（现成的） |
| `./flash.sh gcc` | 烧 guest + `images/bao-upstream-gcc-autosar.bin`（上游 main + v850-gcc） |
| `./flash.sh ref` | 烧 guest + `images/bao-reference-twovm.bin`（历史参考版，用于对照） |
| `./flash.sh bao` | 只重烧 bao，日常迭代用这个 |
| `./flash.sh guests` | 只烧两个 guest |
| `./flash.sh reset` | 只释放复位 |

环境变量：`PLATFORM`、`CONFIG` 选择要烧哪个构建；`IMAGE` 直接指定 bin 文件；`RFP_DIR` 覆盖 rfp-cli 位置。

## 目录

```
flash.sh                    烧录脚本
guests/
  vm0-autosar-0x10000.hex   VM0，AUTOSAR，驱动 CAN（RSCFD0）        355 KB
  vm1-mcal-0x7F0000.hex     VM1，MCAL，驱动 RLIN35 串口              566 KB
images/
  bao-ccrh208-autosar.bin      CC-RH V2.08.00，osyx/rh850-u2a-ccrh208    46 KB
  bao-upstream-gcc-autosar.bin v850-elf-gcc，upstream/rh850-u2a-gcc      37 KB
  bao-reference-twovm.bin      历史参考镜像，源码已失传，仅供对照         47 KB
tools/
  rfp-cli/                  Renesas Flash Programmer CLI V1.16       76 MB
  99-renesas-e2.rules       E2 的 udev 规则
  install-udev-rule.sh      装上面那条规则（需要 sudo）
installers/
  cc-rh_v20800_*.zip        CC-RH V2.08.00 编译器安装包              20 MB
  license-manager_*.zip     配套的授权管理器                          73 MB
```

`installers/` 里两个 zip 是**编译**用的，烧录本身用不到。CC-RH 是商业授权软件，装完要用
license-manager 激活。装好后：

```bash
export PATH=/usr/local/Renesas/CC-RH/V2.08.00/bin:$PATH
make PLATFORM=rh850-u2a-vm1+vm2 CONFIG=autosar-vm0+vm1
./flash.sh
```

## Flash 布局

| 地址 | 内容 |
|---|---|
| `0x000000` | Bao hypervisor |
| `0x010000` | VM0 — AUTOSAR guest，CAN |
| `0x7F0000` | VM1 — MCAL guest，串口 |

guest 用 `VM_IMAGE_LOADED` 声明，不内嵌进 bao.bin，各自单独烧。

## 两个容易踩的坑

**串口是 500000 波特率**，不是 115200。写死在
`src/platform/drivers/renesas_rlin3/renesas_rlin3.c` 的 `uart_init()` 里。按 115200 抓收不到任何东西。

**串口不是 E2 的口**。console 走一个独立的 CH340（USB `1a86:55d3`）接在板子 RLIN35 引脚上，
节点恰好也是 `/dev/ttyACM0`，容易搞混。E2（USB `045b:82a1`）只负责烧录。

## 底层命令

`flash.sh` 就是三条 rfp-cli：

```bash
echo ffff | rfp-cli -device "RH850/U2x" -tool e2 -osc 40.0 -program -file guests/vm0-autosar-0x10000.hex
echo ffff | rfp-cli -device "RH850/U2x" -tool e2 -osc 40.0 -program -file guests/vm1-mcal-0x7F0000.hex
echo ffff | rfp-cli -device "RH850/U2x" -tool e2 -osc 40.0 -program -bin 0x0 <bao.bin> -run
```

`ffff` 是 ID code，从 stdin 喂进去；`-osc 40.0` 是主时钟 40 MHz。
**不要加 `-erase-chip`**——那会把已经烧好的 guest 一起抹掉。

## 更多

三个镜像都在同一块 RH850/U2A 上验证过：串口持续输出、CAN 持续发帧、复位后稳定。

| 镜像 | 工具链 | 分支 | 体积 | 串口 |
|---|---|---|---|---|
| `bao-upstream-gcc-autosar.bin` | v850-elf-gcc 14.2.0 | `upstream/rh850-u2a-gcc` | 37 KB | 391 B/s |
| `bao-ccrh208-autosar.bin` | CC-RH V2.08.00 | `osyx/rh850-u2a-ccrh208` | 46 KB | 386 B/s |
| `bao-reference-twovm.bin` | CC-RH（旧版） | 已失传 | 47 KB | 386 B/s |

排查记录和背景见 `.claude/analysis/porting/hardware-bringup.html`。
