#include <stdio.h>
#include "error.h"
#include "print.h"
#include "eval.h"
#include <setjmp.h>

void scm_print_error(const char *info)
{
    printf("%s", info);
}

void scm_throw_error()
{
    longjmp(eval_error_jmp, 1);
}

scm_object* scm_wrong_contract(const char *proc_name, const char *expected, int index, int argc, scm_object *argv[])
{
    scm_print_error(proc_name);
    scm_print_error(": contract violation\n");
    scm_print_error("  expected: ");
    scm_print_error(expected);
    scm_print_error("\n");
    scm_print_error("  given: ");
    scm_write(scm_stdout_port, argv[index]);
    scm_print_error("\n");
    if(argc > 1) {
        scm_print_error("  argument position: ");
        char buf[10] = {0};
        sprintf(buf, "%d\n", index + 1);
        scm_print_error(buf);

        scm_print_error("  other arguments...:\n");
        int argi;
        for(argi = 0; argi < argc; argi++) {
            if(argi == index)
                continue;
            scm_print_error("   ");
            scm_write(scm_stdout_port, argv[argi]);
            scm_print_error("\n");
        }
    }

    scm_throw_error();

    return NULL;
}