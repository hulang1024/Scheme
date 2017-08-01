#include "fun.h"
#include "eval.h"
#include "bool.h"
#include "list.h"
#include "env.h"
#include "error.h"

static scm_object* procedure_p_prim(int, scm_object *[]);
static scm_object* apply_prim(int, scm_object *[]);
static scm_object* void_prim(int, scm_object *[]);
static scm_object* void_p_prim(int, scm_object *[]);

void scm_init_fun(scm_env *env)
{
    scm_add_prim(env, "procedure?", procedure_p_prim, 1, 1);
    scm_add_prim(env, "apply", apply_prim, 2, -1);
    scm_add_prim(env, "void", void_prim, 0, 0);
    scm_add_prim(env, "void?", void_p_prim, 1, 1);
}

static scm_object* procedure_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_PROCEDUREP(argv[0]));
}

static scm_object* apply_prim(int argc, scm_object *argv[])
{
    if (!SCM_PROCEDUREP(argv[0]))
        return scm_wrong_contract("apply", "procedure?", 0, argc, argv);

    scm_object *list  = argv[1];
    scm_object **args = malloc(sizeof(scm_object *) * scm_list_length(list));
    int args_cnt = 0;
    while (!SCM_NULLP(list)) {
        if (SCM_PAIRP(list)) {
            args[args_cnt++] = SCM_CAR(list);
            list = SCM_CDR(list);
        } else {
            if (!SCM_NULLP(list))
                return scm_wrong_contract("apply", "list?", 1, argc, argv);
        }
    }
    return scm_apply(argv[0], args_cnt, args);
}

static scm_object* void_prim(int argc, scm_object *argv[])
{
    return scm_void;
}

static scm_object* void_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_VOIDP(argv[0]));
}
