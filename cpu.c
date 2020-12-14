//
//  cpu.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "cpu.h"
//
byte get_instruction(vm *vm)
{
    return 0;
}
// Parse instruction
vm_instruction parse_instruction(byte instruction)
{
    byte operate_code = (instruction >> 4) & 1;
    byte function_code = ((instruction << 4) >> 4) & 1;
    vm_instruction i = {operate_code, function_code};
    byte result[2] = {operate_code, function_code};
    return i;
}
// return pc next offset bytes
int execute(vm *vm)
{
    return 0;
}
