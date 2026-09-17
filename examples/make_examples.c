#include "../vm.h"

#include <stdint.h>
#include <stdio.h>

static int write_program(const char *path, const uint8_t *code, size_t size)
{
    const uint8_t header[OHVM_HEADER_SIZE] = {
        'O', 'H', 'V', 'M', OHVM_VERSION, 0, 0, 0
    };
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        fprintf(stderr, "cannot create %s\n", path);
        return 1;
    }
    int failed = fwrite(header, 1, sizeof(header), file) != sizeof(header) ||
                 fwrite(code, 1, size, file) != size || fclose(file) != 0;
    if (failed) {
        fprintf(stderr, "cannot write %s\n", path);
        return 1;
    }
    printf("created %s\n", path);
    return 0;
}

int main(void)
{
    const uint8_t answer[] = {
        OP_PUSH, 42, 0, 0, 0,
        OP_PRINT,
        OP_HALT
    };

    const uint8_t sum_1_to_100[] = {
        OP_PUSH, 0, 0, 0, 0,       /* sum = 0 */
        OP_STORE, 0,
        OP_PUSH, 1, 0, 0, 0,       /* i = 1 */
        OP_STORE, 1,
        OP_LOAD, 0,                 /* loop: sum += i */
        OP_LOAD, 1,
        OP_ADD,
        OP_STORE, 0,
        OP_LOAD, 1,                 /* ++i */
        OP_PUSH, 1, 0, 0, 0,
        OP_ADD,
        OP_DUP,
        OP_STORE, 1,
        OP_PUSH, 101, 0, 0, 0,     /* while (i < 101) */
        OP_LT,
        OP_JZ, 44, 0,
        OP_JMP, 14, 0,
        OP_LOAD, 0,
        OP_PRINT,
        OP_HALT
    };

    const uint8_t factorial[] = {
        OP_PUSH, 1, 0, 0, 0,       /* result = 1 */
        OP_STORE, 0,
        OP_PUSH, 5, 0, 0, 0,       /* n = 5 */
        OP_STORE, 1,
        OP_LOAD, 0,                 /* loop: result *= n */
        OP_LOAD, 1,
        OP_MUL,
        OP_STORE, 0,
        OP_LOAD, 1,                 /* --n */
        OP_PUSH, 1, 0, 0, 0,
        OP_SUB,
        OP_DUP,
        OP_STORE, 1,
        OP_JZ, 38, 0,
        OP_JMP, 14, 0,
        OP_LOAD, 0,
        OP_PRINT,
        OP_HALT
    };

    const uint8_t function_call[] = {
        OP_PUSH, 7, 0, 0, 0,
        OP_CALL, 10, 0,
        OP_PRINT,
        OP_HALT,
        OP_DUP,                     /* square(value) */
        OP_MUL,
        OP_RET
    };

    return write_program("examples/answer.ohvmbc", answer, sizeof(answer)) ||
           write_program("examples/sum_1_to_100.ohvmbc", sum_1_to_100,
                         sizeof(sum_1_to_100)) ||
           write_program("examples/factorial.ohvmbc", factorial,
                         sizeof(factorial)) ||
           write_program("examples/function_call.ohvmbc", function_call,
                         sizeof(function_call));
}