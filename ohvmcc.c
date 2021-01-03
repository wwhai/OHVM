//
//  ohvmcc.c
//  OHVM
//
//  Created by wwhai on 2020/12/24.
//

#include "ohvmcc.h"
#include "cpu.h"
typedef unsigned char I;
//
int compile_bc(char *name, unsigned char *bc, int length)
{
    FILE *file = fopen(name, "wb");
    if (!file)
    {
        printf("[OHVMCC] Unable to open file!");
        return 1;
    }
    unsigned int start = 0;
    unsigned char v1 = 0, v2 = 0, v3 = 1, stop = 0x0002;
    fwrite("VMBC", 4, 1, file);
    printf("[OHVMCC] Write file header 'VMBC'\n");
    fwrite(&v1, 1, 1, file);
    fwrite(&v2, 1, 1, file);
    fwrite(&v3, 1, 1, file);
    printf("[OHVMCC] Write file version %d.%d.%d\n", v1, v2, v3);
    fwrite(&start, 4, 1, file);
    printf("[OHVMCC] Start address is: %d\n", start);
    fwrite(bc, length, 1, file);
    fwrite(&stop, 1, 1, file);
    fclose(file);
    printf("[OHVMCC] Finished\n");

    return 0;
}

//gcc oohvmcc.c -o oohvmcc && ./oohvmcc && rm -rf oohvmcc && xxd oohvmcc_nop.ohvmbc && mv ./*.ohvmbc ./res/
int main(int argc, char const *argv[])
{
    // NOP
    unsigned char nop_test[] = {
        NOP,
        NOP,
        NOP,
        // assert pc == 3
    };
    compile_bc("nop_test.ohvmbc", nop_test, sizeof(nop_test));
    // ADD R0 R1
    unsigned char add_r0_r1_test[] = {
        // IMR R0 0001H
        IMR,
        (I)0x0000, // R0
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        // IMR R1 0001H
        IMR,
        (I)0x0001, // R1
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        (I)0x0001,
        // IMR R2 0002H
        IMR,
        (I)0x0002, // R2
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        (I)0x0002,
        ADDAR,
        (I)0x0000, // ADDAR R0
        ADDAR,
        (I)0x0001, // ADDAR R1
        ADDAR,
        (I)0x0002, // ADDAR R2
        // assert ACC == 3
    };
    compile_bc("add_r0_r1_test.ohvmbc", add_r0_r1_test, sizeof(add_r0_r1_test));
    unsigned char call_stack_overflow_test[] = {
        IMA, // Start at 0x0001 (ram[2015])
        (I)0x00,
        (I)0x00,
        (I)0x00,
        (I)0x01,
        ADDAR,
        (I)0x00, // ADDAR R0
        CALL,    // CALL 0x0001 BACK
        (I)0x00,
        (I)0x00,
        (I)0x04,
        (I)0x01,
        (I)0x01,
    };
    compile_bc("call_stack_overflow_test.ohvmbc", call_stack_overflow_test, sizeof(call_stack_overflow_test));
    unsigned char call_test[] = {
        CALL, // CALL 0x0001 BACK
        (I)0x00,
        (I)0x00,
        (I)0x04,
        (I)0x07,
        (I)0x01,
        (I)0x02,
        //----------------------------
        // FUN:
        //    ADD A R0
        //    BACK
        // END
        IMR,     // 0x31     // Start at 0x0001 (ram[2015])
        (I)0x00, // Init R0 = 0
        (I)0x00,
        (I)0x00,
        (I)0x00,
        (I)0x01,
        ADDAR, // 1C ,  0x1C
        (I)0x00,
        (I)0x07, // RETURN
        // Assert pc == 1
        //----------------------------
    };
    compile_bc("call_test.ohvmbc", call_test, sizeof(call_test));
    return 0;
}
