//
//  vm.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "vm.h"
#include "log.h"
#include "types.h"
#include "cpu.h"
#include "execute_function.h"
#include <string.h>
// Load Instructions
void load_instrucsions(operate_function *operate_functions)
{
  operate_functions[0x0000] = NOP;
  operate_functions[0x0001] = START;
  operate_functions[0x0002] = STOP;
  operate_functions[0x0003] = RSTR;
  operate_functions[0x0004] = TIMER;
  operate_functions[0x0005] = GOTO;
  operate_functions[0x0006] = JNOC;
  operate_functions[0x0007] = CALL;
  operate_functions[0x0008] = BACK;
}
// Initial
void init_vm(vm *vm)
{
  // PC
  vm->pc = 0;
  // SP
  vm->sp = 0;
  // BC size
  vm->bc_size = 0;
  // Init ram
  reset_ram(vm);
  // Load Instructions
  load_instrucsions(operate_functions);
}

// new vm
vm *new_vm()
{
  vm *openvm = new_object(vm);
  init_vm(openvm);
  return openvm;
};
/**
 * 
 *  Core process
 * 
 * */
void run_bc(vm *vm)
{
  while (true)
  {
    execute(vm);
  }
}
// Load bytecode
void load_vmbc(char *path, vm *vm)
{
  FILE *file = fopen(path, "rb");
  if (!file)
  {
    log_error("Bytecode load failed,file not exist:%s!", path);
    exit(0);
  }
  else
  {
    fseek(file, 0L, SEEK_END);
    int bc_file_size = ftell(file);
    if (bc_file_size > MAX_RAM_SIZE - INTERNAL_ADDRESS_SIZE)
    {
      log_error("Bytecode too large!");
      exit(0);
    }
    fseek(file, 0, SEEK_SET);
    ovmbc ovmbc_info;
    fread(&ovmbc_info, sizeof(ovmbc), 1, file);
    ovmbc_output(&ovmbc_info);
    int bytecode_length = bc_file_size - type_size(ovmbc_info);
    vm->bc_size = bytecode_length;
    byte bytecode[bytecode_length];
    fseek(file, sizeof(ovmbc), SEEK_SET);
    fread(bytecode, bytecode_length, 1, file);
    log_debug("Bytecode file size:%d byte size:%d", bc_file_size, bytecode_length);
    for (size_t i = INTERNAL_ADDRESS_SIZE; i < INTERNAL_ADDRESS_SIZE + bytecode_length; i++)
    {
      set_ram(i, bytecode[i - INTERNAL_ADDRESS_SIZE], vm);
    }
    fclose(file);
  }
}
// reset
void reset_ram(vm *vm)
{
  // 1024 - MAX
  for (uint32 i = INTERNAL_ADDRESS_SIZE; i < MAX_RAM_SIZE; i++)
  {
    set_ram(i, 0, vm);
  }
}
// set value to ram
void set_ram(uint32 offset, byte value, vm *vm)
{
  if (offset < 0)
  {
    log_error("Offset must be an unsigned integer!");
    exit(0);
  }
  if (offset > MAX_RAM_SIZE)
  {
    log_error("Offset too large ,max ram size is:%d", MAX_RAM_SIZE);
    exit(0);
  }
  // 1024 + offset
  vm->ram[offset] = value;
}
// get value from ram
byte get_ram(uint32 offset, vm *vm)
{
  if (offset < 0)
  {
    log_error("Offset must be an unsigned integer!");
    exit(0);
  }
  if (offset > MAX_RAM_SIZE)
  {
    log_error("Offset too large ,max ram size is:%d", MAX_RAM_SIZE);
    exit(0);
  }
  return vm->ram[offset];
}

// set start address
void set_start_address(uint32 value, vm *vm)
{
  vm->start_address = value;
}
uint32 get_start_address(vm *vm)
{
  return vm->start_address;
}

// set/get value to/from acc
void set_acc(byte value, vm *vm)
{
  vm->ram[ACC] = value;
}
uint32 get_acc(vm *vm)
{
  return vm->ram[ACC];
}
// set/get value to/from pc
void set_pc(uint32 value, vm *vm)
{
  memcpy(&vm->ram[PC], &value, sizeof(uint32));
}
uint32 get_pc(vm *vm)
{
  void *value = calloc(sizeof(byte), 4);
  byte bytes[4] = {vm->ram[PC] + 0, vm->ram[PC] + 1, vm->ram[PC] + 2, vm->ram[PC] + 3};
  memcpy(value, bytes, 1);
  return *((uint32 *)value);
}
// set/get value to/from sd
void set_sd(byte value, vm *vm)
{
  vm->ram[SD] = value;
}
uint32 get_sd(vm *vm)
{
  return vm->ram[SD];
}
// set/get value to/from sp
void set_sp(byte value, vm *vm)
{
  vm->ram[SP] = value;
}
uint32 get_sp(vm *vm)
{
  return vm->ram[SP];
}
// set/get value to/from ex
void set_ex(byte value, vm *vm)
{
  vm->ram[EX] = value;
}
uint32 get_ex(vm *vm)
{
  return vm->ram[EX];
}
