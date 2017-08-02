#ifndef SCHEME_STR_H
#define SCHEME_STR_H

#include "scm.h"

void scm_init_string(scm_env *);
scm_object* scm_make_string(char *);

#endif //SCHEME_STR_H
