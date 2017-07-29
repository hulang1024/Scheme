#include "number.h"
#include "bool.h"
#include "env.h"

static scm_object* number_p_prim(int, scm_object *[]);
static scm_object* integer_p_prim(int, scm_object *[]);
static scm_object* plus_prim(int, scm_object *[]);
static scm_object* minus_prim(int, scm_object *[]);
static scm_object* mul_prim(int, scm_object *[]);
static scm_object* div_prim(int, scm_object *[]);

void scm_init_number(scm_env *env)
{
    scm_int_zero->type = scm_integer_type;
    SCM_INT_VAL(scm_int_zero) = 0;

    scm_add_prim(env, "number?", number_p_prim, 0, -1);
    scm_add_prim(env, "integer?", integer_p_prim, 0, -1);
    scm_add_prim(env, "+", plus_prim, 0, -1);
    scm_add_prim(env, "-", minus_prim, 1, -1);
    scm_add_prim(env, "*", mul_prim, 0, -1);
    scm_add_prim(env, "/", div_prim, 1, -1);
}

scm_object* scm_make_integer(int val)
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
    return SCM_INTEGERP(argv[0]) || SCM_FALSEP(argv[0]) ? scm_true : scm_false;
}

static scm_object* integer_p_prim(int argc, scm_object *argv[])
{
    return SCM_INTEGERP(argv[0]) ? scm_true : scm_false;
}

static scm_object* plus_prim(int argc, scm_object *argv[])
{
    if(argc > 0) {
        if (SCM_INTEGERP(argv[0]))
            return scm_make_integer(SCM_INT_VAL(argv[0]) + SCM_INT_VAL(argv[1]));
        //if(SCM_FALSEP(argv[0]))
        return scm_make_float(SCM_FLOAT_VAL(argv[0]) + SCM_FLOAT_VAL(argv[1]));
    } else {
        return scm_int_zero;
    }
}

static scm_object* minus_prim(int argc, scm_object *argv[])
{
    if (SCM_INTEGERP(argv[0]))
        return scm_make_integer(SCM_INT_VAL(argv[0]) - SCM_INT_VAL(argv[1]));
    //if(SCM_FALSEP(argv[0]))
    return scm_make_float(SCM_FLOAT_VAL(argv[0]) - SCM_FLOAT_VAL(argv[1]));
}

static scm_object* mul_prim(int argc, scm_object *argv[])
{
    if(argc > 0) {
        if (SCM_INTEGERP(argv[0]))
            return scm_make_integer(SCM_INT_VAL(argv[0]) * SCM_INT_VAL(argv[1]));
        //if(SCM_FALSEP(argv[0]))
        return scm_make_float(SCM_FLOAT_VAL(argv[0]) * SCM_FLOAT_VAL(argv[1]));
    } else {
        return scm_int_zero;
    }
}

static scm_object* div_prim(int argc, scm_object *argv[])
{
    if (SCM_INTEGERP(argv[0]))
        return scm_make_integer(SCM_INT_VAL(argv[0]) / SCM_INT_VAL(argv[1]));
    //if(SCM_FALSEP(argv[0]))
    return scm_make_float(SCM_FLOAT_VAL(argv[0]) / SCM_FLOAT_VAL(argv[1]));
}
