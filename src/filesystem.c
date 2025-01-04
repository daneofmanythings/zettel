#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "filesystem.h"

void prep_environment() {
  setenv("XDG_ZETTEL_HOME", XDG_ZETTEL_HOME, 0);
  setenv("ZET_KNOWLEDGE", ZET_KNOWLEDGE, 0);
  setenv("ZET_PROJECTS", ZET_PROJECTS, 0);
  setenv("ZET_ARCHIVE", ZET_ARCHIVE, 0);
  setenv("ZET_JOURNAL", ZET_JOURNAL, 0);
}

// FIX: This is mega bad and broken.
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
      break; // env variable not found.
    }
    // check the new length.
    // TODO: verify this arithmetic
    if (PATH_MAX <= strlen(temp_inspect) + env_start - env_end + strlen(temp_env_var_path)) {
      // overflow
      break;
    }
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
