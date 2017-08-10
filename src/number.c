#include "number.h"
#include "bool.h"
#include "env.h"
#include "error.h"

static scm_object* number_p_prim(int, scm_object *[]);
static scm_object* integer_p_prim(int, scm_object *[]);
static scm_object* real_p_prim(int, scm_object *[]);
static scm_object* zero_p_prim(int, scm_object *[]);
static scm_object* positive_p_prim(int, scm_object *[]);
static scm_object* negative_p_prim(int, scm_object *[]);
static scm_object* odd_p_prim(int, scm_object *[]);
static scm_object* even_p_prim(int, scm_object *[]);
static scm_object* plus_prim(int, scm_object *[]);
static scm_object* minus_prim(int, scm_object *[]);
static scm_object* mul_prim(int, scm_object *[]);
static scm_object* div_prim(int, scm_object *[]);
static scm_object* remainder_prim(int, scm_object *[]);
static scm_object* modulo_prim(int, scm_object *[]);
static scm_object* abs_prim(int, scm_object *[]);
static scm_object* eq_prim(int, scm_object *[]);
static scm_object* lt_prim(int, scm_object *[]);
static scm_object* gt_prim(int, scm_object *[]);
static scm_object* lteq_prim(int, scm_object *[]);
static scm_object* gteq_prim(int, scm_object *[]);

void scm_init_number(scm_env *env)
{
    scm_add_prim(env, "number?", number_p_prim, 1, 1);
    scm_add_prim(env, "integer?", integer_p_prim, 1, 1);
    scm_add_prim(env, "real?", real_p_prim, 1, 1);

    scm_add_prim(env, "zero?", zero_p_prim, 1, 1);
    scm_add_prim(env, "positive?", positive_p_prim, 1, 1);
    scm_add_prim(env, "negative?", negative_p_prim, 1, 1);
    scm_add_prim(env, "odd?", odd_p_prim, 1, 1);
    scm_add_prim(env, "even?", even_p_prim, 1, 1);

    scm_add_prim(env, "+", plus_prim, 0, -1);
    scm_add_prim(env, "-", minus_prim, 1, -1);
    scm_add_prim(env, "*", mul_prim, 0, -1);
    scm_add_prim(env, "/", div_prim, 1, -1);

    scm_add_prim(env, "remainder", remainder_prim, 2, 2);
    scm_add_prim(env, "modulo", modulo_prim, 2, 2);
    scm_add_prim(env, "abs", abs_prim, 1, 1);

    scm_add_prim(env, "=", eq_prim, 2, -1);
    scm_add_prim(env, "<", lt_prim, 2, -1);
    scm_add_prim(env, ">", gt_prim, 2, -1);
    scm_add_prim(env, "<=", lteq_prim, 2, -1);
    scm_add_prim(env, ">=", gteq_prim, 2, -1);
}

scm_object* scm_make_integer(long val)
{
    scm_object *o = scm_malloc_object(sizeof(scm_integer));
    o->type = scm_integer_type;
    SCM_INT_VAL(o) = val;
    return o;
}

scm_object* scm_make_float(double val)
{
    scm_object *o = scm_malloc_object(sizeof(scm_float));
    o->type = scm_float_type;
    SCM_FLOAT_VAL(o) = val;
    return o;
}

static scm_object* number_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_NUMBERP(argv[0]));
}

static scm_object* integer_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_INTEGERP(argv[0]));
}

static scm_object* real_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_INTEGERP(argv[0]) || SCM_FALSEP(argv[0]));
}

static scm_object* zero_p_prim(int argc, scm_object *argv[])
{
    if (SCM_INTEGERP(argv[0]))
        return SCM_BOOL(SCM_INT_VAL(argv[0]) == 0);
    if (SCM_FLOATP(argv[0]))
        return SCM_BOOL(SCM_FLOAT_VAL(argv[0]) == 0.0f);
    return scm_wrong_contract("zero?", "number?", 0, argc, argv);
}

#define GEN_POSI_NEGATIVE_PRIM(name, sname, op) \
    static scm_object* name(int argc, scm_object *argv[]) \
    { \
        if (SCM_INTEGERP(argv[0])) \
            return SCM_BOOL(SCM_INT_VAL(argv[0]) op 0); \
        if (SCM_FLOATP(argv[0])) \
            return SCM_BOOL(SCM_FLOAT_VAL(argv[0]) op 0.0f); \
        return scm_wrong_contract(sname, "real?", 0, argc, argv); \
    }

