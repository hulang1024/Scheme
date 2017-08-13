#include "print.h"
#include "list.h"
#include "bool.h"
#include "env.h"
#include "scm.h"
#include "pcc32.h"

#define WRITE_TEXT_COLOR LIGHT_BLUE
#define DISPLAY_TEXT_COLOR LIGHT_MAGENTA

enum {
    DISPLAY = 0,
    WRITE = 1
};

static scm_object* write_prim(int, scm_object *[]);
static scm_object* display_prim(int, scm_object *[]);
static scm_object* newline_prim(int, scm_object *[]);

static void write(scm_object *port, scm_object *, int);
static void write_list(scm_object *port, scm_object *, int);
static void write_vector(scm_object *port, scm_object *, int);

void scm_init_print(scm_env *env)
{
    scm_add_prim(env, "write", write_prim, 1, 2);
    scm_add_prim(env, "display", display_prim, 1, 2);
}

void scm_write(scm_object *port, scm_object *obj)
{
    int oc = getTextColor();
    setTextColor(WRITE_TEXT_COLOR); // 仅交互调用时设置颜色

    write(port, obj, WRITE);

    setTextColor(oc);
}

void scm_display(scm_object *port, scm_object *obj)
{
    int oc = getTextColor();
    setTextColor(DISPLAY_TEXT_COLOR);

    write(port, obj, DISPLAY);

    setTextColor(oc);
}

static scm_object* write_prim(int argc, scm_object *argv[])
{
    write(scm_stdout_port, argv[0], WRITE);
    return scm_void;
}

static scm_object* display_prim(int argc, scm_object *argv[])
{
    write(scm_stdout_port, argv[0], DISPLAY);
    return scm_void;
}

static void write(scm_object *port, scm_object *obj, int notdisplay)
{
    switch (SCM_TYPE(obj)) {
        case scm_true_type:
            scm_write_cstr(port, "#t");
            break;
        case scm_false_type:
            scm_write_cstr(port, "#f");
            break;
        case scm_integer_type: {
            char s[11] = {0};
            sprintf(s, "%ld", SCM_INT_VAL(obj));
            scm_write_cstr(port, s);
            break;
        }
        case scm_float_type: {
            char s[17] = {0};
            sprintf(s, "%lf", SCM_FLOAT_VAL(obj));
            scm_write_cstr(port, s);
            break;
        }
        case scm_char_type:
            if (notdisplay) {
                switch (SCM_CHAR_VAL(obj)) {
                    case '\n':
                        scm_write_cstr(port, "#\\newline");
                        break;
                    case ' ':
                        scm_write_cstr(port, "#\\space");
                        break;
                    default:
                        scm_write_cstr(port, "#\\");
                        scm_putc(port, SCM_CHAR_VAL(obj));
                }
            } else {
                scm_putc(port, SCM_CHAR_VAL(obj));
            }
            break;
        case scm_string_type:
            if (notdisplay) {
                scm_write_cstr(port, "\"");
                char *str = SCM_CHAR_STR_VAL(obj);
                char *sc;
                char *ps;
                for (sc = str; *sc; sc++) {
                    ps = NULL;
                    switch (*sc) {
                        case '\a': ps = "\\a"; break;
                        case '\b': ps = "\\b"; break;
                        case '\f': ps = "\\f"; break;
                        case '\n': ps = "\\n"; break;
                        case '\r': ps = "\\r"; break;
                        case '\t': ps = "\\t"; break;
                        case '\v': ps = "\\v"; break;
                    }
                    if (ps == NULL)
                        scm_putc(port, *sc);
                    else
                        scm_write_cstr(port, ps);
                }
                scm_write_cstr(port, "\"");
            }
            else
                scm_write_cstr(port, SCM_CHAR_STR_VAL(obj));
            break;
        case scm_symbol_type:
            scm_write_cstr(port, SCM_SYMBOL_STR_VAL(obj));
            break;
        case scm_pair_type:
            write_list(port, obj, notdisplay);
            break;
        case scm_vector_type:
            write_vector(port, obj, notdisplay);
            break;  
        case scm_null_type:
            scm_write_cstr(port, "()");
            break;
        case scm_primitive_type:
            scm_write_cstr(port, "#<procedure:");
            scm_write_cstr(port,((scm_primitive_proc *)obj)->name);
            scm_write_cstr(port, ">");
            break;
        case scm_compound_type:
            scm_write_cstr(port, "#<procedure:");
            scm_write_cstr(port, ((scm_compound_proc *)obj)->name ? ((scm_compound_proc *)obj)->name : "");
            scm_write_cstr(port, ">");
            break;
        case scm_namespace_type:
            scm_write_cstr(port, "#<namespace>");
            break;
        case scm_void_type:
            scm_write_cstr(port, "#<void>");
            break;
        default: ;
    }
}

static void write_list(scm_object *port, scm_object *list, int notdisplay)
{

    scm_putc(port, '(');
    while (!SCM_NULLP(list)) {
        if (SCM_PAIRP(list)) {
            write(port, SCM_CAR(list), notdisplay);
            if (!SCM_NULLP(SCM_CDR(list))) {
                scm_putc(port, ' ');
                list = SCM_CDR(list);
            } else {
                list = scm_null;
            }
        } else {
            scm_putc(port, '.');
            scm_putc(port, ' ');
            write(port, list, notdisplay);
            list = scm_null;
        }
    }
    scm_putc(port, ')');
}

static void write_vector(scm_object *port, scm_object *vector, int notdisplay)
{
    scm_putc(port, '#');
    scm_putc(port, '(');
    int len = SCM_VECTOR_LEN(vector);
    scm_object **elems = SCM_VECTOR_ELEMS(vector);
    int i;
    for (i = 0; i < len; i++) {
        write(port, elems[i], notdisplay);
        if (i + 1 < len)
            scm_putc(port, ' ');
    }
    scm_putc(port, ')');
}