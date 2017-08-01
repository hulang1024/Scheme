#include "system.h"
#include "bool.h"
#include "port.h"
#include "read.h"
#include "print.h"
#include "env.h"
#include "eval.h"
#include "error.h"

static scm_object* load_prim(int, scm_object *[]);

void scm_init_system(scm_env *env)
{
    scm_add_prim(env, "load", load_prim, 1, 1);
}

int scm_load_file(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    scm_object *port = scm_make_file_input_port(file);
    scm_object *exp, *val;
    char ch;

    while (!scm_eofp( ch = scm_getc(port) )) {
        scm_ungetc(ch, port);
        exp = scm_read(port);
        if (exp != NULL) {
            val = scm_eval(exp);
            if (val != NULL && !SCM_VOIDP(val))
                scm_display(scm_stdout_port, val);
        }
    }

    scm_close_input_port(port);
}

static scm_object* load_prim(int argc, scm_object *argv[])
{
    char* filename = SCM_CHAR_STR_VAL(argv[0]);
    int retcode = scm_load_file(filename);
    if (retcode != 0) {
        scm_print_error("open-input-file: cannot open input file\n  path: ");
        scm_print_error(filename);
    }
    return scm_void;
}