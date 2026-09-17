
#include "vm.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *program)
{
  fprintf(stderr, "Usage: %s [--trace] [--max-steps N] program.ohvmbc\n",
      program);
}

int main(int argc, char **argv)
{
  const char *path = NULL;
  bool trace = false;
  size_t max_steps = 1000000;

  for (int index = 1; index < argc; ++index) {
    if (strcmp(argv[index], "--trace") == 0) {
      trace = true;
    } else if (strcmp(argv[index], "--max-steps") == 0 && index + 1 < argc) {
      char *end;
      errno = 0;
      unsigned long long value = strtoull(argv[++index], &end, 10);
      if (errno != 0 || *end != '\0' || value == 0 || value > SIZE_MAX) {
        usage(argv[0]);
        return EXIT_FAILURE;
      }
      max_steps = (size_t)value;
    } else if (path == NULL) {
      path = argv[index];
    } else {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  if (path == NULL) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  Ohvm vm;
  ohvm_init(&vm);
  OhvmStatus status = ohvm_load_file(&vm, path);
  if (status != OHVM_OK) {
    fprintf(stderr, "ohvm: %s: %s\n", path, ohvm_status_name(status));
    return EXIT_FAILURE;
  }
  vm.trace = trace;
  status = ohvm_run(&vm, max_steps);
  if (status != OHVM_OK) {
    fprintf(stderr, "ohvm: byte %u: %s\n", vm.ip,
        ohvm_status_name(status));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
