#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "options.h"
#include "utilities.h"

struct determine_mode_test_case {
  char* input;
  mode_e expected;
};
ParameterizedTestParameters(options, determine_mode) {
  static struct determine_mode_test_case tcs[] = {
      {"new\0",      NEW         },
      {"search\0",   SEARCH      },
      {"notamode\0", INVALID_MODE},
      {"n\0",        NEW         },
      {"s\0",        SEARCH      },
      {"no\0",       INVALID_MODE},
  };
  size_t size = sizeof(tcs) / sizeof(struct determine_mode_test_case);
  return cr_make_param_array(struct determine_mode_test_case, tcs, size);
}
ParameterizedTest(struct determine_mode_test_case* tc, options, determine_mode) {
  mode_e result = determine_mode(tc->input);
  cr_assert(eq(u8, result, tc->expected), "input='%s'\n", tc->input);
}

struct determine_template_test_case {
  char* input;
  new_template_e expected;
};
ParameterizedTestParameters(options, determine_template) {
  static struct determine_template_test_case tcs[] = {
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
  size_t size = sizeof(tcs) / sizeof(struct determine_template_test_case);
  return cr_make_param_array(struct determine_template_test_case, tcs, size);
}
ParameterizedTest(struct determine_template_test_case* tc, options, determine_template) {
  new_template_e result = determine_template(tc->input);
  cr_assert(eq(int, result, tc->expected), "input='%s'\n", tc->input);
}

struct determine_search_test_case {
  char* input;
  search_type_e expected;
};
ParameterizedTestParameters(options, determine_search) {
  static struct determine_search_test_case tcs[] = {
      {"-fk\0",   FILENAMES     },
      {"f\0",     INVALID_SEARCH},
      {"fh\0",    INVALID_SEARCH},
      {"-c\0",    CONTENTS      },
      {"-caks\0", CONTENTS      },
      {"-g\0",    INVALID_SEARCH},
      {"-t\0",    INVALID_SEARCH},
      {" \0",     INVALID_SEARCH},
  };
  size_t size = sizeof(tcs) / sizeof(struct determine_search_test_case);
  return cr_make_param_array(struct determine_search_test_case, tcs, size);
}
ParameterizedTest(struct determine_search_test_case* tc, options, determine_search) {
  search_type_e result = determine_search(tc->input);
  cr_assert(eq(int, result, tc->expected), "input='%s'\n", tc->input);
}

struct determine_title_test_case {
  int argc;
  char* argv_str;
  char* expected;
};
ParameterizedTestParameters(options, determine_title) {
  static struct determine_title_test_case tcs[] = {
      {4, "zet n -p one",             "one\0"            },
      {5, "zet n -k one two",         "one.two\0"        },
      {6, "zet n -s one two three",   "one.two.three\0"  },
      {7, "zet n -k this is a title", "this.is.a.title\0"}
  };
  size_t size = sizeof(tcs) / sizeof(struct determine_title_test_case);
  return cr_make_param_array(struct determine_title_test_case, tcs, size);
}
ParameterizedTest(struct determine_title_test_case* tc, options, determine_title) {
  char** argv = split_string_on_delimiter(tc->argv_str, ' ');
  char title[NAME_MAX] = {0};
  bool did_fail = determine_title(title, tc->argc, argv);
  cr_assert_eq(did_fail, true);
  cr_assert(eq(str, title, tc->expected), "input='%s'\n", tc->argv_str);
  split_string_cleanup(argv);
}

struct determine_locations_test_case {
  char* input;
  uint8_t expected;
};
ParameterizedTestParameters(options, determine_locations) {
  static struct determine_locations_test_case tcs[] = {
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
  size_t size = sizeof(tcs) / sizeof(struct determine_locations_test_case);
  return cr_make_param_array(struct determine_locations_test_case, tcs, size);
}
ParameterizedTest(struct determine_locations_test_case* tc, options, determine_locations) {
  uint8_t result = determine_search_locations(tc->input);
  cr_assert(eq(u8, result, tc->expected), "input='%s'", tc->input);
}

struct run_parameters_create_test_case {
  int argc;
  char* argv_str;
  mode_e mode;
  new_template_e template;
  char* title;
  search_type_e search_type;
  uint8_t locations;
}; // TODO: ADD MORE TESTS. THIS IS THE IMPORTANT ONE
ParameterizedTestParameters(options, run_parameters_create) {
  static struct run_parameters_create_test_case tcs[] = {
      {7, "zet n -k this is a test", NEW,    KNOWLEDGE,        "this.is.a.test", INVALID_SEARCH, 0 },
      {3, "zet s -fap",              SEARCH, INVALID_TEMPLATE, NULL,             FILENAMES,      10},
  };
  size_t size = sizeof(tcs) / sizeof(struct run_parameters_create_test_case);
  return cr_make_param_array(struct run_parameters_create_test_case, tcs, size);
}
ParameterizedTest(struct run_parameters_create_test_case* tc, options, run_parameters_create) {
  char** argv = split_string_on_delimiter(tc->argv_str, ' ');
  run_parameters_t result = run_parameters_create(tc->argc, argv);
  cr_assert(eq(int, tc->mode, result.mode), "input='%s'\n", tc->argv_str);
  if (tc->mode == NEW) {
    cr_assert(eq(int, tc->template, result.params.new.template), "input='%s'\n", tc->argv_str);
    cr_assert(eq(str, tc->title, result.params.new.title), "input='%s'\n", tc->argv_str);
  }
  if (tc->mode == SEARCH) {
    cr_assert(eq(int, tc->search_type, result.params.search.type), "input='%s'\n", tc->argv_str);
    cr_assert(eq(u8, tc->locations, result.params.search.locations), "input='%s'\n", tc->argv_str);
  }
  split_string_cleanup(argv);
}
