#include <criterion/criterion.h>
#include <criterion/internal/new_asserts.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>
#include <stddef.h>

#include "utilities.h"

struct count_char_in_string_test_case {
  char c;
  char* string;
  size_t expected;
};
ParameterizedTestParameters(utilities, count_char_in_string) {
  static struct count_char_in_string_test_case tcs[] = {
      {' ', "this is a test", 3},
      {'t', "this is a test", 3},
      {'a', "this is a test", 1},
      {'m', "this is a test", 0},
  };
  size_t size = sizeof(tcs) / sizeof(struct count_char_in_string_test_case);
  return cr_make_param_array(struct count_char_in_string_test_case, tcs, size);
}
ParameterizedTest(struct count_char_in_string_test_case* tc, utilities, count_char_in_string) {
  size_t result = count_char_in_string(tc->c, tc->string);
  cr_assert(eq(sz, tc->expected, result), "c='%s', string='%s'\n", &tc->c, tc->string);
}

struct trim_whitespace_test_case {
  char* string;
  char* expected;
};
ParameterizedTestParameters(utilities, trim_whitespace) {
  static struct trim_whitespace_test_case tcs[] = {
      {"This is a test.",                   "This is a test."     },
      {"This is a test     .",              "This is a test     ."},
      {"        This is a test.",           "This is a test."     },
      {"This is a test.        ",           "This is a test."     },
      {"\r\r\r\rThis is a test.\n\t\r    ", "This is a test."     },
  };
  size_t size = sizeof(tcs) / sizeof(struct trim_whitespace_test_case);
  return cr_make_param_array(struct trim_whitespace_test_case, tcs, size);
}
ParameterizedTest(struct trim_whitespace_test_case* tc, utilities, trim_whitespace) {
  char* result = trim_whitespace(tc->string);
  cr_assert(eq(str, tc->expected, result), "string='%s'\n", tc->string);
}

struct string_split_on_delimiter_test_case {
  char* string;
  char delimiter;
  char* expected[10];
};
ParameterizedTestParameters(utilities, split_string_on_delimiter) {
  static struct string_split_on_delimiter_test_case tcs[] = {
      {"This is a test.", ' ', {"This", "is", "a", "test."}},
      {"This is a test.", 'i', {"Th", "s ", "s a test."}   },
      {"iiiiiiiiii",      'i', {""}                        },
  };
  size_t size = sizeof(tcs) / sizeof(struct string_split_on_delimiter_test_case);
  return cr_make_param_array(struct string_split_on_delimiter_test_case, tcs, size);
}
ParameterizedTest(struct string_split_on_delimiter_test_case* tc, utilities, split_string_on_delimiter) {
  char** result = split_string_on_delimiter(tc->string, tc->delimiter);
  for (size_t i = 0; result[i] != NULL; ++i) {
    cr_assert(eq(str, tc->expected[i], result[i]), "string='%s'\n", tc->string);
  };
  split_string_cleanup(result);
}
