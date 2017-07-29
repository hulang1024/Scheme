#include "char.h"

scm_object* scm_make_string(char *str)
{
    scm_object *o = scm_malloc_object(sizeof(scm_string));
    o->type = scm_string_type;
    SCM_CHAR_STR_VAL(o) = str;
    return o;
}
