1. ucore系统的构建过程

  1. ucore.img构建: Makefile中`$(UCOREIMG)` recipe在`bin/ucore.img`生成系统磁盘镜像. 它由`bin/bootblock`和`bin/kernel`经过填0对齐到10000个block构成, 默认block为512字节, 所以磁盘镜像大小为4.88MB.
  2. `bin/bootblock`构建:
  - `tools/function.mk`文件定义一些Makefile中的帮助函数,
    - `listf`是根据文件目录和文件名选中特定文件
    - `listf_cc`是只选中某目录下的.c和.S文件
    - `totarget`是生成参数1加上bin/前缀.
    - `toobj` 生成将参数1后缀名变为.o的函数
  - bootfiles
    利用这几个函数生成`bootfiles`变量, `bootfiles = $(call listf_cc,boot)`, 从而bootfile为boot目录下的所有.c,.S文件.
  - sign
    *******此文件运行于host, 是构建工具, 之后详细写
  - 构建过程
    我们倒过来分析, `make "V="`, 可以看到链接命令行

        ld -m elf_i386 -nostdlib -N -e start -Ttext 0x7C00 obj/boot/bootasm.o obj/boot/bootmain.o -o obj/bootblock.o

    不链接标准库, 入口点是bootasm.S中的`start`符号, 代码段从0x7C00开始, 即BIOS将磁盘第一个扇区默认加载的地址, 编译成i386架构的elf格式文件.
