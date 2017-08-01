#ifndef SCHEME_ERROR_H
#define SCHEME_ERROR_H

#include "scm.h"

void scm_print_error(const char *info);

/* print contract violation error */
scm_object* scm_wrong_contract(const char *, const char *, int, int, scm_object *[]);
/* print arity mismatch error */
scm_object* scm_unmatched_arity(const char *, int, const char *, int, scm_object *[]);
/* print identifier undefined error */
scm_object* scm_undefined_identifier(scm_symbol *);

void scm_throw_error();

#endif //SCHEME_ERROR_H
