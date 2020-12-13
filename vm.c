//
//  vm.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "vm.h"
#include "log.h"
#include "types.h"
// Initial
void init_vm(vm *vm)
{
  // PC
  vm->pc = 0;
  // SP
  vm->sp = 0;
  // Init register

  // Init ram
  for (unsigned long i = 0; i < ((sizeof(vm->ram)) / (sizeof(uint16))); i++)
  {
    vm->ram[i] = 0;
  }
}
// new vm
vm *new_vm()
{
  vm *openvm = (vm *)malloc(sizeof(vm));
  init_vm(openvm);
  return openvm;
};
// run
void run_bc(vm *vm)
{
  for (int i = 0; i < MAX_RAM_SIZE; i++)
  {
    vm_instruction vm_inst = parse_instruction(vm->ram[i]);
    log_debug("OPCODE:0x%08H FUNC_CODE:0x%08H", vm_inst.operate_code, vm_inst.function_code);
  }
}
// Load bytecode
void load_vmbc(char *path, vm *vm)
{
  FILE *file = fopen(path, "rb");
  if (!file)
  {
    log_error("Bytecode file load failed,file not exist:%s!", path);
    return;
  }
  else
  {
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    log_debug("Bytecode size:%d byte", file_size);
    if (file_size > MAX_RAM_SIZE)
    {
      log_error("Bytecode too large!");
      exit(0);
    }
    fseek(file, 0, SEEK_SET);
    ovmbc ovmbc_info;
    fread(&ovmbc_info, sizeof(ovmbc), 1, file);
    ovmbc_output(&ovmbc_info);
    int bytecode_length = file_size - sizeof(ovmbc);
    byte bytecode[bytecode_length];
    fseek(file, sizeof(ovmbc), SEEK_SET);
    fread(bytecode, bytecode_length, 1, file);
    for (int i = 0; i < bytecode_length; i++)
    {
      vm->ram[i] = bytecode[i];
    }
    fclose(file);
  }
}
