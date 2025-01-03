#include "options.h"
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mode_e determine_mode(char* mode) {
  size_t i;
  for (i = 0; strcmp(modes[i], "\0") != 0; i++) {
    if (strcmp(mode, modes[i]) == 0) {
      break;
    }
    // also valid to use the single letter.
    if (strlen(mode) == 1 && strncmp(mode, modes[i], 1) == 0) {
      break;
    }
  }
  return i;
}

new_template_e determine_template(char* template_args) {
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
  for (i = 0; strcmp(templates[i], "\0") != 0; i++) {
    if (strncmp(t, templates[i], 1) == 0) {
      break;
    }
  }
  return i;
}

// WARN: this should probably return an error, not a bool
bool determine_title(char title[NAME_MAX], int argc, char** argv) {
  if (argc < 4) {
    return false;
  }
  // TODO: bounds check the length of the title so it doesn't overflow.
  char* title_ptr = title;
  for (size_t i = 3; i < argc; i++) {
    title_ptr = stpcpy(title_ptr, argv[i]);
    *title_ptr++ = '.';
  }
  *(--title_ptr) = '\0';
  return true;
}

new_mode_params_t determine_new_params(int argc, char** argv) {
  new_mode_params_t params;
  // TODO: check bounding
  params.template = determine_template(argv[2]);
  if (!determine_title(params.title, argc, argv)) {
    // handle error
  }
  return params;
}

search_type_e determine_search(char* search_args) {
  size_t len = strlen(search_args);
  if (len < 2) {
    return INVALID_SEARCH;
  }

  if (search_args[0] != '-') {
    // TODO: implement whole names.
    return INVALID_SEARCH;
  }
  size_t i;
  char* t = &search_args[1];
  for (i = 0; strcmp(searches[i], "\0") != 0; i++) {
    if (strncmp(t, searches[i], 1) == 0) {
      break;
    }
  }
  return i;
}

uint8_t determine_search_locations(char* search_args) {
  // TODO: check for '-'

  // TODO: add full name functionality?

  uint8_t location_mask = 0;
  for (size_t i = 0; strcmp(search_locations[i], "\0") != 0; ++i) {
    if (strchr(search_args, search_locations[i][0])) {
      location_mask += pow(2, i);
    }
  }
  return location_mask;
}

search_mode_params_t determine_search_params(int argc, char** argv) {
  search_mode_params_t params = {0};
  params.type = determine_search(argv[2]);
  params.locations = determine_search_locations(argv[2]);
  return params;
}

run_parameters_t run_parameters_create(int argc, char** argv) {
  run_parameters_t run_parameters = {0};
  run_parameters.mode = determine_mode(argv[1]);
  // dispatch on mode
  switch (run_parameters.mode) {
  case NEW:
    run_parameters.parameters.new = determine_new_params(argc, argv);
    break;
  case SEARCH:
    if (argc < 3) {
      // TODO: FIGURE OUT ERROR HANDLING
      fprintf(stderr, "need options for search\n");
      exit(1);
    }
    run_parameters.parameters.search = determine_search_params(argc, argv);
    break;
  default: // INVALID
           // TODO: handle this failure state;
    break;
  }
  return run_parameters;
}
