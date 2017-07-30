#ifndef SCHEME_NUMBER_H
#define SCHEME_NUMBER_H

#include "scm.h"

scm_object scm_int_zero[0];

void scm_init_number(scm_env *);
scm_object* scm_make_integer(long);
scm_object* scm_make_float(double);

#endif //SCHEME_NUMBER_H
