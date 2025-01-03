#include <stdio.h>

#include "run.h"

extern const char* DEFAULT_XDG_DIRECTORY;

void search(search_type_e type, uint8_t location_mask);
void new(new_template_e template, char* title);
void run(run_parameters_t rp) {
  switch (rp.mode) {
  case NEW:
    new (rp.parameters.new.template, rp.parameters.new.title);
    break;
  case SEARCH:
    search(rp.parameters.search.type, rp.parameters.search.locations);
    break;
  default:
    break;
  }
}

void new(new_template_e template, char* title) {
  //
  printf("new called with template=%d, title='%s'\n", template, title);
}

void search(search_type_e type, uint8_t location_mask) {
  printf("search called with mode=%d and mask=%d\n", type, location_mask);
}
