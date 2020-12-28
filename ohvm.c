
//
//  ohvm.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "ohvm.h"
#include "types.h"
#include <string.h>
#include <unistd.h>
// operate function mapping
operate_function operate_functions[INSTRUCTIONS_COUNT];
// Initial
void init_ohvm(ohvm *vm)
{
  // PC is start at 1024
  // 0-1023 be used internal function
  for (uint32 i = 0; i < MAX_STACK_DEEPTH; i++)
  {
    log_debug("INIT STACK[%d]", i);
    for (uint32 j = 0; j < MAX_REGISTER_COUNT; j++)
    {
      log_debug("INIT STACK[%d].R[%d]", i, j);
      vm->stack[i].r[j] = 0;
    }
    vm->stack[i].pc = INTERNAL_ADDRESS_SIZE;
    vm->stack[i].acc = 0;
    vm->stack[i].ex = 0;
  }

  // SP
  vm->sp = 0;
  // BC size
  vm->bc_size = 0;
  //
  vm->sbp = 0;
  //
  vm->sd = 0;
  //
  vm->sp = 0;
}
/**
 *
 * @return ohvm*
 */
ohvm *new_ohvm()
{
  ohvm *openohvm = new_object(ohvm);
  // Init Vm
  init_ohvm(openohvm);
  // Init ram
  reset_ram(openohvm);
  //
  load_instrucsions(operate_functions);
  //
  return openohvm;
};
//
void run_bc(ohvm *vm)
{
  while (1)
  {
    execute(vm);
  }
}
//
void execute(ohvm *vm)
{
  uint32 deepth = vm->sd;
  uint32 i = vm->ram[vm->stack[deepth].pc];
  uint32 pc = vm->stack[deepth].pc;
  log_debug("CURRENT STACK:%d INSTRUCTION:0x%08x PC:%d", deepth, i, pc);
  operate_functions[vm->ram[pc]](vm);
}
// Load bytecode
void load_ohvmbc(char *path, ohvm *vm)
{
  log_debug("LOAD VM BYTE CODE");
  FILE *file = fopen(path, "rb");
  if (!file)
  {
    log_error("File not exist:%s!", path);
    exit(0);
  }
  else
  {
    fseek(file, 0L, SEEK_END);
    unsigned long bc_file_size = ftell(file);
    if (bc_file_size > MAX_RAM_SIZE - INTERNAL_ADDRESS_SIZE)
    {
      log_error("File too large!");
      exit(0);
    }
    fseek(file, 0, SEEK_SET);
    ohvmbc ohvmbc_info;
    fread(&ohvmbc_info, sizeof(ohvmbc), 1, file);
    ohvmbc_output(&ohvmbc_info);
    unsigned long bytecode_length = bc_file_size - type_size(ohvmbc_info);
    vm->bc_size = (uint32)bytecode_length;
    byte bytecode[bytecode_length];
    fseek(file, sizeof(ohvmbc), SEEK_SET);
    fread(bytecode, bytecode_length, 1, file);
    log_debug("BC FILE SIZE :%d BC SIZE:%d", bc_file_size, bytecode_length);
    for (uint32 i = INTERNAL_ADDRESS_SIZE;
         i < INTERNAL_ADDRESS_SIZE + bytecode_length; i++)
    {
      set_ram_value(i, bytecode[i - INTERNAL_ADDRESS_SIZE], vm);
    }
    fclose(file);
  }
}
// reset
void reset_ram(ohvm *vm)
{
  log_debug("RESET RAM");
  // [INTERNAL_ADDRESS_SIZE - MAX_RAM_SIZE]
  for (uint32 i = INTERNAL_ADDRESS_SIZE; i < MAX_RAM_SIZE; i++)
  {
    set_ram_value(i, 0, vm);
  }
}
// set value to ram
void set_ram_value(uint32 offset, byte value, ohvm *vm)
{
  if (offset > MAX_RAM_SIZE)
  {
    log_error("Offset too large ,max ram size is:%d", MAX_RAM_SIZE);
    exit(0);
  }
  // 1024 + offset
  log_debug("SET RAM[%d] = %d", offset, value);
  vm->ram[offset] = value;
}
// get value from ram
uint32 get_ram_value(uint32 offset, ohvm *vm)
{
  uint32 pc = vm->stack[vm->sd].pc;
  // Max is 4
  if (offset > 4)
  {
    return 0;
  }
  if (offset == 1)
  {
    return (uint32)vm->ram[pc];
  }
  if (offset == 4)
  {
    union
    {
      byte bv[4];
      uint32 iv;
    } v, tv;
    tv.bv[0] = 1;
    if (tv.iv == 1)
    {
      // big
      for (uint32 i = 0; i < offset; i++)
      {
        v.bv[i] = vm->ram[pc + i];
      }
    }
    else
    {
      // little
      for (uint32 i = 0; i < offset; i++)
      {
        v.bv[offset - 1 - i] = vm->ram[pc + i];
      }
    }

    return v.iv;
  }
  return 0;
}
// set/get value to/from acc
void set_acc_value(uint32 value, ohvm *vm)
{
  uint32 deepth = vm->sd;
  vm->stack[deepth].acc = value;
  log_debug("SET STACK[%d], ACC = %d", deepth, value);
}
//
uint32 get_acc_value(ohvm *vm)
{
  uint32 deepth = vm->sd;
  return vm->stack[deepth].acc;
}
// set/get value to/from pc
void set_pc_value(uint32 value, ohvm *vm) { vm->stack[vm->sd].pc = value; }
uint32 get_pc_value(ohvm *vm) { return vm->stack[vm->sd].pc; }
// set/get value to/from sd
void set_sd(byte value, ohvm *vm) { vm->sd = value; }
uint32 get_sd(ohvm *vm) { return vm->sd; }
// set/get value to/from sp
void set_sp(byte value, ohvm *vm) { vm->sp = value; }
uint32 get_sp(ohvm *vm) { return vm->sp; }
// set/get value to/from ex
void set_ex(byte value, ohvm *vm) { vm->stack[vm->sd].ex = value; }
uint32 get_ex(ohvm *vm) { return vm->stack[vm->sd].ex; }
//
void set_r_value(int rn, uint32 value, ohvm *vm)
{
  vm->stack[vm->sd].r[rn] = value;
  log_debug("SET STACK[%d], R[%d] = %d", vm->sd, rn, value);
}
//
uint32 get_r_value(int rn, ohvm *vm)
{
  uint32 sd = vm->sd;
  stack stack = vm->stack[sd];
  return stack.r[rn];
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void NOP_0X0000(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  uint32 pc = vm->stack[vm->sd].pc;
  set_pc_value(pc + 1, vm);
}
void START_0X0001(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  uint32 pc = vm->stack[vm->sd].pc;
  set_pc_value(pc + 1, vm);
}
void STOP_0X0002(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  uint32 pc = vm->stack[vm->sd].pc;
  set_pc_value(pc + 1, vm);
  exit(0);
}
void RSTR_0X0003(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  uint32 pc = vm->stack[vm->sd].pc;
  set_pc_value(pc + 1, vm);
}
// SLEEP ms
void TIMER_0X0004(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  uint32 pc = vm->stack[vm->sd].pc;
  set_pc_value(pc + 1, vm);
  uint32 value = get_ram_value(4, vm);
  sleep(value);
  set_pc_value(pc + 4, vm);
}
// GOTO
void GOTO_0X0005(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  uint32 pc = vm->stack[vm->sd].pc;
  set_pc_value(pc + 1, vm);
  uint32 address = get_ram_value(4, vm);
  vm->stack[vm->sd].pc = address;
}
// 1 PUSH Stack
// 2 save Rn
// BACK : return value
// NO BACK : void
void CALL_0X0006(ohvm *vm) {}
void BACK_0X0007(ohvm *vm) {}
void JMP_0X0008(ohvm *vm) {}
void JZ_0X0009(ohvm *vm) {}
void JE_0X000A(ohvm *vm) {}
void JX_0X000B(ohvm *vm) {}
void CMRAE_0X000C(ohvm *vm) {}
void CMRAG_0X000D(ohvm *vm) {}
void CMRAL_0X000E(ohvm *vm) {}
void CMRRE_0X000F(ohvm *vm) {}
void CMRRG_0X0010(ohvm *vm) {}
void CMRRL_0X0011(ohvm *vm) {}
void CMASE_0X0012(ohvm *vm) {}
void CMASG_0X0013(ohvm *vm) {}
void CMASL_0X0014(ohvm *vm) {}
void CMRSE_0X0015(ohvm *vm) {}
void CMRSG_0X0016(ohvm *vm) {}
void CMRSL_0X0017(ohvm *vm) {}
// Acc = Acc + 1
void INCA_0X0018(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_acc_value(vm->stack[vm->sd].acc + 1, vm);
}
// Acc = Acc- 1
void DECA_0X0019(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_acc_value(vm->stack[vm->sd].acc - 1, vm);
}
// Rn = Rn + 1
void INCR_0X001A(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  // Rn ++
  uint32 sd = vm->sd;
  stack stack = vm->stack[sd];
  set_r_value(rn, stack.r[rn] + 1, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// Rn = Rn -1
void DECR_0X001B(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  // Rn --
  set_r_value(rn, vm->stack[vm->sd].r[rn] - 1, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// ADDAR Rn
void ADDAR_0X001C(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  uint32 rn_value = vm->stack[vm->sd].r[rn];
  uint32 acc = vm->stack[vm->sd].acc;
  set_acc_value(acc + rn_value, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  log_debug("ADDAR: ACC=%d STACK[%d]R[%d] = %d", acc, vm->sd, rn, rn_value);
}
void SUBAR_0X001D(ohvm *vm) {}
void INCS_0X001E(ohvm *vm) {}
void DECS_0X001F(ohvm *vm) {}
void ADDAS_0X0020(ohvm *vm) {}
void SUBAS_0X0021(ohvm *vm) {}
void ANDR_0X0022(ohvm *vm) {}
void AOR_0X0023(ohvm *vm) {}
void AXR_0X0024(ohvm *vm) {}
void BSLR_0X0025(ohvm *vm) {}
void BSRR_0X0026(ohvm *vm) {}
void BSLLR_0X0027(ohvm *vm) {}
void BSRLR_0X0028(ohvm *vm) {}
void ANDS_0X0029(ohvm *vm) {}
void AOS_0X002A(ohvm *vm) {}
void AXS_0X002B(ohvm *vm) {}
void BSLS_0X002C(ohvm *vm) {}
void BSRS_0X002D(ohvm *vm) {}
void BSLLS_0X002E(ohvm *vm) {}
void BSRLS_0X002F(ohvm *vm) {}
// IMA {value}
void IMA_0X0030(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 value = get_ram_value(4, vm);
  set_acc_value(vm->stack[vm->sd].acc + value, vm);
  set_pc_value(vm->stack[vm->sd].pc + 4, vm);
}
// IMR {value}
void IMR_0X0031(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  // PC +1 -> Rn
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  // PC +1 -> Value
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 value = get_ram_value(4, vm);
  // set value
  set_r_value(rn, value, vm);
  // Next
  set_pc_value(vm->stack[vm->sd].pc + 4, vm);
}
//
void IMS_0X0032(ohvm *vm) {}
// GET {address} -> ACC
void GET_0X0033(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  uint32 sd = vm->sd;
  stack stack = vm->stack[sd];
  set_acc_value(vm->stack[vm->sd].acc + stack.r[rn], vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
void MVRR_0X0034(ohvm *vm) {}
void MVRS_0X0035(ohvm *vm) {}
void MVSR_0X0036(ohvm *vm) {}
void MVAR_0X0037(ohvm *vm) {}
void INTK_0X0038(ohvm *vm) {}
void KEY_0X0039(ohvm *vm) {}
void PLY_0X003A(ohvm *vm) {}
void PLYS_0X003B(ohvm *vm) {}
void GSET_0X003C(ohvm *vm) {}
void CLS_0X003D(ohvm *vm) {}
void DPXY_0X003E(ohvm *vm) {}
void DXY_0X003F(ohvm *vm) {}
void DCXY_0X0040(ohvm *vm) {}
void DLINE_0X0041(ohvm *vm) {}
void DRECT_0X0042(ohvm *vm) {}
void SCRU_0X0043(ohvm *vm) {}
void SCRD_0X0044(ohvm *vm) {}
void SCRL_0X0045(ohvm *vm) {}
void SCRR_0X0046(ohvm *vm) {}
// Load Instructions
void load_instrucsions(operate_function *operate_functions)
{
  operate_functions[0X0000] = NOP_0X0000;
  operate_functions[0X0001] = START_0X0001;
  operate_functions[0X0002] = STOP_0X0002;
  operate_functions[0X0003] = RSTR_0X0003;
  operate_functions[0X0004] = TIMER_0X0004;
  operate_functions[0X0005] = GOTO_0X0005;
  operate_functions[0X0006] = CALL_0X0006;
  operate_functions[0X0007] = BACK_0X0007;
  operate_functions[0X0008] = JMP_0X0008;
  operate_functions[0X0009] = JZ_0X0009;
  operate_functions[0X000A] = JE_0X000A;
  operate_functions[0X000B] = JX_0X000B;
  operate_functions[0X000C] = CMRAE_0X000C;
  operate_functions[0X000D] = CMRAG_0X000D;
  operate_functions[0X000E] = CMRAL_0X000E;
  operate_functions[0X000F] = CMRRE_0X000F;
  operate_functions[0X0010] = CMRRG_0X0010;
  operate_functions[0X0011] = CMRRL_0X0011;
  operate_functions[0X0012] = CMASE_0X0012;
  operate_functions[0X0013] = CMASG_0X0013;
  operate_functions[0X0014] = CMASL_0X0014;
  operate_functions[0X0015] = CMRSE_0X0015;
  operate_functions[0X0016] = CMRSG_0X0016;
  operate_functions[0X0017] = CMRSL_0X0017;
  operate_functions[0X0018] = INCA_0X0018;
  operate_functions[0X0019] = DECA_0X0019;
  operate_functions[0X001A] = INCR_0X001A;
  operate_functions[0X001B] = DECR_0X001B;
  operate_functions[0X001C] = ADDAR_0X001C;
  operate_functions[0X001D] = SUBAR_0X001D;
  operate_functions[0X001E] = INCS_0X001E;
  operate_functions[0X001F] = DECS_0X001F;
  operate_functions[0X0020] = ADDAS_0X0020;
  operate_functions[0X0021] = SUBAS_0X0021;
  operate_functions[0X0022] = ANDR_0X0022;
  operate_functions[0X0023] = AOR_0X0023;
  operate_functions[0X0024] = AXR_0X0024;
  operate_functions[0X0025] = BSLR_0X0025;
  operate_functions[0X0026] = BSRR_0X0026;
  operate_functions[0X0027] = BSLLR_0X0027;
  operate_functions[0X0028] = BSRLR_0X0028;
  operate_functions[0X0029] = ANDS_0X0029;
  operate_functions[0X002A] = AOS_0X002A;
  operate_functions[0X002B] = AXS_0X002B;
  operate_functions[0X002C] = BSLS_0X002C;
  operate_functions[0X002D] = BSRS_0X002D;
  operate_functions[0X002E] = BSLLS_0X002E;
  operate_functions[0X002F] = BSRLS_0X002F;
  operate_functions[0X0030] = IMA_0X0030;
  operate_functions[0X0031] = IMR_0X0031;
  operate_functions[0X0032] = IMS_0X0032;
  operate_functions[0X0033] = GET_0X0033;
  operate_functions[0X0034] = MVRR_0X0034;
  operate_functions[0X0035] = MVRS_0X0035;
  operate_functions[0X0036] = MVSR_0X0036;
  operate_functions[0X0037] = MVAR_0X0037;
  operate_functions[0X0038] = INTK_0X0038;
  operate_functions[0X0039] = KEY_0X0039;
  operate_functions[0X003A] = PLY_0X003A;
  operate_functions[0X003B] = PLYS_0X003B;
  operate_functions[0X003C] = GSET_0X003C;
  operate_functions[0X003D] = CLS_0X003D;
  operate_functions[0X003E] = DPXY_0X003E;
  operate_functions[0X003F] = DXY_0X003F;
  operate_functions[0X0040] = DCXY_0X0040;
  operate_functions[0X0041] = DLINE_0X0041;
  operate_functions[0X0042] = DRECT_0X0042;
  operate_functions[0X0043] = SCRU_0X0043;
  operate_functions[0X0044] = SCRD_0X0044;
  operate_functions[0X0045] = SCRL_0X0045;
  operate_functions[0X0046] = SCRR_0X0046;
}
