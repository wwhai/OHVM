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
    // if address greater than instructions count, return NOP
    if (address > INSTRUCTIONS_COUNT)
    {
        return 0;
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
