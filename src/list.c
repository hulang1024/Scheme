#include "list.h"
#include "number.h"
#include "bool.h"
#include "env.h"
#include "error.h"

scm_object scm_null[1];

static scm_object* pair_p_prim(int, scm_object *[]);
static scm_object* null_p_prim(int, scm_object *[]);
static scm_object* list_p_prim(int, scm_object *[]);
static scm_object* cons_prim(int, scm_object *[]);
static scm_object* car_prim(int, scm_object *[]);
static scm_object* cdr_prim(int, scm_object *[]);
static scm_object* setcar_prim(int, scm_object *[]);
static scm_object* setcdr_prim(int, scm_object *[]);
static scm_object* list_prim(int, scm_object *[]);
static scm_object* length_prim(int, scm_object *[]);

void scm_init_list(scm_env *env)
{
    scm_null->type = scm_null_type;

    scm_add_prim(env, "pair?", pair_p_prim, 1, 1);
    scm_add_prim(env, "null?", null_p_prim, 1, 1);
    scm_add_prim(env, "list?", list_p_prim, 1, 1);
    scm_add_prim(env, "cons", cons_prim, 2, 2);
    scm_add_prim(env, "car", car_prim, 1, 1);
    scm_add_prim(env, "cdr", cdr_prim, 1, 1);
    scm_add_prim(env, "set-car!", setcar_prim, 2, 2);
    scm_add_prim(env, "set-cdr!", setcdr_prim, 2, 2);
    scm_add_prim(env, "list", list_prim, 0, -1);
    scm_add_prim(env, "length", length_prim, 1, 1);
}

scm_object* scm_make_pair(scm_object *car, scm_object *cdr)
{
    scm_object *pair = scm_malloc_object(sizeof(scm_pair));
    pair->type = scm_pair_type;
    SCM_PAIR_FLAGS(pair) = SCM_PAIR_FLAGS_INIT;
    SCM_CAR(pair) = car;
    SCM_CDR(pair) = cdr;
    return pair;
}

scm_object* scm_make_list_pair(scm_object *car, scm_object *cdr)
{
  scm_object *r = scm_make_pair(car, cdr);
  SCM_PAIR_FLAGS(r) |= SCM_PAIR_IS_LIST;
  return r;
}

scm_object* scm_build_list(int size, scm_object **argv)
{
    scm_object *pair = scm_null;
    int i;

    for (i = size; i--; ) {
        pair = SCM_CONS(argv[i], pair);
    }

    return pair;
}

int scm_list_length(scm_object *list)
{
    int len = 0;
    while (!SCM_NULLP(list)) {
        len++;
        if (SCM_PAIRP(list))
            list = SCM_CDR(list);
        else
            list = scm_null;
    }
    return len;
}

int scm_is_list(scm_object *obj)
{
    if (SCM_PAIRP(obj)) {
        if (SCM_PAIR_FLAGS(obj) & SCM_PAIR_IS_LIST)
            return 1;
        else {
            while (SCM_PAIRP(obj))
                obj = SCM_CDR(obj);
            if (SCM_NULLP(obj))
                return 1;
        }
    } else if (SCM_NULLP(obj))
        return 1;
    return 0;
}

static scm_object* pair_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_PAIRP(argv[0]));
}

static scm_object* null_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_NULLP(argv[0]));
}

static scm_object* list_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(scm_is_list(argv[0]));
}

static scm_object* cons_prim(int argc, scm_object *argv[])
{
    return SCM_CONS(argv[0], argv[1]);
}

static scm_object* car_prim(int argc, scm_object *argv[])
{
    if (!SCM_PAIRP(argv[0]))
        return scm_wrong_contract("car", "pair?", 0, argc, argv);

    return SCM_CAR(argv[0]);
}

static scm_object* cdr_prim(int argc, scm_object *argv[])
{
    if (!SCM_PAIRP(argv[0]))
        return scm_wrong_contract("cdr", "pair?", 0, argc, argv);

    return SCM_CDR(argv[0]);
}

static scm_object* setcar_prim(int argc, scm_object *argv[])
{
    if (!SCM_PAIRP(argv[0]))
        return scm_wrong_contract("set-car!", "pair?", 0, argc, argv);

    SCM_CAR(argv[0]) = argv[1];
    return scm_void;
}

static scm_object* setcdr_prim(int argc, scm_object *argv[])
{
    if (!SCM_PAIRP(argv[0]))
        return scm_wrong_contract("set-cdr!", "pair?", 0, argc, argv);

    SCM_CDR(argv[0]) = argv[1];
    return scm_void;
}

static scm_object* list_prim(int argc, scm_object *argv[])
{
    return scm_build_list(argc, argv);
}

static scm_object* length_prim(int argc, scm_object *argv[])
{
    if (!SCM_PAIRP(argv[0]))
        return scm_wrong_contract("length", "pair?", 0, argc, argv);

    return scm_make_integer(scm_list_length(argv[0]));
}