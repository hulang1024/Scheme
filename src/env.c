#include "env.h"
#include "bool.h"
#include "number.h"
#include "symbol.h"
#include "list.h"
#include "read.h"
#include "print.h"
#include "eval.h"
#include "fun.h"
#include "system.h"

scm_env* scm_basic_env()
{
    scm_env *env = (scm_env *)scm_malloc_object(sizeof(scm_env));
    env->rest = NULL;

    scm_init_bool(env);
    scm_init_number(env);
    scm_init_symbol(env);
    scm_init_list(env);
    scm_init_port(env);
    scm_init_print(env);
    scm_init_read(env);
    scm_init_system(env);
    scm_init_eval(env);
    scm_init_fun(env);

    return env;
}

void scm_env_add_binding(scm_env *env, scm_symbol *id, scm_object *val)
{
    while (env->rest) {
        env = env->rest;
    }

    env->rest = (scm_env *)scm_malloc_object(sizeof(scm_env));
    env->id = id;
    env->val = val;
    env->rest->rest = NULL;
}

scm_env_entry* scm_env_lookup(scm_env *env, scm_symbol *id)
{
    while (env) {
        if (SAME_PTR(env->id, id))
            return env;
        env = env->rest;
    }
    return NULL;
}

void scm_add_prim(scm_env *env, const char *name, scm_prim prim, int min_arity, int max_arity)
{
    // make primitive procedure
    scm_primitive_proc *pprim = (scm_primitive_proc *)scm_malloc_object(sizeof(scm_primitive_proc));
    ((scm_object *)pprim)->type = scm_primitive_type;
    pprim->name = name;
    pprim->prim = prim;
    pprim->min_arity = min_arity;
    pprim->max_arity = max_arity;

    scm_env_add_binding(env, scm_get_intern_symbol(name), (scm_object *)pprim);
}