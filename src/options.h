#ifndef SRC_OPTIONS_H
#define SRC_OPTIONS_H

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern const char ZET_KNOWLEDGE[];
extern const char ZET_PROJECTS[];
extern const char ZET_SCRATCH[];
extern const char ZET_JOURNAL[];
extern const char ZET_ARCHIVE[];
extern const char KNOWLEDGE_BODY[];
extern const char PROJECT_BODY[];
extern const char SCRATCH_BODY[];
extern const char JOURNAL_BODY[];

// TODO: I think these constants may need to go elsewhere. LIKE MAYBE THE CONSTANTS FILE
typedef struct {
  const char* name;
  const char* short_name;
} mode_defaults_t;
static const mode_defaults_t MODE_DEFAULTS[] = {
    {"new",    "n" },
    {"search", "s" },
    {"\0",     "\0"},
};
typedef enum {
  NEW,
  SEARCH,
  INVALID_MODE,
} mode_defaults_index;

typedef struct {
  const char* name;
  const char* short_name;
  const char* path;
  const char* body;
} template_defaults_t;
static const template_defaults_t TEMPLATE_DEFAULTS[] = {
    {"knowledge", "k",  ZET_KNOWLEDGE, KNOWLEDGE_BODY},
    {"project",   "p",  ZET_PROJECTS,  PROJECT_BODY  },
    {"scratch",   "s",  ZET_SCRATCH,   SCRATCH_BODY  },
    {"journal",   "j",  ZET_JOURNAL,   JOURNAL_BODY  },
    {"\0",        "\0", "\0",          "\0"          },
};
typedef enum {
  KNOWLEDGE,
  PROJECT,
  SCRATCH,
  JOURNAL,
  INVALID_TEMPLATE,
} template_defaults_index;

typedef struct {
  const char* name;
  const char* short_name;
} search_type_defaults_t;
static const search_type_defaults_t SEARCH_TYPE_DEFAULTS[] = {
    {"filenames", "f" },
    {"contents",  "c" },
    {"\0",        "\0"},
};
typedef enum {
  FILENAMES,
  CONTENTS,
  INVALID_SEARCH,
} search_type_defaults_index;

typedef struct {
  const char* name;
  const char* short_name;
  const char* path;
} search_location_defaults_t;
static const search_location_defaults_t SEARCH_LOCATION_DEFAULTS[] = {
    {"knowledge", "k",  ZET_KNOWLEDGE},
    {"project",   "p",  ZET_PROJECTS },
    {"scratch",   "s",  ZET_SCRATCH  },
    {"journal",   "j",  ZET_JOURNAL  },
    {"archive",   "a",  ZET_ARCHIVE  },
    {"\0",        "\0", "\0"         },
};
typedef enum {
  KNOWLEDGES,
  PROJECTS,
  SCRATCHES,
  JOURNALS,
  ARCHIVES,
  INVALID_SEARCH_LOCATION,
} search_locations_defaults_index;

typedef struct new_mode_params {
  template_defaults_index template;
  char content[NAME_MAX];
} new_mode_params_t;

typedef struct search_mode_params {
  search_type_defaults_index type;
  uint8_t locations;
} search_mode_params_t;

typedef struct run_parameters {
  mode_defaults_index mode;
  union {
    new_mode_params_t new;
    search_mode_params_t search;
  } params;
} run_parameters_t;

mode_defaults_index determine_mode(char* mode);
template_defaults_index determine_template(char* template_args);
void determine_content(template_defaults_index template, char title_ptr[NAME_MAX], int argc, char** argv);
new_mode_params_t determine_new_params(int argc, char** argv);
search_type_defaults_index determine_search_type(char* search_args);
uint8_t determine_search_locations(char* search_args);
search_mode_params_t determine_search_params(int argc, char** argv);
run_parameters_t run_parameters_create(int argc, char** argv);

#endif // !DEBUG
