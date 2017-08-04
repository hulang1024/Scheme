#include <string.h>
#include "str.h"
#include "bool.h"
#include "number.h"
#include "char.h"
#include "list.h"
#include "env.h"
#include "error.h"

scm_object scm_empty_string[0];

static scm_object* string_p_prim(int, scm_object *[]);
static scm_object* make_string_prim(int, scm_object *[]);
static scm_object* string_prim(int, scm_object *[]);
static scm_object* string_length_prim(int, scm_object *[]);
static scm_object* string_ref_prim(int, scm_object *[]);
static scm_object* string_set_prim(int, scm_object *[]);
static scm_object* substring_prim(int, scm_object *[]);
static scm_object* string_append_prim(int, scm_object *[]);
static scm_object* string_copy_prim(int, scm_object *[]);
static scm_object* string_fill_prim(int, scm_object *[]);
static scm_object* string_to_list_prim(int, scm_object *[]);
static scm_object* list_to_string_prim(int, scm_object *[]);

void scm_init_string(scm_env *env)
{
    scm_empty_string->type = scm_string_type;
    SCM_CHAR_STR_VAL(scm_empty_string) = "";

    scm_add_prim(env, "string?", string_p_prim, 1, 1);
    scm_add_prim(env, "make-string", make_string_prim, 1, 2);
    scm_add_prim(env, "string", string_prim, 0, -1);
    scm_add_prim(env, "string-length", string_length_prim, 1, 1);
    scm_add_prim(env, "string-ref", string_ref_prim, 2, 2);
    scm_add_prim(env, "string-set!", string_set_prim, 3, 3);
    scm_add_prim(env, "substring", substring_prim, 3, 3);
    scm_add_prim(env, "string-append", string_append_prim, 0, -1);
    scm_add_prim(env, "string->list", string_to_list_prim, 1, 1);
    scm_add_prim(env, "list->string", list_to_string_prim, 1, 1);
    scm_add_prim(env, "string-copy", string_copy_prim, 1, 1);
    scm_add_prim(env, "string-fill!", string_fill_prim, 2, 2);
}

scm_object* scm_make_string(char *str, int len)
{
    scm_object *o = scm_malloc_object(sizeof(scm_string));
    o->type = scm_string_type;
    SCM_CHAR_STR_VAL(o) = str;
    SCM_STR_LEN(o) = len;
    return o;
}

static scm_object* string_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_STRINGP(argv[0]));
}

static scm_object* make_string_prim(int argc, scm_object *argv[])
{
    if (!is_exact_nonnegative_integer(argv[0]))
        return scm_wrong_contract("make-string", "exact-nonnegative-integer?", 0, argc, argv);

    char c = '\0';
    if (argc > 1) {
        if (!SCM_CHARP(argv[1]))
            return scm_wrong_contract("make-string", "char?", 1, argc, argv);

        c = SCM_CHAR_VAL(argv[1]);
    }
    
    int len = SCM_INT_VAL(argv[0]);
    char *cs = malloc(sizeof(char) * len + 1);
    int i;
    for (i = 0; i < len; i++)
        cs[i] = c;
    cs[i] = '\0';

    return scm_make_string(cs, len);
}

static scm_object* string_prim(int argc, scm_object *argv[])
{
    int i;
    char *cs = malloc(sizeof(char) * argc + 1);
    for (i = 0; i < argc; i++) {
        if (!SCM_CHARP(argv[i]))
            return scm_wrong_contract("string", "char?", i, argc, argv);

        cs[i] = SCM_CHAR_VAL(argv[i]);
    }
    cs[i] = '\0';
    
    return scm_make_string(cs, argc);
}

static scm_object* string_length_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("string-length", "string?", 0, argc, argv);

    return scm_make_integer(SCM_STR_LEN(argv[0]));
}

static scm_object* string_ref_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("string-ref", "string?", 0, argc, argv);
    if (!is_exact_nonnegative_integer(argv[1]))
        return scm_wrong_contract("string-ref", "exact-nonnegative-integer?", 1, argc, argv);

    int len = SCM_STR_LEN(argv[0]);
    int k = SCM_INT_VAL(argv[1]);
    if (!(0 <= k && k < len))
        return scm_out_of_range("string-ref", argv[0], k, k, 0);

    return scm_make_char(SCM_CHAR_STR_VAL(argv[0])[k]);
}

