#include <time.h>
#include <string.h>
#include "system.h"
#include "bool.h"
#include "number.h"
#include "str.h"
#include "symbol.h"
#include "port.h"
#include "read.h"
#include "print.h"
#include "env.h"
#include "eval.h"
#include "error.h"


static const char *help_info = "\n"
    " (?)                     print help\n"
    " (exit [code])           exit REPL\n"
    " (set 'prompt <string>)  set prompt\n";

static scm_object* load_prim(int, scm_object *[]);
static scm_object* time_prim(int, scm_object *[]);
static scm_object* rand_prim(int, scm_object *[]);
static scm_object* help_prim(int, scm_object *[]);
static scm_object* exit_prim(int, scm_object *[]);
static scm_object* set_prim(int, scm_object *[]);

enum {
    ENCODING_UNKNOWN,
    ENCODING_UTF_8,
    ENCODING_UTF_16BE,
    ENCODING_UTF_16LE,
    ENCODING_UTF_32BE,
    ENCODING_UTF_32LE,
    ENCODING_UTF_7,
    ENCODING_GB_18030
};

static int get_skip_encoding_marks(scm_object *port);

void scm_init_system(scm_env *env)
{
    srand((unsigned)time(NULL));

    scm_add_prim(env, "load", load_prim, 1, 1);

    scm_add_prim(env, "time", time_prim, 0, 0);
    scm_add_prim(env, "rand", rand_prim, 0, 0);

    scm_add_prim(env, "?", help_prim, 0, 0);
    scm_add_prim(env, "exit", exit_prim, 0, 1);
    scm_add_prim(env, "set", set_prim, 2, -1);
}



int scm_load_file(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    scm_object *port = scm_make_file_input_port(file);
    scm_object *exp, *val;
    int ch;

    get_skip_encoding_marks(port);

    while (!scm_eofp(ch = scm_getc(port))) {
        scm_ungetc(ch, port);
        exp = scm_read(port);
        if (!exp) // 如果遇到错误，中止执行
            break;
        val = scm_eval(exp);
        if (!val) // 同上
            break;
        if (!SCM_VOIDP(val)) {
            scm_write(scm_stdout_port, val); // 默认用write
            printf("\n");
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
        scm_print_error("\n");
        scm_throw_eval_error();
    }

    return scm_void;
}

static scm_object* time_prim(int argc, scm_object *argv[])
{
    return scm_make_integer(time(NULL));
}

static scm_object* rand_prim(int argc, scm_object *argv[])
{
    return scm_make_integer(rand());
}

static scm_object* help_prim(int argc, scm_object *argv[])
{
    printf("%s\n", help_info);
    return scm_void;
}

static scm_object* exit_prim(int argc, scm_object *argv[])
{
    int code = 0;
    if (argc > 0) {
        if (!SCM_INTEGERP(argv[0]))
            return scm_wrong_contract("exit", "integer?", 0, argc, argv);
        code = SCM_INT_VAL(argv[0]);
    }
    exit(code);
    return scm_void;
}

static scm_object* set_prim(int argc, scm_object *argv[])
{
    if (!SCM_SYMBOLP(argv[0]))
        return scm_wrong_contract("set", "symbol?", 0, argc, argv);

    const char *name = SCM_SYMBOL_STR_VAL(argv[0]);

    int ok = 0;
    char *fail_cause = NULL;

    if (stricmp(name, "prompt") == 0) {
        if (SCM_STRINGP(argv[1])) {
            scm_g_repl_prompt = SCM_CHAR_STR_VAL(argv[1]);
            ok = 1;
        } else
            fail_cause = "argument is not string.";
    } else {
        fail_cause = "error option.";
    }

    if (!ok)
        printf("set failed, %s\n", fail_cause);

    return scm_void;
}

static int get_skip_encoding_marks(scm_object *port)
{
    int bytes[4] = {0x100};
    int read_bytes = 0;
    int c;
    while (read_bytes < 4 && !scm_eofp(c = scm_getc(port)))
        bytes[read_bytes++] = c;
    
    int encoding = ENCODING_UNKNOWN;
    int unread_bytes = read_bytes;
    /* 处理可能的常见的BOM */
    if ((bytes[0] == 0x00) && (bytes[1] == 0x00) && (bytes[2] == 0xFE) && (bytes[3] == 0xFF)) {
        encoding = ENCODING_UTF_32BE;
        unread_bytes = read_bytes - 4;
    } else if ((bytes[0] == 0xFF) && (bytes[1] == 0xFE) && (bytes[2] == 0x00) && (bytes[3] == 0x00)) {
        encoding = ENCODING_UTF_32LE;
        unread_bytes = read_bytes - 4;
    } else if ((bytes[0] == 0x2B) && (bytes[1] == 0x2F) && (bytes[2] == 0x76)
               && (bytes[3] == 0x38 || bytes[3] == 0x39 || bytes[3] == 0x2B || bytes[3] == 0x2F)) {
        encoding = ENCODING_UTF_7;
        unread_bytes = read_bytes - 4;
    } else if ((bytes[0] == 0x84) && (bytes[1] == 0x31) && (bytes[2] == 0x95) && (bytes[3] == 0x33)) {
        encoding = ENCODING_GB_18030;
        unread_bytes = read_bytes - 4;
    } else if ((bytes[0] == 0xEF) && (bytes[1] == 0xBB) && (bytes[2] == 0xBF)) {
        encoding = ENCODING_UTF_8;
        unread_bytes = read_bytes - 3;
    } else if ((bytes[0] == 0xFE) && (bytes[1] == 0xFF)) {
        encoding = ENCODING_UTF_16BE;
        unread_bytes = read_bytes - 2;
    } else if ((bytes[0] == 0xFF) && (bytes[1] == 0xFE)) {
        encoding = ENCODING_UTF_16LE;
        unread_bytes = read_bytes - 2;
    } // TODO: more

    // un-read, stack pop
    int i;
    for (int i = 0; i < unread_bytes; i++)
        scm_ungetc(bytes[read_bytes - 1 - i], port);

    return encoding;
}