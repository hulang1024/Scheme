#include <stdio.h>
#include <string.h>
#include "scm.h"
#include "bool.h"
#include "port.h"
#include "print.h"
#include "read.h"
#include "env.h"
#include "eval.h"

void repl()
{
    scm_env *global_env = scm_basic_env();
    scm_object *exp, *val;

    while(1) {
        printf("> ");
        exp = scm_read(scm_stdin_port);
        if(exp != NULL) {
            val = scm_eval(exp, global_env);
            if(val != NULL && !SCM_VOIDP(val)) {
                scm_write(scm_stdout_port, val);
                putchar('\n');
            }
        }
    }
}

void exec_src_file(const char *filepath)
{
    scm_env *global_env = scm_basic_env();
    FILE *file = fopen(filepath, "r");
    scm_object *port = scm_make_file_input_port(file);
    scm_object *exp, *val;
    char ch;

    while(!scm_eofp( ch = scm_getc(port) )) {
        scm_ungetc(ch, port);
        exp = scm_read(port);
        if (exp != NULL) {
            val = scm_eval(exp, global_env);
            if (val != NULL && !SCM_VOIDP(val))
                scm_display(scm_stdout_port, val);
        }
    }

    scm_close_input_port(port);
}

int main(int argc, char *argv[])
{
    if(argc == 1) {
        repl();
    } else if(argc == 3) {
        char *opt = argv[1];
        char *filepath = argv[2];
        if(strcmp(opt, "-file") == 0)
            exec_src_file(filepath);
    }

    return 0;
}