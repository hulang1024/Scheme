#include <string.h>
#include "bool.h"
#include "list.h"

void scm_init_bool(scm_env *env)
{
    scm_true->type = scm_true_type;
    scm_false->type = scm_false_type;
    scm_void->type = scm_void_type;
}

int scm_equal(scm_object *x, scm_object *y)
{
    if(!SAME_OBJ(x, y))
        return 0;
    if(!SCM_SAME_TYPE(SCM_TYPE(x), SCM_TYPE(y))) // types: true, false, null...
        return 0;
    
    switch(SCM_TYPE(x)) {
        case scm_integer_type:
            return SCM_INT_VAL(x) == SCM_INT_VAL(y);
        case scm_float_type:
            return SCM_FLOAT_VAL(x) == SCM_FLOAT_VAL(y);
        case scm_char_type:
            return SCM_CHAR_VAL(x) == SCM_CHAR_VAL(y);
        case scm_string_type:
            return strcmp(SCM_CHAR_STR_VAL(x), SCM_CHAR_STR_VAL(y)) == 0;
        case scm_symbol_type:
            return strcmp(SCM_SYMBOL_STR_VAL(x), SCM_SYMBOL_STR_VAL(y)) == 0;
        case scm_pair_type:
            // TODO: equal of pair
            return 0;
    }
    return 0;
}