#ifndef SCHEME_ENV_H
#define SCHEME_ENV_H

#include "scm.h"

typedef struct _scm_env scm_env;
typedef struct _scm_env scm_env_entry;

// TODO: high-performance
struct _scm_env {
    scm_symbol *id;
    scm_object *val;
    struct _scm_env *rest;
};

scm_env* scm_basic_env();
void scm_env_add_binding(scm_env *, scm_symbol *, scm_object *);
scm_env_entry* scm_env_lookup(scm_env *, scm_symbol *);
void scm_add_prim(scm_env *, const char *, scm_prim, int, int);

#endif //SCHEME_ENV_H
