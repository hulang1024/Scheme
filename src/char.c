#include <ctype.h>
#include "char.h"
#include "bool.h"
#include "number.h"
#include "env.h"
#include "error.h"

static scm_object* char_p_prim(int, scm_object *[]);
static scm_object* eq_p_prim(int, scm_object *[]);
static scm_object* lt_p_prim(int, scm_object *[]);
static scm_object* gt_p_prim(int, scm_object *[]);
static scm_object* lteq_p_prim(int, scm_object *[]);
static scm_object* gteq_p_prim(int, scm_object *[]);
static scm_object* ci_eq_p_prim(int, scm_object *[]);
static scm_object* ci_lt_p_prim(int, scm_object *[]);
static scm_object* ci_gt_p_prim(int, scm_object *[]);
static scm_object* ci_lteq_p_prim(int, scm_object *[]);
static scm_object* ci_gteq_p_prim(int, scm_object *[]);
static scm_object* alphabetic_p_prim(int, scm_object *[]);
static scm_object* numeric_p_prim(int, scm_object *[]);
static scm_object* whitespace_p_prim(int, scm_object *[]);
static scm_object* upper_case_p_prim(int, scm_object *[]);
static scm_object* lower_case_p_prim(int, scm_object *[]);
static scm_object* char_to_integer_prim(int, scm_object *[]);
static scm_object* integer_to_char_prim(int, scm_object *[]);
static scm_object* upcase_prim(int, scm_object *[]);
static scm_object* downcase_prim(int, scm_object *[]);

void scm_init_char(scm_env *env)
{
    scm_add_prim(env, "char?", char_p_prim, 1, 1);
    
    scm_add_prim(env, "char=?", eq_p_prim, 2, -1);
    scm_add_prim(env, "char<?", lt_p_prim, 2, -1);
    scm_add_prim(env, "char>?", gt_p_prim, 2, -1);
    scm_add_prim(env, "char<=?", lteq_p_prim, 2, -1);
    scm_add_prim(env, "char>=?", gteq_p_prim, 2, -1);
    scm_add_prim(env, "char-ci=?", ci_eq_p_prim, 2, -1);
    scm_add_prim(env, "char-ci<?", ci_lt_p_prim, 2, -1);
    scm_add_prim(env, "char-ci>?", ci_gt_p_prim, 2, -1);
    scm_add_prim(env, "char-ci<=?", ci_lteq_p_prim, 2, -1);
    scm_add_prim(env, "char-ci>=?", ci_gteq_p_prim, 2, -1);
    
    scm_add_prim(env, "char-alphabetic?", alphabetic_p_prim, 1, 1);
    scm_add_prim(env, "char-numeric?", numeric_p_prim, 1, 1);
    scm_add_prim(env, "char-whitespace?", whitespace_p_prim, 1, 1);
    scm_add_prim(env, "char-upper-case?", upper_case_p_prim, 1, 1);
    scm_add_prim(env, "char-lower-case?", lower_case_p_prim, 1, 1);
    
    scm_add_prim(env, "char->integer", char_to_integer_prim, 1, 1);
    scm_add_prim(env, "integer->char", integer_to_char_prim, 1, 1);
    
    scm_add_prim(env, "char-upcase", upcase_prim, 1, 1);
    scm_add_prim(env, "char-downcase", downcase_prim, 1, 1);
}

scm_object* scm_make_char(char val)
{
    scm_object *o = scm_malloc_object(sizeof(scm_char));
    o->type = scm_char_type;
    SCM_CHAR_VAL(o) = val;
    return o;
}

static scm_object* char_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_CHARP(argv[0]));
}

static scm_object* char_to_integer_prim(int argc, scm_object *argv[])
{
    if (SCM_CHARP(argv[0]))
        return scm_make_integer(SCM_CHAR_VAL(argv[0]));
    return scm_wrong_contract("char->integer", "char?", 0, argc, argv);
}

