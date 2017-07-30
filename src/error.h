#ifndef SCHEME_ERROR_H
#define SCHEME_ERROR_H

#include "scm.h"

void scm_print_error(const char *info);
scm_object* scm_wrong_contract(const char *, const char *, int, int, scm_object *[]);

void scm_throw_error();

#endif //SCHEME_ERROR_H
