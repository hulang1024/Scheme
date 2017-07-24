#include "scm.h"

scm_object* scm_make_integer(int val)
{
    scm_object *o = scm_malloc_object(sizeof(scm_integer));
    o->type = scm_integer_type;
    SCM_INT_VAL(o) = val;
    return o;
}