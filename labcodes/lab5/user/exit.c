#include <stdio.h>
#include <ulib.h>

int magic = -0x10384;


extern void rpl_lt_dpl();
// RPL < DPL, must run in
void rpl_lt_dpl() {
	asm volatile ("mov %0, %%ax;mov %%ax, %%ds;"
			: : "i"((2 << 3) | 3));
}
int
main(void) {
    int pid, code;

    cprintf("checking rpl_lt_dpl\n");

    cprintf("I am the parent. Forking the child...\n");
    if ((pid = fork()) == 0) {
        cprintf("I am the child.\n");
        yield();
        yield();
        yield();
        yield();
        yield();
        yield();
        yield();
        exit(magic);
    }
    else {
        cprintf("I am parent, fork a child pid %d\n",pid);
    }

    assert(pid > 0);
    cprintf("I am the parent, waiting now..\n");

    assert(waitpid(pid, &code) == 0 && code == magic);
    assert(waitpid(pid, &code) != 0 && wait() != 0);
    cprintf("waitpid %d ok.\n", pid);

    cprintf("exit pass.\n");
    return 0;
}

