#include <stdio.h>
#include "scm.h"

int main(int argc, char *argv[])
{
    test();

    scm_object *stdin_port = scm_make_stdin_port();
    scm_object *stdout_port = scm_make_stdout_port();
    
    scm_env *global_env;
    scm_object *exp = NULL;
    scm_object *val;

    while(1) {
        exp = scm_read(stdin_port);
        if(exp != NULL) {
            val = scm_eval(exp, global_env);
            scm_println(stdout_port, val);
        }
    }
    return 0;
}