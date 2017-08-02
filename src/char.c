#include "char.h"
#include "bool.h"
#include "env.h"

static scm_object* char_p_prim(int, scm_object *[]);

void scm_init_char(scm_env *env)
{
    scm_add_prim(env, "char?", char_p_prim, 1, 1);
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
