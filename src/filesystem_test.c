#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>
#include <linux/limits.h>
#include <stdlib.h>

#include "filesystem.h"

Test(filesystem, expand_path_env_variables) {
  setenv("HOME_TEST", "/home/user", 1);
  setenv("CONFIG_TEST", "$HOME/.config", 1);
  static struct params {
    char* input;
    char* expected;
  } params[] = {
      {"/home/user",                "/home/user"                },
      {"$HOME",                     "/home/dane"                },
      {"$HOME_TEST/stuff",          "/home/user/stuff"          },
      {"$HOME_TEST/stuff/file.txt", "/home/user/stuff/file.txt" },
      {"$CONFIG_TEST/testing",      "/home/user/.config/testing"},
  };
  char result[PATH_MAX] = {0};
  for (size_t i = 0; i < 3; ++i) {
    expand_path_env_variables(result, params[i].input);
    cr_assert(eq(str, params[i].expected, result), "input='%s'", params[i].input);
  }
}
