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
static scm_object* list_tail_prim(int, scm_object *[]);
static scm_object* list_ref_prim(int, scm_object *[]);
static scm_object* length_prim(int, scm_object *[]);
static scm_object* append_prim(int, scm_object *[]);
static scm_object* reverse_prim(int, scm_object *[]);
static scm_object* memq_prim(int, scm_object *[]);
static scm_object* memv_prim(int, scm_object *[]);
static scm_object* member_prim(int, scm_object *[]);

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
    
    scm_add_prim(env, "list-tail", list_tail_prim, 2, 2);
    scm_add_prim(env, "list-ref", list_ref_prim, 2, 2);
    scm_add_prim(env, "length", length_prim, 1, 1);
    scm_add_prim(env, "append", append_prim, 0, -1);
    scm_add_prim(env, "reverse", reverse_prim, 1, 1);

    scm_add_prim(env, "memq", memq_prim, 2, 2);
    scm_add_prim(env, "memv", memv_prim, 2, 2);
    scm_add_prim(env, "member", member_prim, 2, 2);
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

/*
 * 返回具有一个“是表”标记的序对（但不在结尾增加空表）
 */
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
        pair = SCM_LCONS(argv[i], pair); /* using SCM_LCONS ! */
    }

    return pair;
}
/*
 * @list1 严格表
 * @list2 表
 */
scm_object* scm_append_list2(scm_object *list1, scm_object *list2)
{
    // TODO: iteration
    return SCM_PAIRP(list1) ?
           SCM_CONS(SCM_CAR(list1), scm_append_list2(SCM_CDR(list1), list2)) : list2;
}

/*
 * 假设list是一个序对（可以是非严格表），返回其元素个数
 */
int scm_list_length(scm_object *list)
{
    int len = 0;
    /* 遍历list，直到空表 */
    while (!SCM_NULLP(list)) {
        len++;
        if (SCM_PAIRP(list))
            list = SCM_CDR(list);
        else
            list = scm_null; /* 设空表以结束 */
    }
    return len;
}

/**
 * 是否一个严格表
 */
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

static scm_object* do_checked_list_ref(const char *name, int takecar, int argc, scm_object *argv[]) {
    if(!SCM_PAIRP(argv[0]))
        return scm_wrong_contract(name, "pair?", 0, argc, argv);
    if(!is_exact_nonnegative_integer(argv[1]))
        return scm_wrong_contract(name, "exact-nonnegative-integer?", 1, argc, argv);
    
    scm_object *list = argv[0];
    int index = SCM_INT_VAL(argv[1]);
    while (index > 0) {
        index--;
        if (!SCM_PAIRP(list))
            return scm_wrong_contract("cdr", "pair?", 0, argc, argv);
        list = SCM_CDR(list);
    }
    
    if (takecar) {
        if (!SCM_PAIRP(list))
            return scm_wrong_contract("car", "pair?", 0, argc, argv);

        return SCM_CAR(list);
    } else
        return list;
}

static scm_object* list_tail_prim(int argc, scm_object *argv[])
{
    return do_checked_list_ref("list-tail", 0, argc, argv);
}

static scm_object* list_ref_prim(int argc, scm_object *argv[])
{
    return do_checked_list_ref("list-ref", 1, argc, argv);
}

static scm_object* length_prim(int argc, scm_object *argv[])
{
    if (!scm_is_list(argv[0]))
        return scm_wrong_contract("length", "list?", 0, argc, argv);

    return scm_make_integer(scm_list_length(argv[0]));
}

static scm_object* append_prim(int argc, scm_object *argv[])
{
    scm_object *ret = scm_null;
    if (argc > 0) {
        int i;
        for (i = 0; i < argc - 1; i++) {
            if (!scm_is_list(argv[0]))
                return scm_wrong_contract("append", "list?", i, argc, argv);
            ret = scm_append_list2(ret, argv[i]);
        }
        ret = scm_append_list2(ret, argv[i]);
    }

    return ret;
}

static scm_object* reverse_prim(int argc, scm_object *argv[])
{
    if (!scm_is_list(argv[0]))
        return scm_wrong_contract("reverse", "list?", 0, argc, argv);

    scm_object *ret = scm_null;
    scm_object *list = argv[0];
    scm_list_for_each(list) {
        ret = SCM_LCONS(SCM_CAR(list), ret);
    }

    return ret;
}

#define GEN_MEMBER_PRIM(name, scm_name, eq) \
    static scm_object* name(int argc, scm_object *argv[]) \
    { \
        if (!scm_is_list(argv[1])) \
            return scm_wrong_contract(scm_name, "list?", 1, argc, argv); \
    \
        scm_object *obj = argv[0]; \
        scm_object *list = argv[1]; \
        scm_list_for_each(list) { \
            if (scm_eq(SCM_CAR(list), obj)) \
                return list; \
        } \
        return scm_false; \
    }

GEN_MEMBER_PRIM(memq_prim, "memq", eq?);
GEN_MEMBER_PRIM(memv_prim, "memv", eqv?);
GEN_MEMBER_PRIM(member_prim, "member", equal?);