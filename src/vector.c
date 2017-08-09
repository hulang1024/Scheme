#include "vector.h"
#include "number.h"
#include "bool.h"
#include "list.h"
#include "env.h"
#include "error.h"

static scm_object* vector_p_prim(int, scm_object *[]);
static scm_object* make_vector_prim(int, scm_object *[]);
static scm_object* vector_prim(int, scm_object *[]);
static scm_object* vector_set_prim(int, scm_object *[]);
static scm_object* vector_ref_prim(int, scm_object *[]);
static scm_object* vector_length_prim(int, scm_object *[]);
static scm_object* vector_to_list_prim(int, scm_object *[]);
static scm_object* list_to_vector_prim(int, scm_object *[]);
static scm_object* vector_fill_prim(int, scm_object *[]);

void scm_init_vector(scm_env *env)
{
    scm_add_prim(env, "vector?", vector_p_prim, 1, 1);
    scm_add_prim(env, "make-vector", make_vector_prim, 1, 2);
    scm_add_prim(env, "vector", vector_prim, 0, -1);
    scm_add_prim(env, "vector-set!", vector_set_prim, 3, 3);
    scm_add_prim(env, "vector-ref", vector_ref_prim, 2, 2);
    scm_add_prim(env, "vector-length", vector_length_prim, 1, 1);
    scm_add_prim(env, "vector->list", vector_to_list_prim, 1, 1);
    scm_add_prim(env, "list->vector", list_to_vector_prim, 1, 1);
    scm_add_prim(env, "vector-fill!", vector_fill_prim, 2, 2);
}

scm_object* scm_make_vector(scm_object *elems[], int len)
{
    scm_object *vec = scm_malloc_object(sizeof(scm_vector));

    vec->type = scm_vector_type;
    SCM_VECTOR_ELEMS(vec) = elems;
    SCM_VECTOR_LEN(vec) = len;

    return vec;
}

scm_object* scm_list_to_vector(scm_object *list, int len)
{
    scm_object **elems = malloc(sizeof(scm_object *) * len);
    int i = 0;
    scm_list_for_each(list) {
        elems[i++] = SCM_CAR(list);
    }
    
    return scm_make_vector(elems, len);
}

static scm_object* vector_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_VECTORP(argv[0]));
}

static scm_object* make_vector_prim(int argc, scm_object *argv[])
{
    if (!is_exact_nonnegative_integer(argv[0]))
        return scm_wrong_contract("make-vector", "exact-nonnegative-integer?", 0, argc, argv);

    scm_object *obj = argc > 1 ? argv[1] : scm_make_integer(0);

    int len = SCM_INT_VAL(argv[0]);
    scm_object **elems = malloc(sizeof(scm_object *) * len);
    int i;
    for (i = 0; i < len; i++)
        elems[i] = obj;

    return scm_make_vector(elems, len);
}

static scm_object* vector_prim(int argc, scm_object *argv[])
{
    return scm_make_vector(argv, argc);
}

#define VECTOR_INDEX_CHECK(name) \
    if (!SCM_VECTORP(argv[0])) \
        return scm_wrong_contract(name, "vector?", 0, argc, argv); \
    if (!is_exact_nonnegative_integer(argv[1])) \
        return scm_wrong_contract(name, "exact-nonnegative-integer?", 1, argc, argv); \
    \
    int k = SCM_INT_VAL(argv[1]); \
    if (!(0 <= k && k < SCM_VECTOR_LEN(argv[0]))) \
        return scm_out_of_range(name, argv[0], k, k, 0);
        
static scm_object* vector_set_prim(int argc, scm_object *argv[])
{
    VECTOR_INDEX_CHECK("vector-set!");
    
    SCM_VECTOR_ELEMS(argv[0])[k] = argv[2];
    
    return scm_void;
}

static scm_object* vector_ref_prim(int argc, scm_object *argv[])
{
    VECTOR_INDEX_CHECK("vector-ref");
    
    return SCM_VECTOR_ELEMS(argv[0])[k];
}

static scm_object* vector_length_prim(int argc, scm_object *argv[])
{
    if (!SCM_VECTORP(argv[0]))
        return scm_wrong_contract("vector-length", "vector?", 0, argc, argv);
    
    return scm_make_integer(SCM_VECTOR_LEN(argv[0]));
}

static scm_object* vector_to_list_prim(int argc, scm_object *argv[])
{
    if (!SCM_VECTORP(argv[0]))
        return scm_wrong_contract("vector->list", "vector?", 0, argc, argv);
    
    return scm_build_list(SCM_VECTOR_LEN(argv[0]), SCM_VECTOR_ELEMS(argv[0]));
}

static scm_object* list_to_vector_prim(int argc, scm_object *argv[])
{
    if (!SCM_LISTP(argv[0]))
        return scm_wrong_contract("list->vector", "list?", 0, argc, argv);
    
    return scm_list_to_vector(argv[0], scm_list_length(argv[0]));
}

static scm_object* vector_fill_prim(int argc, scm_object *argv[])
{
    if (!SCM_VECTORP(argv[0]))
        return scm_wrong_contract("vector-fill", "vector?", 0, argc, argv);
    
    scm_object **elems = SCM_VECTOR_ELEMS(argv[0]);
    int len = SCM_VECTOR_LEN(argv[0]);
    scm_object* obj = argv[1];
    
    for (len--; len >= 0; len--)
        elems[len] = obj;

    return scm_void;
}