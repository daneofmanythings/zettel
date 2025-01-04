#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/filesystem.h"
#include "src/help.h"
#include "src/options.h"
#include "src/run.h"

int main(int argc, char** argv) {

  prep_environment();

  // FILE* f = fopen("/home/dane/projects/zettel/test.md", "w");
  // const char* data = "This is a journal entry\n";
  // fwrite(data, sizeof(char), strlen(data), f);
  // fclose(f);

  // if (argc == 1) {
  //   printf("%s\n", general_usage_text);
  //   return EXIT_SUCCESS;
  // }

  char* test = malloc(sizeof(char) * 20);
  strcpy(test, "this is a test\0");
  size_t len = 5;
  char** many_test = calloc(sizeof(char*), len);
  char* left = test;
  char* right = left;
  size_t i;
  bool running = true;
  while (running) {
    right = strchrnul(left, ' ');
    if (*right == '\0') {
      running = false;
    }
    *right = '\0';
    many_test[i] = malloc(sizeof(char) * 10);
    strcpy(many_test[i], left);
    i += 1;
    left = right + 1;
  }
  many_test[i] = NULL;
  for (i = 0; many_test[i] != NULL; ++i) {
    printf("%s\n", many_test[i]);
  }

  // dispatch
  // run_parameters_t run_params = run_parameters_create(argc, argv);
  // run(run_params);

  return EXIT_SUCCESS;
}
