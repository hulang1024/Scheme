#include "env.h"
#include "bool.h"
#include "number.h"
#include "symbol.h"
#include "char.h"
#include "str.h"
#include "list.h"
#include "vector.h"
#include "read.h"
#include "print.h"
#include "fun.h"
#include "eval.h"
#include "system.h"
#include "hashtable.h"
#include "lib/builtinlib.h"

int symbol_equal(void *x, void *y)
{
    return SCM_SAME_SYMBOL(x, y);
}

int symbol_hash(void *sym)
{
    /*
    int ret = 0;
    char *sc = SCM_SYMBOL_STR_VAL(sym);
    while(*sc++) {
        ret *= 10;
        r += *sc;
    }
    
    if (ret < 0) ret = -ret;
    
    return ret;
    */
    
    return SCM_SYMBOL_HASH(sym);
}

scm_env* scm_basic_env()
{
    scm_env *env = scm_env_new_frame(120);

    scm_init_bool(env);
    scm_init_char(env);
    scm_init_string(env);
    scm_init_number(env);
    scm_init_symbol(env);
    scm_init_list(env);
    scm_init_vector(env);
    scm_init_port(env);
    scm_init_print(env);
    scm_init_read(env);
    scm_init_fun(env);
    scm_init_eval(env);
    scm_init_system(env);

    scm_init_builtin_lib(env);

    return env;
}

scm_env* scm_env_new_frame(int size)
{
    scm_env *env = (scm_env *)scm_malloc_object(sizeof(scm_env));
    env->bindings = hashtable_new(size, symbol_equal, symbol_hash);
    env->parent = NULL;
    
    return env;
}

void scm_env_add_binding(scm_env *env, scm_symbol *id, scm_object *val)
{
    hashtable_set(env->bindings, id, val);
}

void scm_env_set_binding(scm_env *env, scm_symbol *id, scm_object *val)
{
    hashtable_set(env->bindings, id, val);
}

scm_env_entry* scm_env_lookup(scm_env *env, scm_symbol *id)
{
    scm_object *obj = NULL;
    scm_env *parent = env;
    
    while (((obj = hashtable_get(parent->bindings, id)) == NULL))
        parent = parent->parent;
        if (parent == NULL)
            break;
    }
    
    return obj;
}

void scm_add_prim(scm_env *env, const char *name, scm_prim prim, int min_arity, int max_arity)
{
    assert(0 <= min_arity && max_arity >= -1
        && (max_arity >= 0 ? min_arity <= max_arity : 1));
    // make primitive procedure
    scm_primitive_proc *pprim = (scm_primitive_proc *)scm_malloc_object(sizeof(scm_primitive_proc));
    ((scm_object *)pprim)->type = scm_primitive_type;
    pprim->name = name;
    pprim->prim = prim;
    pprim->min_arity = min_arity;
    pprim->max_arity = max_arity;

    scm_env_add_binding(env, scm_get_intern_symbol(name), (scm_object *)pprim);
}