GEN_POSI_NEGATIVE_PRIM(positive_p_prim, "positive?", >);
GEN_POSI_NEGATIVE_PRIM(negative_p_prim, "negative?", <);

#define GEN_ODD_EVEN_PRIM(name, sname, equal) \
    static scm_object* name(int argc, scm_object *argv[]) \
    { \
        if (!SCM_INTEGERP(argv[0])) \
            return scm_wrong_contract(sname, "integer?", 0, argc, argv); \
        return SCM_BOOL((SCM_INT_VAL(argv[0]) % 2) equal 0); \
    }
GEN_ODD_EVEN_PRIM(odd_p_prim, "odd?", !=);
GEN_ODD_EVEN_PRIM(even_p_prim, "even?", ==);

#define CHECK_AND_GET_MAX_TYPE(op) \
    int max_type = scm_integer_type; \
    int argi; \
    for (argi = 0; argi < argc; argi++) { \
        switch (argv[argi]->type) { \
            case scm_integer_type: \
                if (scm_integer_type > max_type) \
                    max_type = scm_integer_type;\
                break; \
            case scm_float_type: \
                if (scm_float_type > max_type) \
                    max_type = scm_float_type; \
                break; \
            default: \
                return scm_wrong_contract(#op, "number?", argi, argc, argv);\
        } \
    }

#define GEN_PLUS_OR_MUL_PRIM_ACCUMULATE(op) \
    switch (argv[0]->type) { \
        case scm_integer_type:\
            ret = SCM_INT_VAL(argv[0]); \
            break; \
        case scm_float_type: \
            ret = SCM_FLOAT_VAL(argv[0]); \
            break; \
    } \
    for (argi = 1; argi < argc; argi++) { \
        switch (argv[argi]->type) { \
            case scm_integer_type: \
                ret op##= SCM_INT_VAL(argv[argi]);\
                break; \
            case scm_float_type: \
                ret op##= SCM_FLOAT_VAL(argv[argi]);\
                break; \
        } \
    }
#define GEN_PLUS_OR_MUL_PRIM(fname, op, init) static scm_object* fname##_prim(int argc, scm_object *argv[]) { \
    if (argc == 0) \
        return scm_make_integer(init); \
    CHECK_AND_GET_MAX_TYPE(op) \
    switch (max_type) { \
        case scm_integer_type: { \
            long ret; \
            GEN_PLUS_OR_MUL_PRIM_ACCUMULATE(op) \
            return scm_make_integer(ret); \
        } \
        case scm_float_type: { \
            double ret; \
            GEN_PLUS_OR_MUL_PRIM_ACCUMULATE(op) \
            return scm_make_float(ret); \
        } \
    } \
}

#define GEN_MINUS_OR_DIV_PRIM_ACCUMULATE(op, init) \
    switch (argv[0]->type) { \
        case scm_integer_type:\
            ret = SCM_INT_VAL(argv[0]); \
            break; \
        case scm_float_type: \
            ret = SCM_FLOAT_VAL(argv[0]); \
            break; \
    } \
    if (argc == 1) \
        ret = init op ret; \
    else \
    for (argi = 1; argi < argc; argi++) { \
        switch (argv[argi]->type) { \
            case scm_integer_type: \
                ret op##= SCM_INT_VAL(argv[argi]);\
                break; \
            case scm_float_type: \
                ret op##= SCM_FLOAT_VAL(argv[argi]);\
                break; \
        } \
    }
#define GEN_MINUS_OR_DIV_PRIM(fname, op, init) static scm_object* fname##_prim(int argc, scm_object *argv[]) { \
    CHECK_AND_GET_MAX_TYPE(op) \
    switch (max_type) { \
        case scm_integer_type: { \
            long ret; \
            GEN_MINUS_OR_DIV_PRIM_ACCUMULATE(op, init) \
            return scm_make_integer(ret); \
        } \
        case scm_float_type: { \
            double ret; \
            GEN_MINUS_OR_DIV_PRIM_ACCUMULATE(op, init) \
            return scm_make_float(ret); \
        } \
    } \
}


