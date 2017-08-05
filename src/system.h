#ifndef SCHEME_SYSTEM_H
#define SCHEME_SYSTEM_H

#include "scm.h"

extern char *scm_g_repl_prompt;

void scm_init_system(scm_env *);
int scm_load_file(const char *);

#endif //SCHEME_SYSTEM_H
