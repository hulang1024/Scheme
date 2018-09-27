#include <stdio.h>
#include <string.h>
#include "scm.h"
#include "port.h"
#include "print.h"
#include "read.h"
#include "system.h"
#include "eval.h"

char *scm_g_repl_prompt = "> ";

void repl()
{
    puts("Welcome to Scheme. github.com/hulang1024/scheme, (?) for help");

    scm_object *exp, *val;

    while (1) {
        printf("%s", scm_g_repl_prompt);
        exp = scm_read(scm_stdin_port);
        if (exp) {
            val = scm_eval(exp);
            if (val && !SCM_VOIDP(val)) {
                scm_write(scm_stdout_port, val);
                putchar('\n');
            }
        }
    }
}

void eval_src_string(char *src) {
    scm_object *val = scm_eval_src_string(src);
    if (val)
        scm_write(scm_stdout_port, val);
}

int main(int argc, char *argv[])
{
    scm_init();
    if (argc == 1) {
        // do repl
        repl();
    } else if (argc > 1) {
        if (strcmp(argv[1], "--e") == 0) {
            if (argc > 2) {
                eval_src_string(argv[2]);
            }
        } else {
            // load files
            int i;
            for (i = 1; i < argc; i++) {
                scm_load_file(argv[i]);
            }
        }
    }
}
