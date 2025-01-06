#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "filesystem.h"

extern const char XDG_ZETTEL_HOME[];
extern const char ZET_KNOWLEDGE[];
extern const char ZET_PROJECTS[];
extern const char ZET_SCRATCH[];
extern const char ZET_ARCHIVE[];
extern const char ZET_JOURNAL[];

void prep_environment() {
  setenv("XDG_ZETTEL_HOME", XDG_ZETTEL_HOME, 0);
  setenv("ZET_KNOWLEDGE", ZET_KNOWLEDGE, 0);
  setenv("ZET_PROJECTS", ZET_PROJECTS, 0);
  setenv("ZET_SCRATCH", ZET_SCRATCH, 0);
  setenv("ZET_ARCHIVE", ZET_ARCHIVE, 0);
  setenv("ZET_JOURNAL", ZET_JOURNAL, 0);
}

// FIX: This is mega bad and ugly.
void expand_path_env_variables(char dest[PATH_MAX], const char* path) {
  // possibly declare temp variables;
  char* env_start = NULL;
  char* env_end = NULL;
  char temp_inspect[PATH_MAX] = {0};
  char temp_copy[PATH_MAX] = {0};
  char temp_env_var_name[PATH_MAX] = {0};
  char* temp_env_var_path = NULL;
  if (strlen(path) >= PATH_MAX) {
    return; // leaving 1 spot for '\0' cuz why are your paths so long?
  }
  strcpy(temp_inspect, path);
  // loop until all environment variables are expanded;
  while (true) {
    // find start of environment variable '$';
    env_start = strchrnul(temp_inspect, '$');
    // if not found, exit loop;
    if (*env_start == '\0') { // Assumes 'path' is null terminated
      break;
    }
    // find end of env variable '/' or '\0';
    env_end = env_start;
    env_end = strchrnul(env_end, '/');
    *env_start = '\0'; // setting up the copying
    *env_end = '\0';
    strcpy(temp_env_var_name, env_start + 1);
    // getenv the value
    temp_env_var_path = getenv(temp_env_var_name);
    if (temp_env_var_path == NULL) {
      fprintf(stderr, "ERROR | filesystem::expand_path_env_variables | environment variable='%s' not found\n",
              temp_env_var_name);
      break; // env variable not found.
    }
    //
    // TODO: CHECK NEW PATH LENGTH FOR OVERFLOW BEFORE CONCATENATING
    //
    // do some strcopying into a temp buffer
    char* tmp = temp_copy;
    tmp = stpcpy(tmp, temp_inspect);
    tmp = stpcpy(tmp, temp_env_var_path);
    tmp = stpcpy(tmp, "/");
    tmp = stpcpy(tmp, env_end + 1);
    // transfer to the buffer we inspect
    strcpy(temp_inspect, temp_copy);
  }
  // copy into dest;
  strcpy(dest, temp_inspect);
  if (dest[strlen(dest) - 1] == '/') {
    dest[strlen(dest) - 1] = '\0';
  }
}

void write_new_file(const char* path, const char* body) {
  mkdir_from_absolute_filepath(path);
  FILE* f = fopen(path, "w");
  if (f == NULL) {
    fprintf(stderr, "ERROR | filesystem::write_new_file | could not open file: %s\n", path);
    exit(1);
  }
  fwrite(body, sizeof(char), strlen(body), f);
  fclose(f);
}

void mkdir_from_absolute_filepath(const char* filepath) {
  struct stat st = {0};
  if (stat(filepath, &st) == 0) { // we hit part of the path that exists
    return;
  }

  char temp[PATH_MAX];
  strcpy(temp, filepath);
  char* slash = strrchr(temp, '/');
  if (slash == NULL) {
    // TODO: invaid path. DO ERROR
    fprintf(stderr, "ERROR | filesystem::mkdir_r | invalid path: %s\n", temp);
    exit(1);
  }
  if (temp == slash) { // we hit the root '/'. above existence check should be sufficient
    return;
  }
  *slash = '\0';
  mkdir_from_absolute_filepath(temp);
  mkdir(temp, 0777);
}

void write_to_scratch(const char* path, const char* body) {
  mkdir_from_absolute_filepath(path);
  FILE* f = fopen(path, "a");
  if (f == NULL) {
    fprintf(stderr, "ERROR | filesystem::write_to_scratch | could not write to file='%s'\n", path);
    exit(1);
  }
  const char* padding = "\n\n";
  fwrite(body, sizeof(char), strlen(body), f);
  fwrite(padding, sizeof(char), strlen(padding), f);
}
