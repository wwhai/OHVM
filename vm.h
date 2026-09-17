#ifndef OHVM_VM_H
#define OHVM_VM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define OHVM_VERSION 1
#define OHVM_HEADER_SIZE 8
#define OHVM_CODE_CAPACITY 4096
#define OHVM_STACK_CAPACITY 256
#define OHVM_GLOBAL_COUNT 256
#define OHVM_CALL_CAPACITY 64

typedef enum {
    OP_HALT = 0x00,
    OP_PUSH = 0x01,
    OP_POP = 0x02,
    OP_DUP = 0x03,
    OP_LOAD = 0x04,
    OP_STORE = 0x05,
    OP_ADD = 0x06,
    OP_SUB = 0x07,
    OP_MUL = 0x08,
    OP_DIV = 0x09,
    OP_MOD = 0x0a,
    OP_EQ = 0x0b,
    OP_LT = 0x0c,
    OP_GT = 0x0d,
    OP_JMP = 0x0e,
    OP_JZ = 0x0f,
    OP_CALL = 0x10,
    OP_RET = 0x11,
    OP_PRINT = 0x12
} OhvmOpcode;

typedef enum {
    OHVM_OK = 0,
    OHVM_IO_ERROR,
    OHVM_BAD_FORMAT,
    OHVM_PROGRAM_TOO_LARGE,
    OHVM_IP_OUT_OF_BOUNDS,
    OHVM_UNKNOWN_OPCODE,
    OHVM_STACK_OVERFLOW,
    OHVM_STACK_UNDERFLOW,
    OHVM_CALL_OVERFLOW,
    OHVM_CALL_UNDERFLOW,
    OHVM_DIVIDE_BY_ZERO,
    OHVM_BAD_JUMP,
    OHVM_STEP_LIMIT
} OhvmStatus;

typedef struct {
    uint8_t code[OHVM_CODE_CAPACITY];
    size_t code_size;
    uint16_t ip;
    int32_t stack[OHVM_STACK_CAPACITY];
    size_t stack_size;
    int32_t globals[OHVM_GLOBAL_COUNT];
    uint16_t calls[OHVM_CALL_CAPACITY];
    size_t call_size;
    bool halted;
    bool trace;
    FILE *output;
} Ohvm;

void ohvm_init(Ohvm *vm);
OhvmStatus ohvm_load_program(Ohvm *vm, const uint8_t *code, size_t size,
                             uint16_t entry);
OhvmStatus ohvm_load_file(Ohvm *vm, const char *path);
OhvmStatus ohvm_step(Ohvm *vm);
OhvmStatus ohvm_run(Ohvm *vm, size_t max_steps);
const char *ohvm_status_name(OhvmStatus status);

#endif