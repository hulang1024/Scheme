
#ifndef SCM_H
#define SCM_H

typedef short ScmType;

enum {
    scm_integer_type,
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
    ScmType type;
} ScmObject;

typedef struct {
    ScmObject o;
    int int_val;
} ScmInteger;

typedef struct {
    ScmObject o;
    short val;
} ScmBool;

typedef struct {
    ScmObject o;
    char char_val;
} ScmChar;

typedef struct {
    ScmObject o;
    char *char_str_val;
} ScmString;

typedef struct {
    ScmObject o;
    char *s;
} ScmSymbol;

typedef struct {
    ScmObject o;
    ScmObject *car;
    ScmObject *cdr;
} ScmPair;

typedef ScmObject *(ScmPrim)(int argc, ScmObject *argv[]);

typedef struct {
    ScmObject o;
    const char *name;
    ScmPrim prim;
    int minArity;
    int maxArity;
} ScmPrimitiveProc;

typedef struct {
    ScmObject o;
    char *name;
    ScmEnv *env; // for closure
    ScmObject *body;
    ScmObject *params;
    int minArity;
    int maxArity;
} ScmCompoundProc;


/* param o: scheme object pointer */
#define SCM_TYPE(o) (((ScmObject*)(o))->type)

/* param a/b: as scheme type */
#define SCM_SAME_TYPE(a, b) ((ScmType)a == (ScmType)b)

#define SCM_INTEGERP(o) (SCM_TYPE(o) == scm_integer_type)
#define SCM_BOOLP(o) (SCM_TYPE(o) == scm_bool_type)
#define SCM_CHARP(o) (SCM_TYPE(o) == scm_char_type)
#define SCM_STRINGP(o) (SCM_TYPE(o) == scm_string_type)
#define SCM_SYMBOLP(o) (SCM_TYPE(o) == scm_symbol_type)
#define SCM_PAIRP(o) (SCM_TYPE(o) == scm_pair_type)
#define SCM_PROCEDUREP(o) ((SCM_TYPE(o) == scm_primitive_type) || (SCM_TYPE(o) == scm_compound_type))
#define SCM_NAMESPACEP(o) (SCM_TYPE(o) == scm_namespace_type)
#define SCM_VOIDP(o) (SCM_TYPE(o) == scm_void_type)

#endif //SCM_H
