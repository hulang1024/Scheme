#ifndef SCHEME_BOOL_H
#define SCHEME_BOOL_H

#include "scm.h"

#define SCM_BOOL(b) ((b) ? scm_true : scm_false)

extern scm_object scm_true[];
extern scm_object scm_false[];
extern scm_object scm_void[];

int scm_eq(scm_object *, scm_object *);
int scm_eqv(scm_object *, scm_object *);
int scm_equal(scm_object *, scm_object *);

void scm_init_bool(scm_env *env);

#endif //SCHEME_BOOL_H
