#include <defs.h>
#include <stdio.h>
#include <string.h>
#include <console.h>
#include <kdebug.h>
#include <picirq.h>
#include <trap.h>
#include <clock.h>
#include <intr.h>
#include <pmm.h>
#include <kmonitor.h>
#include <memlayout.h>
int kern_init(void) __attribute__((noreturn));
void grade_backtrace(void);
static void lab1_switch_test(void);

struct segdesc {
    unsigned sd_lim_15_0 : 16;      // low bits of segment limit
    unsigned sd_base_15_0 : 16;     // low bits of segment base address
    unsigned sd_base_23_16 : 8;     // middle bits of segment base address
    unsigned sd_type : 4;           // segment type (see STS_ constants)
    unsigned sd_s : 1;              // 0 = system, 1 = application
    unsigned sd_dpl : 2;            // descriptor Privilege Level
    unsigned sd_p : 1;              // present
    unsigned sd_lim_19_16 : 4;      // high bits of segment limit
    unsigned sd_avl : 1;            // unused (available for software use)
    unsigned sd_rsv1 : 1;           // reserved
    unsigned sd_db : 1;             // 0 = 16-bit segment, 1 = 32-bit segment
    unsigned sd_g : 1;              // granularity: limit scaled by 4K when set
    unsigned sd_base_31_24 : 8;     // high bits of segment base address
};
//#define STA_X           0x8         // Executable segment
//#define STA_E           0x4         // Expand down (non-executable segments)
//#define STA_C           0x4         // Conforming code segment (executable only)
//#define STA_W           0x2         // Writeable (non-executable segments)
//#define USER_CS     ((GD_UTEXT) | DPL_USER)
//#define USER_DS     ((GD_UDATA) | DPL_USER)
//#define STA_R           0x2         // Readable (executable segments)
//#define STA_A           0x1         // Accessed
//
//#define SEG(type, base, lim, dpl)                           \
//    (struct segdesc) {                                      \
//        ((lim) >> 12) & 0xffff, (base) & 0xffff,            \
//        ((base) >> 16) & 0xff, type, 1, dpl, 1,             \
//        (unsigned)(lim) >> 28, 0, 0, 1, 1,                  \
//        (unsigned) (base) >> 24                             \
//    }


void print_cpl() {
	uint16_t reg1;

	asm volatile ("mov %%cs, %0;"
	            : "=m"(reg1));
	cprintf("*******CPL = %d\n", reg1 & 3);
}

void rpl_gt_cpl() {
	asm volatile ("mov %0, %%ax;mov %%ax, %%ds;"
			: : "i"(USER_DS));
}

void rpl_eq_cpl() {
	asm volatile ("mov %0, %%ax;mov %%ax, %%ds;"
			: : "i"(KERNEL_DS));
}

void rpl_eq_dpl() {
	asm volatile ("mov %0, %%ax;mov %%ax, %%ds;"
			: : "i"(KERNEL_DS));
}

// RPL > DPL
void rpl_gt_dpl() {
	asm volatile ("mov %0, %%ax;mov %%ax, %%ds;"
			: : "i"(KERNEL_DS_RPL_3));
}

int
kern_init(void) {
    extern char edata[], end[];
    memset(edata, 0, end - edata);

    cons_init();                // init the console

    const char *message = "(THU.CST) os is loading ...";
    cprintf("%s\n\n", message);

    print_cpl();

    print_kerninfo();

    grade_backtrace();

    pmm_init();                 // init physical memory management


    pic_init();                 // init interrupt controller
    idt_init();                 // init interrupt descriptor table
    cprintf("check rpl dpl cpl\n");

    rpl_gt_dpl();

    clock_init();               // init clock interrupt
    intr_enable();              // enable irq interrupt

    //LAB1: CAHLLENGE 1 If you try to do it, uncomment lab1_switch_test()
    // user/kernel mode switch test
    //lab1_switch_test();

    /* do nothing */
    while (1);
}

void __attribute__((noinline))
grade_backtrace2(int arg0, int arg1, int arg2, int arg3) {
    mon_backtrace(0, NULL, NULL);
}

void __attribute__((noinline))
grade_backtrace1(int arg0, int arg1) {
    grade_backtrace2(arg0, (int)&arg0, arg1, (int)&arg1);
}

void __attribute__((noinline))
grade_backtrace0(int arg0, int arg1, int arg2) {
    grade_backtrace1(arg0, arg2);
}

void
grade_backtrace(void) {
    grade_backtrace0(0, (int)kern_init, 0xffff0000);
}

static void
lab1_print_cur_status(void) {
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;
    asm volatile (
            "mov %%cs, %0;"
            "mov %%ds, %1;"
            "mov %%es, %2;"
            "mov %%ss, %3;"
            : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));
    cprintf("%d: @ring %d\n", round, reg1 & 3);
    cprintf("%d:  cs = %x\n", round, reg1);
    cprintf("%d:  ds = %x\n", round, reg2);
    cprintf("%d:  es = %x\n", round, reg3);
    cprintf("%d:  ss = %x\n", round, reg4);
    round ++;
}

static void
lab1_switch_to_user(void) {
    //LAB1 CHALLENGE 1 : TODO
}

static void
lab1_switch_to_kernel(void) {
    //LAB1 CHALLENGE 1 :  TODO
}

static void
lab1_switch_test(void) {
    lab1_print_cur_status();
    cprintf("+++ switch to  user  mode +++\n");
    lab1_switch_to_user();
    lab1_print_cur_status();
    cprintf("+++ switch to kernel mode +++\n");
    lab1_switch_to_kernel();
    lab1_print_cur_status();
}

