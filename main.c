//
//  main.c
//  OpenVM
//
//  Created by wwhai on 2020/11/26.
//

#include <stdio.h>
#include "vm.h"
int main(int argc, char *argv[]){
    log_set_level(LOG_DEBUG);
    vm * openvm = new_vm();
    load_vmbc("/Users/wangwenhai/github/OpenVM/res/test_nop.ovmbc", openvm);
    run_bc(openvm);
    return 0;
}
