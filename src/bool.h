#ifndef SCHEME_BOOL_H
#define SCHEME_BOOL_H

#include "scm.h"

scm_object scm_true[1];
scm_object scm_false[1];
scm_object scm_void[1];

void scm_init_bool(scm_env *env);
int scm_equal(scm_object *, scm_object *);

#endif //SCHEME_BOOL_H
