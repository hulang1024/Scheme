#ifndef SCHEME_ENV_H
#define SCHEME_ENV_H

#include "scm.h"

struct scm_env {
    hashtable *bindings;
    scm_env *parent;
};

scm_env* scm_basic_env();
scm_env* scm_env_new_frame(int size);
void scm_env_add_binding(scm_env *, scm_symbol *, scm_object *);
vodi scm_env_set_binding(scm_env *, scm_symbol *, scm_object *);
scm_object* scm_env_lookup(scm_env *, scm_symbol *);
void scm_add_prim(scm_env *, const char *, scm_prim, int, int);

#endif //SCHEME_ENV_H