#define EQ_PRIM_FOREACH(first_val) \
    for (argi = 1; argi < argc; argi++) { \
        switch (argv[argi]->type) { \
            case scm_integer_type: \
                if (first_val != SCM_INT_VAL(argv[argi]))\
                    return scm_false; \
                break; \
            case scm_float_type: \
                if (first_val != SCM_FLOAT_VAL(argv[argi]))\
                    return scm_false; \
                break; \
            default: \
                return scm_wrong_contract("=", "number?", argi, argc, argv);\
        }\
    }

static scm_object* eq_prim(int argc, scm_object *argv[])
{
    scm_object *first = argv[0];
    int argi;

    switch (first->type) {
        case scm_integer_type:
            EQ_PRIM_FOREACH(SCM_INT_VAL(first))
            break;
        case scm_float_type:
            EQ_PRIM_FOREACH(SCM_FLOAT_VAL(first))
            break;
        default:
            return scm_wrong_contract("=", "number?", 0, argc, argv);
    }

    return scm_true;
}

#define COMP_PRIM_CMP_WITH_NEXT_ARG(op, prev_arg_val) \
    switch (argv[argi + 1]->type) { \
        case scm_integer_type: \
            if (!(prev_arg_val op SCM_INT_VAL(argv[argi + 1]))) \
                return scm_false; \
            break; \
        case scm_float_type: \
            if (!(prev_arg_val op SCM_FLOAT_VAL(argv[argi + 1]))) \
                return scm_false; \
            break; \
        default: \
            return scm_wrong_contract(#op, "real?", argi + 1, argc, argv);\
    }
#define GEN_COMP_PRIM(fname, op) static scm_object* fname##_prim(int argc, scm_object *argv[]) { \
    int argi; \
    for (argi = 0; argi < argc - 1; argi++) { \
        switch (argv[argi]->type) { \
            case scm_integer_type: \
                COMP_PRIM_CMP_WITH_NEXT_ARG(op, SCM_INT_VAL(argv[argi])) \
                break; \
            case scm_float_type: \
                COMP_PRIM_CMP_WITH_NEXT_ARG(op, SCM_FLOAT_VAL(argv[argi])) \
                break; \
            default: \
                return scm_wrong_contract(#op, "real?", argi, argc, argv); \
        } \
    } \
    return scm_true; \
}

GEN_PLUS_OR_MUL_PRIM(plus, +, 0);
GEN_PLUS_OR_MUL_PRIM(mul, *, 1);
GEN_MINUS_OR_DIV_PRIM(minus, -, 0);
GEN_MINUS_OR_DIV_PRIM(div, /, 1);
GEN_COMP_PRIM(lt, <);
GEN_COMP_PRIM(gt, >);
GEN_COMP_PRIM(lteq, <=);
GEN_COMP_PRIM(gteq, >=);


static scm_object* remainder_prim(int argc, scm_object *argv[])
{
    if (!SCM_INTEGERP(argv[0]))
        return scm_wrong_contract("remainder", "integer?", 0, argc, argv);
    if (!SCM_INTEGERP(argv[1]))
        return scm_wrong_contract("remainder", "integer?", 1, argc, argv);

    return scm_make_integer(SCM_INT_VAL(argv[0]) % SCM_INT_VAL(argv[1]));
}

static scm_object* modulo_prim(int argc, scm_object *argv[])
{
    if (!SCM_INTEGERP(argv[0]))
        return scm_wrong_contract("modulo", "integer?", 0, argc, argv);
    if (!SCM_INTEGERP(argv[1]))
        return scm_wrong_contract("modulo", "integer?", 1, argc, argv);

    return scm_make_integer(SCM_INT_VAL(argv[0]) % SCM_INT_VAL(argv[1]));
}

static scm_object* abs_prim(int argc, scm_object *argv[])
{
    if (SCM_INTEGERP(argv[0])) {
        int n = SCM_INT_VAL(argv[0]);
        return scm_make_integer(n < 0 ? - n : n);
    }
    if (SCM_FLOATP(argv[0])) {
        double n = SCM_FLOAT_VAL(argv[0]);
        return scm_make_float(n < 0 ? - n : n);
    }

    return scm_wrong_contract("abs", "number?", 0, argc, argv);
}