static scm_object* string_set_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("string-set!", "string?", 0, argc, argv);
    if (!is_exact_nonnegative_integer(argv[1]))
        return scm_wrong_contract("string-set!", "exact-nonnegative-integer?", 1, argc, argv);
    if (!SCM_CHARP(argv[2]))
        return scm_wrong_contract("string-set!", "char?", 2, argc, argv);

    int len = SCM_STR_LEN(argv[0]);
    int k = SCM_INT_VAL(argv[1]);
    if (!(0 <= k && k < len))
        return scm_out_of_range("string-set!", argv[0], k, k, 0);
    
    SCM_CHAR_STR_VAL(argv[0])[k] = SCM_CHAR_VAL(argv[2]);
    
    return scm_void;
}

static scm_object* substring_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("substring", "string?", 0, argc, argv);
    if (!is_exact_nonnegative_integer(argv[1]))
        return scm_wrong_contract("substring", "exact-nonnegative-integer?", 1, argc, argv);
    if (!is_exact_nonnegative_integer(argv[2]))
        return scm_wrong_contract("substring", "exact-nonnegative-integer?", 2, argc, argv);
    
    int len = SCM_STR_LEN(argv[0]);
    int start = SCM_INT_VAL(argv[1]);
    int end = SCM_INT_VAL(argv[2]);
    if ((0 <= start && start <= len) && (start <= end && end <= len)) {
        len = end - start;
        char *cs = malloc(sizeof(char) * len + 1);
        strncpy(cs, SCM_CHAR_STR_VAL(argv[0]) + start, len);
        cs[len] = '\0';
        return scm_make_string(cs, len);
    } else {
        return scm_out_of_range("substring", argv[0], start, end, 0);
    }
}

static scm_object* string_append_prim(int argc, scm_object *argv[])
{
    if (argc == 0)
        return scm_empty_string;

    int len = 0;
    int i;
    for (i = 0; i < argc; i++) {
        if (!SCM_STRINGP(argv[i]))
            return scm_wrong_contract("string-append", "string?", i, argc, argv);
        len += SCM_STR_LEN(argv[i]);
    }

    char *cs = malloc(sizeof(char) * len + 1);
    int end = 0;
    for (i = 0; i < argc; i++) {
        strcpy(cs + end, SCM_CHAR_STR_VAL(argv[i]));
        end += SCM_STR_LEN(argv[i]);
    }

    return scm_make_string(cs, len);
}

static scm_object* string_to_list_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("string->list", "string?", 0, argc, argv);

    scm_object *ret = scm_null;
    char *cs = SCM_CHAR_STR_VAL(argv[0]);
    int i;
    for(i = SCM_STR_LEN(argv[0]) - 1; i >= 0; i--)
        ret = SCM_CONS(scm_make_char(cs[i]), ret);
    return ret;
}

static scm_object* list_to_string_prim(int argc, scm_object *argv[])
{
    if (!SCM_LISTP(argv[0]))
        return scm_wrong_contract("list->string", "(listof char?)", 0, argc, argv);

    int len = 0;
    scm_object *list = argv[0];
    while (!SCM_NULLP(list)) {
        if (SCM_PAIRP(list)) {
            len++;
            if (!SCM_CHARP(SCM_CAR(list)))
                return scm_wrong_contract("list->string", "(listof char?)", 0, argc, argv);
            list = SCM_CDR(list);
        } else {
            if (!SCM_NULLP(list))
                return scm_wrong_contract("list->string", "(listof char?)", 0, argc, argv);
        }
    }
    char *cs = malloc(sizeof(char) * len + 1);
    list = argv[0];
    int i;
    for (i = 0; i < len; i++) {
        cs[i] = SCM_CHAR_VAL(SCM_CAR(list));
        list = SCM_CDR(list);
    }
    cs[i] = '\0';
    return scm_make_string(cs, len);
}

static scm_object* string_copy_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("string-copy", "string?", 0, argc, argv);

    char *cs = malloc(sizeof(char) * SCM_STR_LEN(argv[0]) + 1);
    strcpy(cs, SCM_CHAR_STR_VAL(argv[0]));
    return scm_make_string(cs, SCM_STR_LEN(argv[0]));
}

static scm_object* string_fill_prim(int argc, scm_object *argv[])
{
    if (!SCM_STRINGP(argv[0]))
        return scm_wrong_contract("string-fill!", "string?", 0, argc, argv);
    if (!SCM_CHARP(argv[1]))
        return scm_wrong_contract("string-fill!", "char?", 1, argc, argv);

    char *cs = SCM_CHAR_STR_VAL(argv[0]);
    char c = SCM_CHAR_VAL(argv[1]);
    char *sc = cs;
    while (*sc) *sc++ = c;

    return scm_void;
}
