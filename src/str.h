#ifndef SCHEME_STR_H
#define SCHEME_STR_H

#include "scm.h"

extern scm_object scm_empty_string[];

void scm_init_string(scm_env *);
scm_object* scm_make_string(char *, int);

#endif //SCHEME_STR_H
