#ifndef SCM_H
#define SCM_H

//#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum {
    scm_integer_type = 1,
    scm_float_type = 2,

    /* bool types */
    scm_true_type,
    scm_false_type,

    scm_char_type,
    scm_string_type,

    scm_symbol_type,
    
    scm_pair_type,
    scm_null_type,
    
    scm_primitive_type,
    scm_compound_type,

    /* port types */
    scm_input_port_type,
    scm_output_port_type,

    scm_namespace_type,
    scm_void_type
};

typedef short scm_type;

typedef struct {
    /* 以一个类型标识的字段开始的都是Scheme对象 */
    scm_type type;
} scm_object;

typedef struct {
    scm_object o;
    long int_val;
} scm_integer;

typedef struct {
    scm_object o;
    double float_val;
} scm_float;

typedef struct {
    scm_object o;
    char char_val;
} scm_char;

typedef struct {
    scm_object o;
    int len;
    char *byte_str_val;
} scm_string;

typedef struct {
    scm_object o;
    const char *s;
} scm_symbol;

typedef struct {
    scm_object o;
    short flags;
    scm_object *car;
    scm_object *cdr;
} scm_pair;

/* Scheme基本过程C函数类型 */
typedef scm_object* (* scm_prim)(int argc, scm_object *argv[]);

typedef struct {
    scm_object o;
    const char *name;
    scm_prim prim;
    int min_arity;
    int max_arity;
} scm_primitive_proc;

typedef struct _scm_env scm_env;

typedef struct {
    scm_object o;
    const char *name;
    scm_env *env; // for closure
    scm_object *body;
    scm_object **params;
    int params_len;
    int min_arity;
    int max_arity;
} scm_compound_proc;

#define SAME_PTR(a, b) ((a) == (b))
#define NOT_SAME_PTR(a, b) ((a) != (b))

#define SAME_OBJ(a, b) SAME_PTR(a, b)
#define NOT_SAME_OBJ(a, b) NOT_SAME_PTR(a, b)

/* param o: scheme object pointer */
#define SCM_TYPE(o) (((scm_object *)(o))->type)

/* param a/b: as scheme type */
#define SCM_SAME_TYPE(a, b) ((scm_type)(a) == (scm_type)(b))

#define SCM_INTEGERP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_integer_type)
#define SCM_FLOATP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_float_type)

#define SCM_FALSEP(o) SAME_OBJ(o, scm_false)
#define SCM_TRUEP(o) (!SCM_FALSEP(o))
#define SCM_BOOLP(o) (SAME_OBJ(o, scm_true) || SAME_OBJ(o, scm_false))

#define SCM_CHARP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_char_type)
#define SCM_STRINGP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_string_type)
#define SCM_SYMBOLP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_symbol_type)

#define SCM_PAIRP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_pair_type)
#define SCM_LISTP(o) scm_is_list(o)
#define SCM_NULLP(o) SAME_OBJ(o, scm_null)
#define SCM_VOIDP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_void_type)

#define SCM_PRIMPROCP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_primitive_type)
#define SCM_COMPROCP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_compound_type)
#define SCM_PROCEDUREP(o) (SCM_PRIMPROCP(o) || SCM_COMPROCP(o))
#define SCM_NAMESPACEP(o) SCM_SAME_TYPE(SCM_TYPE(o), scm_namespace_type)

/* accessor macros */
#define SCM_INT_VAL(o) (((scm_integer *)(o))->int_val)
#define SCM_FLOAT_VAL(o) (((scm_float *)(o))->float_val)
#define SCM_CHAR_VAL(o) (((scm_char *)(o))->char_val)
#define SCM_CHAR_STR_VAL(o) (((scm_string *)(o))->byte_str_val)
#define SCM_SYMBOL_STR_VAL(o) (((scm_symbol *)(o))->s)
#define SCM_STR_LEN(o) (((scm_string *)(o))->len)

#define SCM_CAR(o) (((scm_pair *)(o))->car)
#define SCM_CDR(o) (((scm_pair *)(o))->cdr)

#define SCM_PAIR_FLAGS(o) (((scm_pair *)o)->flags)
#define SCM_PAIR_FLAGS_INIT 0
/* proper list flags，加快判断速度 */
#define SCM_PAIR_IS_LIST 1
#define SCM_PAIR_IS_NON_LIST 2


/*                      memory management macros                          */
/* Allocation */
#define scm_malloc_object(size) ((scm_object *)malloc(size))

#endif //SCM_H
