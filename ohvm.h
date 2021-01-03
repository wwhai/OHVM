//
//  ohvm.h
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#ifndef ohvm_h
#define ohvm_h

#include <stdio.h>
#include <stdlib.h>
#include "clog.h"
#include "cpu.h"
#include "types.h"
// Max byte code size, Develop use 4kb, Can configure other value
#ifndef MAX_RAM_SIZE
#define MAX_RAM_SIZE 1024 * 4
#endif
// Max stack deepth
#define MAX_STACK_DEEPTH 16
// Max register count
#define MAX_REGISTER_COUNT 8
// Internal address
#define INTERNAL_ADDRESS_SIZE 1024
// INSTRUCTIONS_COUNT: 256
#define INSTRUCTIONS_COUNT 0xFF
typedef struct __attribute__((__packed__))
{
  uint32 r[MAX_REGISTER_COUNT];
  uint32 pc;
  uint32 acc;
  uint32 ex;
  // Flags: |0|0|0|0|0|0|0|0|
  struct
  {
    int fze : 1;
    int feq : 1;
    int fex : 1;
    int f4 : 1;
    int f5 : 1;
    int f6 : 1;
    int f7 : 1;
    int f8 : 1;
  } flag;
} stack;
/**
* VM
*/
typedef struct __attribute__((__packed__))
{
  // Bytecode size
  uint32 bc_size;
  // stack deepth
  uint32 sd;
  // stack pointer
  uint32 sp;
  // stack base pointer
  uint32 sbp;
  // RAM
  byte ram[MAX_RAM_SIZE];
  // stack 1024 * 8
  stack stack[MAX_STACK_DEEPTH];
} ohvm;
// operate function define
typedef void (*operate_function)(ohvm *vm);
// new ohvm
ohvm *new_ohvm(void);
// Initial
void init_ohvm(ohvm *vm);
// load byte code
void load_ohvmbc(char *path, ohvm *vm);
// run byte code
void run_bc(ohvm *vm);
// set/get value to ram
void set_ram_value(uint32 offset, byte value, ohvm *vm);
uint32 get_ram_value(uint32 offset, ohvm *vm);
// reset
void reset_ram(ohvm *vm);
// set/get value to/from acc
void set_acc_value(uint32 value, ohvm *vm);
uint32 get_acc_value(ohvm *vm);
// set/get value to/from pc
void set_pc_value(uint32 value, ohvm *vm);
uint32 get_pc_value(ohvm *vm);
// set/get value to/from sd
void set_sd(byte value, ohvm *vm);
uint32 get_sd(ohvm *vm);
// set/get value to/from sp
void set_sp(byte value, ohvm *vm);
uint32 get_sp(ohvm *vm);
// set/get value to/from ex
void set_ex(byte value, ohvm *vm);
uint32 get_ex(ohvm *vm);
// set/get register value
void set_r_value(int rn, uint32 value, ohvm *vm);
uint32 get_r_value(int rn, ohvm *vm);
//
void execute(ohvm *vm);
//
void load_instrucsions(operate_function *operate_functions);
//
void NOP_0X00(ohvm *vm);
void START_0X01(ohvm *vm);
void STOP_0X02(ohvm *vm);
void RSTR_0X03(ohvm *vm);
void TIMER_0X04(ohvm *vm);
void GOTO_0X05(ohvm *vm);
void CALL_0X06(ohvm *vm);
void BACK_0X07(ohvm *vm);
void JMP_0X08(ohvm *vm);
void JZ_0X09(ohvm *vm);
void JE_0X0A(ohvm *vm);
void JX_0X0B(ohvm *vm);
void CMRAE_0X0C(ohvm *vm);
void CMRAG_0X0D(ohvm *vm);
void CMRAL_0X0E(ohvm *vm);
void CMRRE_0X0F(ohvm *vm);
void CMRRG_0X10(ohvm *vm);
void CMRRL_0X11(ohvm *vm);
void CMASE_0X12(ohvm *vm);
void CMASG_0X13(ohvm *vm);
void CMASL_0X14(ohvm *vm);
void CMRSE_0X15(ohvm *vm);
void CMRSG_0X16(ohvm *vm);
void CMRSL_0X17(ohvm *vm);
void INCA_0X18(ohvm *vm);
void DECA_0X19(ohvm *vm);
void INCR_0X1A(ohvm *vm);
void DECR_0X1B(ohvm *vm);
void ADDAR_0X1C(ohvm *vm);
void SUBAR_0X1D(ohvm *vm);
void INCS_0X1E(ohvm *vm);
void DECS_0X1F(ohvm *vm);
void ADDAS_0X20(ohvm *vm);
void SUBAS_0X21(ohvm *vm);
void ANDR_0X22(ohvm *vm);
void AOR_0X23(ohvm *vm);
void AXR_0X24(ohvm *vm);
void BSLR_0X25(ohvm *vm);
void BSRR_0X26(ohvm *vm);
void BSLLR_0X27(ohvm *vm);
void BSRLR_0X28(ohvm *vm);
void ANDS_0X29(ohvm *vm);
void AOS_0X2A(ohvm *vm);
void AXS_0X2B(ohvm *vm);
void BSLS_0X2C(ohvm *vm);
void BSRS_0X2D(ohvm *vm);
void BSLLS_0X2E(ohvm *vm);
void BSRLS_0X2F(ohvm *vm);
void IMA_0X30(ohvm *vm);
void IMR_0X31(ohvm *vm);
void IMS_0X32(ohvm *vm);
void GET_0X33(ohvm *vm);
void MVRR_0X34(ohvm *vm);
void MVRS_0X35(ohvm *vm);
void MVSR_0X36(ohvm *vm);
void MVAR_0X37(ohvm *vm);
void INTK_0X38(ohvm *vm);
void KEY_0X39(ohvm *vm);
void PLY_0X3A(ohvm *vm);
void PLYS_0X3B(ohvm *vm);
void GSET_0X3C(ohvm *vm);
void CLS_0X3D(ohvm *vm);
void DPXY_0X3E(ohvm *vm);
void DXY_0X3F(ohvm *vm);
void DCXY_0X40(ohvm *vm);
void DLINE_0X41(ohvm *vm);
void DRECT_0X42(ohvm *vm);
void SCRU_0X43(ohvm *vm);
void SCRD_0X44(ohvm *vm);
void SCRL_0X45(ohvm *vm);
void SCRR_0X46(ohvm *vm);
void SET_0X47(ohvm *vm);

#endif
