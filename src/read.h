#ifndef SCHEME_READ_H
#define SCHEME_READ_H

#include "port.h"

void scm_init_read(scm_env *);
scm_object* scm_read(scm_object *port);

#endif //SCHEME_READ_H
