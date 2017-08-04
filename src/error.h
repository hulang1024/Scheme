#ifndef SCHEME_ERROR_H
#define SCHEME_ERROR_H

#include "scm.h"

void scm_print_error(const char *info);

scm_object* scm_wrong_contract(const char *, const char *, int, int, scm_object *[]);
scm_object* scm_mismatch_arity(scm_object *, int, int, int, int, scm_object *[]);
scm_object* scm_undefined_identifier(scm_symbol *);
scm_object* scm_out_of_range(const char *, scm_object *, int, int, int);

void scm_throw_error();

#endif //SCHEME_ERROR_H
