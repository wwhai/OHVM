//
//  cpu.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "cpu.h"
#include "utils.h"
#include "vm.h"
#include "execute_function.h"
//
byte get_instruction(vm *vm)
{
    byte address = get_pc(vm);
    if (address > INSTRUCTIONS_COUNT)
    {
        return 0x00;
    }
    else
    {
        return address;
    }
}
// return pc next offset bytes
int execute(vm *vm)
{
    operate_functions[get_instruction(vm)](vm);
    return 0;
}
