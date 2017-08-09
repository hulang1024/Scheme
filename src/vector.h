#ifndef SCHEME_VECTOR_H
#define SCHEME_VECTOR_H

#include "scm.h"

void scm_init_vector(scm_env *env);
scm_object* scm_make_vector(scm_object **, int);
scm_object* scm_list_to_vector(scm_object *, int);

#endif //SCHEME_VECTOR_H
