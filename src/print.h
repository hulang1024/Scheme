#ifndef SCHEME_PRINT_H
#define SCHEME_PRINT_H

#include <stdio.h>
#include "scm.h"
#include "port.h"

void scm_init_print(scm_env *);
void scm_write(scm_object *port, scm_object *);
void scm_display(scm_object *port, scm_object *);

#endif //SCHEME_PRINT_H
