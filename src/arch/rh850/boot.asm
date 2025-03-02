
.section ".data", data
.align 2

.public __load_addr
__load_addr:
    .db2 0x10

.public __data_addr
__data_addr:
    .db2 0x10

.section	".text", text
.align	2
__start:
    br __start
