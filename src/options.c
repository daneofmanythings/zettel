#include "options.h"
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mode_defaults_index determine_mode(char* mode) {
  size_t i;
  bool is_name_match;
  bool is_short_name_match;
  for (i = 0; strcmp(MODE_DEFAULTS[i].name, "\0") != 0; i++) {
    is_name_match = strcmp(mode, MODE_DEFAULTS[i].name) == 0;
    is_short_name_match = strcmp(mode, MODE_DEFAULTS[i].short_name) == 0;
    if (is_name_match || is_short_name_match) {
      break;
    }
  }
  return i;
}

template_defaults_index determine_template(char* template_args) {
  size_t len = strlen(template_args);
  if (len < 2) {
    return INVALID_TEMPLATE;
  }

  if (template_args[0] != '-') {
    // TODO: implement whole names.
    return INVALID_TEMPLATE;
  }
  size_t i;
  char* t = &template_args[1];
  for (i = 0; strcmp(TEMPLATE_DEFAULTS[i].short_name, "\0") != 0; i++) {
    if (strncmp(t, TEMPLATE_DEFAULTS[i].short_name, 1) == 0) {
      break;
    }
  }
  return i;
}

void determine_content(template_defaults_index template, char content[NAME_MAX], int argc, char** argv) {
  char filler;
  if (template == SCRATCH) {
    filler = ' ';
  } else {
    filler = '.';
  }

  if (argc < 4) {
    return;
  }
  // TODO: bounds check the length of the title so it doesn't overflow.
  char* content_ptr = content;
  for (size_t i = 3; i < argc; i++) {
    content_ptr = stpcpy(content_ptr, argv[i]);
    *content_ptr++ = filler;
  }
  *(--content_ptr) = '\0';
  return;
}

new_mode_params_t determine_new_params(int argc, char** argv) {
  new_mode_params_t params = {0};
  // TODO: check bounding
  params.template = determine_template(argv[2]);
  determine_content(params.template, params.content, argc, argv);
  return params;
}

search_type_defaults_index determine_search_type(char* search_args) {
  size_t len = strlen(search_args);
  if (len < 2) {
    return INVALID_SEARCH;
  }

  if (search_args[0] != '-') {
    // TODO: implement whole names.
    return INVALID_SEARCH;
  }
  size_t i;
  for (i = 0; strcmp(SEARCH_TYPE_DEFAULTS[i].name, "\0") != 0; ++i) {
    if (strncmp(&search_args[1], SEARCH_TYPE_DEFAULTS[i].short_name, 1) == 0) {
      break;
    }
  }
  return i;
}

uint8_t determine_search_locations(char* search_args) {
  // TODO: check for '-'

  // TODO: add full name functionality?

  uint8_t location_mask = 0;
  for (size_t i = 0; strcmp(SEARCH_LOCATION_DEFAULTS[i].name, "\0") != 0; ++i) {
    if (strchr(search_args, *SEARCH_LOCATION_DEFAULTS[i].short_name)) {
      location_mask += pow(2, i);
    }
  }
  return location_mask;
}

search_mode_params_t determine_search_params(int argc, char** argv) {
  search_mode_params_t params = {0};
  params.type = determine_search_type(argv[2]);
  params.locations = determine_search_locations(argv[2]);
  return params;
}

run_parameters_t run_parameters_create(int argc, char** argv) {
  run_parameters_t run_parameters = {0};
  run_parameters.mode = determine_mode(argv[1]);
  // dispatch on mode
  switch (run_parameters.mode) {
  case NEW:
    run_parameters.params.new = determine_new_params(argc, argv);
    break;
  case SEARCH:
    if (argc < 3) {
      // TODO: FIGURE OUT ERROR HANDLING
      fprintf(stderr, "need options for search\n");
      exit(1);
    }
    run_parameters.params.search = determine_search_params(argc, argv);
    break;
  default: // INVALID
           // TODO: handle this failure state;
    break;
  }
  return run_parameters;
}
