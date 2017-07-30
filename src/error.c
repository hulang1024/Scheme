#include <stdio.h>
#include "error.h"

int error_no = 0;

void scm_error(const char *t, const char *info)
{
    printf("%s: %s\n", t, info);
    error_no = 1;
}
