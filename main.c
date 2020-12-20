//
//  main.c
//  OpenVM
//
//  Created by wwhai on 2020/11/26.
//

#include <stdio.h>
#include "vm.h"
int main(int argc, char *argv[])
{
    char *file = "/Users/wangwenhai/github/OpenVM/res/test_nop.ovmbc";
    log_set_level(LOG_DEBUG);
    vm *openvm = new_vm();
    load_vmbc(file, openvm);
    run_bc(openvm);
    return 0;
}
