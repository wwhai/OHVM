#include <stdio.h>

int test_bc()
{
    FILE *file = fopen("test_nop.ovmbc", "wb");
    if (!file)
    {
        printf("Unable to open file!");
        return 1;
    }
    char type[4] = "VMBC";
    unsigned char v1 = 0, v2 = 0, v3 = 1;
    unsigned int start_address = 0;
    unsigned char demo[] = {0xB0, 0x0, 0x0, 0x0, 0x1};
    fwrite(type, 4, 1, file);
    fwrite(&v1, 1, 1, file);
    fwrite(&v2, 1, 1, file);
    fwrite(&v3, 1, 1, file);
    fwrite(&start_address, 4, 1, file);
    fwrite(demo, 4, 1, file);
    fclose(file);
    return 0;
}