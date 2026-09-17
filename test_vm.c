#include "vm.h"

#include <stdio.h>
#include <stdlib.h>

#define CHECK(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        return 1; \
    } \
} while (0)

static int test_arithmetic(void)
{
    const uint8_t code[] = {
        OP_PUSH, 40, 0, 0, 0,
        OP_PUSH, 2, 0, 0, 0,
        OP_ADD,
        OP_HALT
    };
    Ohvm vm;
    CHECK(ohvm_load_program(&vm, code, sizeof(code), 0) == OHVM_OK);
    CHECK(ohvm_run(&vm, 20) == OHVM_OK);
    CHECK(vm.stack_size == 1);
    CHECK(vm.stack[0] == 42);
    return 0;
}

static int test_loop(void)
{
    const uint8_t code[] = {
        OP_PUSH, 3, 0, 0, 0,
        OP_STORE, 0,
        OP_LOAD, 0,
        OP_PUSH, 1, 0, 0, 0,
        OP_SUB,
        OP_DUP,
        OP_STORE, 0,
        OP_JZ, 24, 0,
        OP_JMP, 7, 0,
        OP_HALT
    };
    Ohvm vm;
    CHECK(ohvm_load_program(&vm, code, sizeof(code), 0) == OHVM_OK);
    CHECK(ohvm_run(&vm, 100) == OHVM_OK);
    CHECK(vm.globals[0] == 0);
    CHECK(vm.stack_size == 0);
    return 0;
}

static int test_call(void)
{
    const uint8_t code[] = {
        OP_CALL, 4, 0,
        OP_HALT,
        OP_PUSH, 42, 0, 0, 0,
        OP_RET
    };
    Ohvm vm;
    CHECK(ohvm_load_program(&vm, code, sizeof(code), 0) == OHVM_OK);
    CHECK(ohvm_run(&vm, 20) == OHVM_OK);
    CHECK(vm.stack_size == 1);
    CHECK(vm.stack[0] == 42);
    CHECK(vm.call_size == 0);
    return 0;
}

static int test_errors(void)
{
    const uint8_t bad_opcode[] = {0xff};
    const uint8_t divide_by_zero[] = {
        OP_PUSH, 1, 0, 0, 0,
        OP_PUSH, 0, 0, 0, 0,
        OP_DIV,
        OP_HALT
    };
    Ohvm vm;
    CHECK(ohvm_load_program(&vm, bad_opcode, sizeof(bad_opcode), 0) == OHVM_OK);
    CHECK(ohvm_run(&vm, 1) == OHVM_UNKNOWN_OPCODE);
    CHECK(ohvm_load_program(&vm, divide_by_zero, sizeof(divide_by_zero), 0) == OHVM_OK);
    CHECK(ohvm_run(&vm, 20) == OHVM_DIVIDE_BY_ZERO);
    return 0;
}

static int test_bytecode_file(void)
{
    static const char path[] = "test_program.ohvmbc";
    const uint8_t file_bytes[] = {
        'O', 'H', 'V', 'M', OHVM_VERSION, 0, 0, 0,
        OP_PUSH, 42, 0, 0, 0,
        OP_HALT
    };
    FILE *file = fopen(path, "wb");
    CHECK(file != NULL);
    CHECK(fwrite(file_bytes, 1, sizeof(file_bytes), file) == sizeof(file_bytes));
    CHECK(fclose(file) == 0);

    Ohvm vm;
    CHECK(ohvm_load_file(&vm, path) == OHVM_OK);
    CHECK(remove(path) == 0);
    CHECK(ohvm_run(&vm, 20) == OHVM_OK);
    CHECK(vm.stack_size == 1);
    CHECK(vm.stack[0] == 42);
    return 0;
}

int main(void)
{
    CHECK(test_arithmetic() == 0);
    CHECK(test_loop() == 0);
    CHECK(test_call() == 0);
    CHECK(test_errors() == 0);
    CHECK(test_bytecode_file() == 0);
    puts("all tests passed");
    return 0;
}