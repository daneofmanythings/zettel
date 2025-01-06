#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "filesystem.h"
#include "options.h"
#include "run.h"

extern const template_defaults_t TEMPLATE_DEFAULTS[];

void run(run_parameters_t rp) {
  switch (rp.mode) {
  case NEW:
    run_new(rp.params.new.template, rp.params.new.content);
    break;
  case SEARCH:
    run_search(rp.params.search.type, rp.params.search.locations);
    break;
  default:
    break;
  }
}

void run_new(template_defaults_index template, char* content) {
  char path[PATH_MAX] = {0};
  const char* body;
  determine_path(path, template, content);
  if (template == SCRATCH) {
    write_to_scratch(path, content);
    return;
  }
  if (false) { // condition to check if a custom template lives in the config dir
    // load custom template
    //
    // body = custom template
  } else {
    body = TEMPLATE_DEFAULTS[template].body;
  }
  // TODO: CHECK IF THE FILE EXISTS FIRST;
  struct stat st = {0};
  if (stat(path, &st) == 0) { // file already exists;
    // TODO: alert user that the file exists
    return;
  }
  write_new_file(path, TEMPLATE_DEFAULTS[template].body);
}

void determine_path(char dest[PATH_MAX], template_defaults_index template, char* content) {
  expand_path_env_variables(dest, TEMPLATE_DEFAULTS[template].path);
  char filename[NAME_MAX] = {0};
  if (template == JOURNAL) {
    // get the date. config options for formatting here possibly;
    time_t time_raw;
    struct tm* time_info;
    time(&time_raw);
    time_info = localtime(&time_raw);
    sprintf(filename, "/%d.%d.%d.md", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday);
  } else if (template == SCRATCH) {
    // scratch's path is a filepath;
  } else {
    sprintf(filename, "/%s.md", content);
  }
  strcat(dest, filename);
}

void run_search(search_type_defaults_index type, uint8_t location_mask) {
  printf("search called with mode=%d and mask=%d\n", type, location_mask);
}
