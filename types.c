//
//  types.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "types.h"
void ovmbc_output(ovmbc *bc)
{
    log_debug("File type:%s", bc->type);
    log_debug("Version:%d-%d-%d", bc->v_main, bc->v_progress, bc->v_bugfix);
    log_debug("Start at:0x%032xH", bc->start_address);
}
//
vm_instruction parse_instruction(byte instruction)
{
  byte operate_code = (instruction >> 4) & 1;
  byte function_code = ((instruction << 4) >> 4) & 1;
  vm_instruction ins = {operate_code, function_code};
  byte result[2] = {operate_code, function_code};
  return ins;
}