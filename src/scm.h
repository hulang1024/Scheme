
#ifndef SCM_H
#define SCM_H

typedef short scm_type;

enum {
    scm_integer_type,
    scm_float_type,
    scm_bool_type,
    scm_char_type,
    scm_string_type,
    scm_symbol_type,
    scm_pair_type,
    
    scm_primitive_type,
    scm_compound_type,
    
    scm_namespace_type,
    scm_void_type
};

typedef struct {
    /* 以一个类型标识的字段开始的都是Scheme对象 */
    scm_type type;
} scm_object;

typedef struct {
    scm_object o;
    int int_val;
} scm_integer;

typedef struct {
    scm_object o;
    float float_val;
} scm_float;

typedef struct {
    scm_object o;
    short val;
} scm_bool;

typedef struct {
    scm_object o;
    char char_val;
} scm_char;

typedef struct {
    scm_object o;
    char *char_str_val;
} scm_string;

typedef struct {
    scm_object o;
    char *s;
} scm_symbol;

typedef struct {
    scm_object o;
    scm_object *car;
    scm_object *cdr;
} scm_pair;

typedef scm_object *(scm_prim)(int argc, scm_object *argv[]);

typedef struct {
    scm_object o;
    const char *name;
    scm_prim prim;
    int min_arity;
    int max_arity;
} scm_primitive_proc;

typedef struct {
    scm_object o;
    char *name;
    scm_env *env; // for closure
    scm_object *body;
    scm_object *params;
    int min_arity;
    int max_arity;
} scm_compound_proc;


/* param o: scheme object pointer */
#define SCM_TYPE(o) (((scm_object*)(o))->type)

/* param a/b: as scheme type */
#define SCM_SAME_TYPE(a, b) ((scm_type)a == (scm_type)b)

#define SCM_INTEGERP(o) (SCM_TYPE(o) == scm_integer_type)
#define SCM_FLOATP(o) (SCM_TYPE(o) == scm_float_type)
#define SCM_BOOLP(o) (SCM_TYPE(o) == scm_bool_type)
#define SCM_CHARP(o) (SCM_TYPE(o) == scm_char_type)
#define SCM_STRINGP(o) (SCM_TYPE(o) == scm_string_type)
#define SCM_SYMBOLP(o) (SCM_TYPE(o) == scm_symbol_type)
#define SCM_PAIRP(o) (SCM_TYPE(o) == scm_pair_type)
#define SCM_PROCEDUREP(o) ((SCM_TYPE(o) == scm_primitive_type) || (SCM_TYPE(o) == scm_compound_type))
#define SCM_NAMESPACEP(o) (SCM_TYPE(o) == scm_namespace_type)
#define SCM_VOIDP(o) (SCM_TYPE(o) == scm_void_type)

#endif //SCM_H
