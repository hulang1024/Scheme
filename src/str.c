#include "string.h"
#include "bool.h"
#include "env.h"

static scm_object* string_p_prim(int, scm_object *[]);

void scm_init_string(scm_env *env)
{
    scm_add_prim(env, "string?", string_p_prim, 1, 1);
}

scm_object* scm_make_string(char *str)
{
    scm_object *o = scm_malloc_object(sizeof(scm_string));
    o->type = scm_string_type;
    SCM_CHAR_STR_VAL(o) = str;
    return o;
}

static scm_object* string_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_STRINGP(argv[0]));
}