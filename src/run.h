#ifndef SRC_RUN_H
#define SRC_RUN_H

#include "options.h"

void run(run_parameters_t run_parameters);
void run_new(template_defaults_index template, char* content);
void determine_path(char dest[PATH_MAX], template_defaults_index template, char* content);
void run_search(search_type_defaults_index type, uint8_t location_mask);

#endif // !DEBUG
