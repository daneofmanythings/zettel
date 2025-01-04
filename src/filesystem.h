#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <limits.h>
#include <stdbool.h>

typedef struct filepath_t filepath_t;

void prep_environment();
void expand_path_env_variables(char dest[PATH_MAX], const char* path);

#endif // !DEBUG
