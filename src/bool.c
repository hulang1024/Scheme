#include <string.h>
#include "scm.h"

/* scm_true & scm_false are pointers */
scm_object scm_true[1];
scm_object scm_false[1];

int scm_equal(scm_object *x, scm_object *y)
{
    if(!SAME_OBJ(x, y))
        return 0;
    if(!SCM_SAME_TYPE(SCM_TYPE(x), SCM_TYPE(y)))
        return 0;
    
    switch(SCM_TYPE(x)) {
        case scm_integer_type:
            return SCM_INT_VAL(x) == SCM_INT_VAL(y);
        case scm_float_type:
            return SCM_FLOAT_VAL(x) == SCM_FLOAT_VAL(y);
        case scm_char_type:
            return SCM_CHAR_VAL(x) == SCM_CHAR_VAL(y);
            break;
        case scm_string_type:
            return strcmp(SCM_CHAR_STR_VAL(x), SCM_CHAR_STR_VAL(y)) == 0;
        case scm_symbol_type:
            return strcmp(SCM_SYMBOL_STR_VAL(x), SCM_SYMBOL_STR_VAL(y)) == 0;
        case scm_pair_type:
            // TODO: equal of pair
            return 0;
        default:
            if(SCM_NULLP(x))
                return 1;
            else if(SCM_BOOLP(x))
                return 1;
    }
    return 0;
}