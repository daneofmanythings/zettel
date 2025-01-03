#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/help.h"
#include "src/options.h"
#include "src/run.h"

int main(int argc, char** argv) {

  if (argc == 1) {
    printf("%s\n", general_usage_text);
    return EXIT_SUCCESS;
  }

  // dispatch
  run_parameters_t run_params = run_parameters_create(argc, argv);
  run(run_params);

  return EXIT_SUCCESS;
}
