#ifndef SCHEME_NUMBER_H
#define SCHEME_NUMBER_H

#include "scm.h"

#define is_exact_nonnegative_integer(o) (SCM_INTEGERP(o) && SCM_INT_VAL(o) >= 0)

void scm_init_number(scm_env *);
scm_object* scm_make_integer(long);
scm_object* scm_make_float(double);

#endif //SCHEME_NUMBER_H
