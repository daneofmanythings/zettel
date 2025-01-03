#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include <limits.h>
#include <stdio.h>

#include "options.h"

struct determine_mode_params {
  char* input;
  mode_e expected;
};
ParameterizedTestParameters(options, determine_mode) {
  static struct determine_mode_params params[] = {
      {"new\0",      NEW         },
      {"search\0",   SEARCH      },
      {"notamode\0", INVALID_MODE},
      {"n\0",        NEW         },
      {"s\0",        SEARCH      },
      {"no\0",       INVALID_MODE},
  };
  size_t size = sizeof(params) / sizeof(struct determine_mode_params);
  return cr_make_param_array(struct determine_mode_params, params, size);
}
ParameterizedTest(struct determine_mode_params* param, options, determine_mode) {
  mode_e result = determine_mode(param->input);
  cr_assert_eq(result, param->expected);
}

struct determine_template_params {
  char* input;
  new_template_e expected;
};
ParameterizedTestParameters(options, determine_template) {
  static struct determine_template_params params[] = {
      {"-k\0",    KNOWLEDGE       },
      {"k\0",     INVALID_TEMPLATE},
      {"kh\0",    INVALID_TEMPLATE},
      {"-pppp\0", PROJECT         },
      {"-ss\0",   SCRATCH         },
      {"-g\0",    INVALID_TEMPLATE},
      {"-f\0",    INVALID_TEMPLATE},
      {"-t\0",    INVALID_TEMPLATE},
      {"-b\0",    INVALID_TEMPLATE},
      {" \0",     INVALID_TEMPLATE},
  };
  size_t size = sizeof(params) / sizeof(struct determine_template_params);
  return cr_make_param_array(struct determine_template_params, params, size);
}
ParameterizedTest(struct determine_template_params* param, options, determine_template) {
  new_template_e result = determine_template(param->input);
  if (result != param->expected) {
    printf("failed, input='%s', output=%d\n", param->input, result);
  }
  cr_assert_eq(result, param->expected);
}

struct determine_search_params {
  char* input;
  search_type_e expected;
};
ParameterizedTestParameters(options, determine_search) {
  static struct determine_search_params params[] = {
      {"-fk\0",   FILENAMES     },
      {"f\0",     INVALID_SEARCH},
      {"fh\0",    INVALID_SEARCH},
      {"-c\0",    CONTENTS      },
      {"-caks\0", CONTENTS      },
      {"-g\0",    INVALID_SEARCH},
      {"-t\0",    INVALID_SEARCH},
      {" \0",     INVALID_SEARCH},
  };
  size_t size = sizeof(params) / sizeof(struct determine_search_params);
  return cr_make_param_array(struct determine_search_params, params, size);
}
ParameterizedTest(struct determine_search_params* param, options, determine_search) {
  search_type_e result = determine_search(param->input);
  if (result != param->expected) {
    printf("failed, input='%s', output=%d\n", param->input, result);
  }
  cr_assert_eq(result, param->expected);
}

struct determine_title_params {
  int argc;
  char** argv;
  char* expected;
};
ParameterizedTestParameters(options, determine_title) {
  static char* argv0[] = {"zet", "s", "-f", "one"};
  static char* argv1[] = {"zet", "s", "-f", "one", "two"};
  static char* argv2[] = {"zet", "s", "-f", "one", "two", "three"};
  static struct determine_title_params params[] = {
      {4, argv0, "one\0"          },
      {5, argv1, "one.two\0"      },
      {6, argv2, "one.two.three\0"},
  };
  size_t size = sizeof(params) / sizeof(struct determine_title_params);
  return cr_make_param_array(struct determine_title_params, params, size);
}
ParameterizedTest(struct determine_title_params* param, options, determine_title) {
  char title[NAME_MAX] = {0};
  bool did_fail = determine_title(title, param->argc, param->argv);
  if (strcmp(title, param->expected) != 0) {
    printf("failed, argc='%d', argv=[ ", param->argc);
    for (size_t i = 0; i < param->argc; i++) {
      printf("%s ", param->argv[i]);
    }
    printf("]\n");
    printf("expected=%s, result=%s\n", param->expected, title);
  }
  cr_assert_eq(did_fail, true);
  cr_assert_eq(strcmp(title, param->expected), 0);
}

struct determine_locations_params {
  char* input;
  uint8_t expected;
};
ParameterizedTestParameters(options, determine_locations) {
  static struct determine_locations_params params[] = {
      {"-k",      1 },
      {"-p",      2 },
      {"-s",      4 },
      {"-a",      8 },
      {"-j",      16},
      {"-saj",    28},
      {"-pp",     2 },
      {"-psjka",  31},
      {"-fpsjka", 31},
      {"-",       0 },
      {"",        0 },
      {"-mg",     0 },
  };
  size_t size = sizeof(params) / sizeof(struct determine_locations_params);
  return cr_make_param_array(struct determine_locations_params, params, size);
}
ParameterizedTest(struct determine_locations_params* param, options, determine_locations) {
  uint8_t result = determine_search_locations(param->input);
  if (result != param->expected) {
    printf("failed, input='%s', output=%d\n", param->input, result);
  }
  cr_assert_eq(result, param->expected);
}

// struct run_parameters_create_params {
//   int argc;
//   char** argv;
//   run_parameters_t expected;
// };
// ParameterizedTestParameters(options, run_parameters_create) {
//   // clang-format off
//   static char* argv0[] = {"zet", "s", "-fpa",};
//   static run_parameters_t expected0 = {
//     .mode = SEARCH,
//     .parameters.search = {
//       .type = FILENAMES,
//       .locations = 2,
//     },
//   };
//   static char* argv1[] = {"zet", "s", "-f", "one", "two"};
//   static char* argv2[] = {"zet", "s", "-f", "one", "two", "three"};
//   // clang-format on
//   static struct run_parameters_create_params params[] = {
//       {4, argv0, expected0},
//   };
//   size_t size = sizeof(params) / sizeof(struct run_parameters_create_params);
//   return cr_make_param_array(struct run_parameters_create_params, params, size);
// }
// ParameterizedTest(struct run_parameters_create_params* param, options, run_parameters_create) {
//   run_parameters_t result = run_parameters_create(param->argc, param->argv);
//   cr_assert_eq(result, param->expected);
// }
