#include "char.h"

scm_object* scm_make_char(char val)
{
    scm_object *o = scm_malloc_object(sizeof(scm_char));
    o->type = scm_char_type;
    SCM_CHAR_VAL(o) = val;
    return o;
}
