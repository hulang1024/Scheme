#include "scm.h"

scm_object* scm_eval(scm_object *exp, scm_env *env)
{
    switch(SCM_TYPE(exp)) {
        case scm_integer_type:
        case scm_float_type:
        case scm_char_type:
        case scm_string_type:
            return exp;
        case scm_symbol_type:
            break;
        case scm_void_type:
            break;
        default:
            if(SCM_NULLP(exp))
                return exp;
            else if(SCM_BOOLP(exp))
                return exp;
    }
}