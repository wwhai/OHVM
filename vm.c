//
//  vm.c
//  OpenVM
//
//  Created by 王文海 on 2020/11/25.
//

#include "vm.h"
#include "log.h"
// Initial
void init_vm(vm* vm){
  // PC
  vm->pc = 0;
  // SP
  vm->sp = 0;
  // Init ram
  log_debug("Initial ram...");
  for(unsigned long i = 0; i < ((sizeof(vm->ram))/(sizeof(unsigned short))); i++ ){
    vm->ram[i] = 0;
  }
  // Init rom
  log_debug("Initial rom...");
  for(unsigned long i = 0; i < ((sizeof(vm->rom))/(sizeof(unsigned short))); i++ ){
    vm->rom[i] = 0;
  }
  //Init Register
  log_debug("Initial registers...");
  for(unsigned long i = 0; i < ((sizeof(vm->r))/(sizeof(int))); i++ ){
    vm->r[i] = 0;
  }
  log_debug("Initial finished.");
}
// new vm
vm* new_vm(){
    vm* openvm = (vm*)malloc(sizeof(vm));
    init_vm(openvm);
    return openvm;
};
// Load bytecode
void load_rom(char* path, vm* vm){
    FILE * file = fopen(path, "rb");
    if(!file){
      return;
    }else{
      fseek(file, 0L, SEEK_END);
      int size = ftell(file);
      log_debug("file size:%d", size);
    }
}

