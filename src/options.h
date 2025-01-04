#ifndef SRC_OPTIONS_H
#define SRC_OPTIONS_H

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// WARNING: order of the enums and arrays are coupled.
// Do not change one and not the other. Things will break.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

// Supported modes
static const char* modes[] = {
    "new",
    "search",
    "\0",
};
typedef enum {
  NEW,
  SEARCH,
  INVALID_MODE,
} mode_e;
mode_e determine_mode(char* mode);

// templates for 'new' mode
static const char* templates[] = {
    "knowledge",
    "project",
    "scratch",
    "\0",
};
typedef enum {
  KNOWLEDGE,
  PROJECT,
  SCRATCH,
  INVALID_TEMPLATE,
} new_template_e;
new_template_e determine_template(char* template_args);
bool determine_title(char title_ptr[NAME_MAX], int argc, char** argv);

typedef struct new_mode_params {
  new_template_e template;
  char title[NAME_MAX];
} new_mode_params_t;
new_mode_params_t determine_new_params(int argc, char** argv);

// NOTE: search modes and locations can't have overlapping first characters
// search types for 'search' mode
static const char* searches[] = {
    "filenames",
    "contents",
    "\0",
};
typedef enum {
  FILENAMES,
  CONTENTS,
  INVALID_SEARCH,
} search_type_e;
search_type_e determine_search(char* search_args);

static const char* search_locations[] = {
    "knowledges", "projects", "scratches", "archives", "journals", "\0",
};
typedef enum {
  KNOWLEDGES,
  PROJECTS,
  SCRATCHES,
  ARCHIVES,
  JOURNALS,
} search_locations_e;
uint8_t determine_search_locations(char* search_args);

typedef struct search_mode_params {
  search_type_e type;
  uint8_t locations;
} search_mode_params_t;
search_mode_params_t determine_search_params(int argc, char** argv);

typedef struct run_parameters {
  mode_e mode;
  union {
    new_mode_params_t new;
    search_mode_params_t search;
  } params;
} run_parameters_t;
run_parameters_t run_parameters_create(int argc, char** argv);

#pragma GCC diagnostic pop

#endif // !DEBUG
