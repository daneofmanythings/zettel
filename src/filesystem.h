#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <limits.h>
#include <stdbool.h>

typedef struct filepath_t filepath_t;

void prep_environment();
void expand_path_env_variables(char dest[PATH_MAX], const char* path);
void write_new_file(const char* path, const char* body);
void write_to_scratch(const char* path, const char* body);
void mkdir_from_absolute_filepath(const char* filepath);

#endif // !DEBUG
