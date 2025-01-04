#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"

char** split_string_on_delimiter(const char* string, const char delimiter) {
  char* temp = trim_whitespace(string);                  // not necessary
  char** result = calloc(sizeof(char*), strlen(string)); // overestimating length
  if (result == NULL) {
    // TODO:
  }

  char* left = temp;
  char* right = temp;
  size_t i = 0;
  unsigned long len = 0;
  bool running = true;
  while (running) {
    while (*left == delimiter) {
      left += 1;
    }
    right = strchrnul(left, delimiter);
    if (*right == '\0') {
      running = false;
    }
    len = right - left;
    result[i] = malloc(sizeof(char) * (len + 1));
    if (result[i] == NULL) {
      // TODO:
    }
    memcpy(result[i], left, len);
    result[i][len] = '\0';
    i += 1;
    left = right + 1;
  }

  free(temp);
  temp = NULL;

  return result;
}

void split_string_cleanup(char** split_string) {
  for (size_t i = 0; split_string[i] != NULL; ++i) {
    free(split_string[i]);
    split_string[i] = NULL;
  }
  free(split_string);
  split_string = NULL;
}

char* trim_whitespace(const char* string) {
  char* trimmed = malloc(sizeof(char) * (strlen(string) + 1));
  if (trimmed == NULL) {
    // TODO:
  }
  while (isspace(*string)) {
    string += 1;
  }
  strcpy(trimmed, string);
  char* trimmed_end = trimmed + strlen(trimmed) - 1;
  while (isspace(*trimmed_end)) {
    trimmed_end -= 1;
  }
  *(trimmed_end + 1) = '\0';

  return trimmed;
}

size_t count_char_in_string(const char c, const char* string) {
  size_t count = 0;
  for (size_t i = 0; string[i] != '\0'; ++i) {
    if (string[i] == c) {
      count += 1;
    }
  }
  return count;
}
