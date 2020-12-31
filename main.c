
//
//  main.c
//  OpenVM
//
//  Created by wwhai on 2020/11/26.
//

#include <stdio.h>
#include "ohvm.h"
#include <string.h>
#include "main.h"
int main(int argc, char *argv[]) {
  char *file = "/Users/wangwenhai/github/OpenVM/res/call_test.ohvmbc";
  log_set_level(LOG_DEBUG);
  ohvm *openohvm = new_ohvm();
  load_ohvmbc(file, openohvm);
  run_bc(openohvm);
  return 0;
}
