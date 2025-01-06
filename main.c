#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "src/filesystem.h"
#include "src/help.h"
#include "src/options.h"
#include "src/run.h"

extern const char XDG_ZETTEL_HOME[];

int main(int argc, char** argv) {

  prep_environment();

  if (argc == 1) {
    printf("%s\n", general_usage_text);
    return EXIT_SUCCESS;
  }

  // dispatch
  run_parameters_t run_params = run_parameters_create(argc, argv);
  run(run_params);

  printf("program complete\n");
  return EXIT_SUCCESS;
}
