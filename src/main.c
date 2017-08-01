#include <stdio.h>
#include <string.h>
#include "scm.h"
#include "port.h"
#include "print.h"
#include "read.h"
#include "system.h"
#include "eval.h"

void repl()
{
    scm_object *exp, *val;

    char *prompt = "> ";
    while (1) {
        printf(prompt);
        exp = scm_read(scm_stdin_port);
        if (exp != NULL) {
            val = scm_eval(exp);
            if (val != NULL && !SCM_VOIDP(val)) {
                scm_write(scm_stdout_port, val);
                putchar('\n');
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        scm_init();
        // do repl
        repl();
    } else if (argc == 3) {
        char *opt = argv[1];
        char *filepath = argv[2];
        if (strcmp(opt, "-file") == 0) {
            scm_init();
            // load file
            scm_load_file(filepath);
        }
    }

    return 0;
}