static scm_object* integer_to_char_prim(int argc, scm_object *argv[])
{
    if (SCM_INTEGERP(argv[0]))
        return scm_make_char(SCM_INT_VAL(argv[0]));
    return scm_wrong_contract("integer->char", "integer?", 0, argc, argv);
}

static scm_object* upcase_prim(int argc, scm_object *argv[])
{
    if (SCM_CHARP(argv[0]))
        return scm_make_char(toupper(SCM_CHAR_VAL(argv[0])));
    return scm_wrong_contract("char-upcase", "char?", 0, argc, argv);
}

static scm_object* downcase_prim(int argc, scm_object *argv[])
{
    if (SCM_CHARP(argv[0]))
        return scm_make_char(tolower(SCM_CHAR_VAL(argv[0])));
    return scm_wrong_contract("char-downcase", "char?", 0, argc, argv);
}


#define GEN_CHAR_COMP_PRIM(fname, scomp, ccomp) \
    static scm_object* fname##_p_prim(int argc, scm_object *argv[]) \
    { \
        int i; \
        for (i = 0; i < argc - 1; i++) { \
            if (SCM_CHARP(argv[i])) { \
                if (SCM_CHARP(argv[i + 1])) { \
                    if (!(SCM_CHAR_VAL(argv[i]) ccomp SCM_CHAR_VAL(argv[i + 1]))) \
                        return scm_false; \
                } else \
                    return scm_wrong_contract("char"#scomp"?", "char?", i + 1, argc, argv); \
            } else \
                return scm_wrong_contract("char"#scomp"?", "char?", i, argc, argv); \
        } \
        return scm_true; \
    }
    
#define GEN_CHAR_CI_COMP_PRIM(fname, scomp, ccomp) \
    static scm_object* ci_##fname##_p_prim(int argc, scm_object *argv[]) \
    { \
        int i; \
        for (i = 0; i < argc - 1; i++) { \
            if (SCM_CHARP(argv[i])) { \
                if (SCM_CHARP(argv[i + 1])) { \
                    if (!(toupper(SCM_CHAR_VAL(argv[i])) ccomp toupper(SCM_CHAR_VAL(argv[i + 1])))) \
                        return scm_false; \
                } else \
                    return scm_wrong_contract("char-ci"#scomp"?", "char?", i + 1, argc, argv); \
            } else \
                return scm_wrong_contract("char-ci"#scomp"?", "char?", i, argc, argv); \
        } \
        return scm_true; \
    }
    
GEN_CHAR_COMP_PRIM(eq, =, ==);
GEN_CHAR_COMP_PRIM(lt, <, <);
GEN_CHAR_COMP_PRIM(gt, >, >);
GEN_CHAR_COMP_PRIM(lteq, <=, <=);
GEN_CHAR_COMP_PRIM(gteq, >=, >=);
GEN_CHAR_CI_COMP_PRIM(eq, =, ==);
GEN_CHAR_CI_COMP_PRIM(lt, <, <);
GEN_CHAR_CI_COMP_PRIM(gt, >, >);
GEN_CHAR_CI_COMP_PRIM(lteq, <=, <=);
GEN_CHAR_CI_COMP_PRIM(gteq, >=, >=);


#define GEN_CHAR_TYPE_P_PRIM(tname, cfname) \
    static scm_object* tname##_p_prim(int argc, scm_object *argv[]) \
    { \
        if (SCM_CHARP(argv[0])) \
            return SCM_BOOL(cfname(SCM_CHAR_VAL(argv[0]))); \
        return scm_wrong_contract("char-"#tname"?", "char?", 0, argc, argv); \
    }
    
GEN_CHAR_TYPE_P_PRIM(alphabetic, isalpha);
GEN_CHAR_TYPE_P_PRIM(numeric, isdigit);
GEN_CHAR_TYPE_P_PRIM(whitespace, isspace);
GEN_CHAR_TYPE_P_PRIM(upper_case, isupper);
GEN_CHAR_TYPE_P_PRIM(lower_case, islower);
