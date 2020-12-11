//
//  main.c
//  OpenVM
//
//  Created by 王文海 on 2020/11/26.
//

#include <stdio.h>
#include "vm.h"
int main(int argc, char *argv[]){
    log_set_level(LOG_DEBUG);
    vm * openvm = new_vm();
    load_rom("/Users/wangwenhai/github/OpenVM/res/test.vmc", openvm);
    return 0;
}
