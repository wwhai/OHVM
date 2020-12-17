#include <stdio.h>

int compile_bc(unsigned char *bc, int length)
{
    FILE *file = fopen("test_nop.ovmbc", "wb");
    if (!file)
    {
        printf("Unable to open file!");
        return 1;
    }
    unsigned char v1 = 0, v2 = 0, v3 = 1;
    unsigned int start_address = 0;
    fwrite("VMBC", 4, 1, file);
    fwrite(&v1, 1, 1, file);
    fwrite(&v2, 1, 1, file);
    fwrite(&v3, 1, 1, file);
    fwrite(&start_address, 4, 1, file);
    fwrite(bc, length, 1, file);
    fclose(file);
    return 0;
}
// rm -rf ./test_nop.ovmbc && gcc test.c -o test && ./test && rm -rf test && xxd test_nop.ovmbc
int main(int argc, char const *argv[])
{
    unsigned char bc[] = {0x0000, 0x0001, 0x0002};
    compile_bc(bc, sizeof(bc));
    return 0;
}
