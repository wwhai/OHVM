#include "vm.h"

#include <limits.h>
#include <string.h>

static OhvmStatus fetch_u8(Ohvm *vm, uint8_t *value)
{
    if (vm->ip >= vm->code_size) {
        return OHVM_IP_OUT_OF_BOUNDS;
    }
    *value = vm->code[vm->ip++];
    return OHVM_OK;
}

static OhvmStatus fetch_u16(Ohvm *vm, uint16_t *value)
{
    uint8_t low;
    uint8_t high;
    OhvmStatus status = fetch_u8(vm, &low);
    if (status != OHVM_OK) {
        return status;
    }
    status = fetch_u8(vm, &high);
    if (status != OHVM_OK) {
        return status;
    }
    *value = (uint16_t)(low | ((uint16_t)high << 8));
    return OHVM_OK;
}

static OhvmStatus fetch_i32(Ohvm *vm, int32_t *value)
{
    uint32_t bits = 0;
    for (unsigned int shift = 0; shift < 32; shift += 8) {
        uint8_t byte;
        OhvmStatus status = fetch_u8(vm, &byte);
        if (status != OHVM_OK) {
            return status;
        }
        bits |= (uint32_t)byte << shift;
    }
    memcpy(value, &bits, sizeof(*value));
    return OHVM_OK;
}

static OhvmStatus push(Ohvm *vm, int32_t value)
{
    if (vm->stack_size == OHVM_STACK_CAPACITY) {
        return OHVM_STACK_OVERFLOW;
    }
    vm->stack[vm->stack_size++] = value;
    return OHVM_OK;
}

static OhvmStatus pop(Ohvm *vm, int32_t *value)
{
    if (vm->stack_size == 0) {
        return OHVM_STACK_UNDERFLOW;
    }
    *value = vm->stack[--vm->stack_size];
    return OHVM_OK;
}

static OhvmStatus binary_operands(Ohvm *vm, int32_t *left, int32_t *right)
{
    if (vm->stack_size < 2) {
        return OHVM_STACK_UNDERFLOW;
    }
    *right = vm->stack[--vm->stack_size];
    *left = vm->stack[--vm->stack_size];
    return OHVM_OK;
}

static OhvmStatus jump_to(Ohvm *vm, uint16_t target)
{
    if (target >= vm->code_size) {
        return OHVM_BAD_JUMP;
    }
    vm->ip = target;
    return OHVM_OK;
}

void ohvm_init(Ohvm *vm)
{
    memset(vm, 0, sizeof(*vm));
    vm->output = stdout;
}

OhvmStatus ohvm_load_program(Ohvm *vm, const uint8_t *code, size_t size,
                             uint16_t entry)
{
    if (size == 0 || entry >= size) {
        return OHVM_BAD_FORMAT;
    }
    if (size > OHVM_CODE_CAPACITY) {
        return OHVM_PROGRAM_TOO_LARGE;
    }
    ohvm_init(vm);
    memcpy(vm->code, code, size);
    vm->code_size = size;
    vm->ip = entry;
    return OHVM_OK;
}

OhvmStatus ohvm_load_file(Ohvm *vm, const char *path)
{
    uint8_t header[OHVM_HEADER_SIZE];
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return OHVM_IO_ERROR;
    }
    if (fread(header, 1, sizeof(header), file) != sizeof(header) ||
        memcmp(header, "OHVM", 4) != 0 || header[4] != OHVM_VERSION ||
        header[5] != 0) {
        fclose(file);
        return OHVM_BAD_FORMAT;
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return OHVM_IO_ERROR;
    }
    long file_size = ftell(file);
    if (file_size < OHVM_HEADER_SIZE) {
        fclose(file);
        return OHVM_BAD_FORMAT;
    }
    size_t code_size = (size_t)file_size - OHVM_HEADER_SIZE;
    if (code_size == 0 || code_size > OHVM_CODE_CAPACITY) {
        fclose(file);
        return code_size == 0 ? OHVM_BAD_FORMAT : OHVM_PROGRAM_TOO_LARGE;
    }
    if (fseek(file, OHVM_HEADER_SIZE, SEEK_SET) != 0 ||
        fread(vm->code, 1, code_size, file) != code_size) {
        fclose(file);
        return OHVM_IO_ERROR;
    }
    fclose(file);

    uint16_t entry = (uint16_t)(header[6] | ((uint16_t)header[7] << 8));
    if (entry >= code_size) {
        return OHVM_BAD_FORMAT;
    }
    memset(vm->stack, 0, sizeof(vm->stack));
    memset(vm->globals, 0, sizeof(vm->globals));
    memset(vm->calls, 0, sizeof(vm->calls));
    vm->code_size = code_size;
    vm->ip = entry;
    vm->stack_size = 0;
    vm->call_size = 0;
    vm->halted = false;
    vm->trace = false;
    vm->output = stdout;
    return OHVM_OK;
}

OhvmStatus ohvm_step(Ohvm *vm)
{
    uint16_t instruction_ip = vm->ip;
    uint8_t opcode;
    OhvmStatus status = fetch_u8(vm, &opcode);
    if (status != OHVM_OK) {
        return status;
    }
    if (vm->trace) {
        fprintf(stderr, "%04x  %02x  stack=%zu\n", instruction_ip, opcode,
                vm->stack_size);
    }

    int32_t left;
    int32_t right;
    uint8_t slot;
    uint16_t target;

    switch ((OhvmOpcode)opcode) {
    case OP_HALT:
        vm->halted = true;
        return OHVM_OK;
    case OP_PUSH:
        status = fetch_i32(vm, &left);
        return status == OHVM_OK ? push(vm, left) : status;
    case OP_POP:
        return pop(vm, &left);
    case OP_DUP:
        if (vm->stack_size == 0) {
            return OHVM_STACK_UNDERFLOW;
        }
        return push(vm, vm->stack[vm->stack_size - 1]);
    case OP_LOAD:
        status = fetch_u8(vm, &slot);
        return status == OHVM_OK ? push(vm, vm->globals[slot]) : status;
    case OP_STORE:
        status = fetch_u8(vm, &slot);
        if (status != OHVM_OK) {
            return status;
        }
        status = pop(vm, &left);
        if (status == OHVM_OK) {
            vm->globals[slot] = left;
        }
        return status;
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV:
    case OP_MOD:
    case OP_EQ:
    case OP_LT:
    case OP_GT:
        status = binary_operands(vm, &left, &right);
        if (status != OHVM_OK) {
            return status;
        }
        if ((opcode == OP_DIV || opcode == OP_MOD) && right == 0) {
            return OHVM_DIVIDE_BY_ZERO;
        }
        switch ((OhvmOpcode)opcode) {
        case OP_ADD:
            return push(vm, (int32_t)((uint32_t)left + (uint32_t)right));
        case OP_SUB:
            return push(vm, (int32_t)((uint32_t)left - (uint32_t)right));
        case OP_MUL:
            return push(vm, (int32_t)((uint32_t)left * (uint32_t)right));
        case OP_DIV:
            return push(vm, left == INT32_MIN && right == -1
                                ? INT32_MIN : left / right);
        case OP_MOD:
            return push(vm, left == INT32_MIN && right == -1
                                ? 0 : left % right);
        case OP_EQ:
            return push(vm, left == right);
        case OP_LT:
            return push(vm, left < right);
        case OP_GT:
            return push(vm, left > right);
        default:
            return OHVM_UNKNOWN_OPCODE;
        }
    case OP_JMP:
        status = fetch_u16(vm, &target);
        return status == OHVM_OK ? jump_to(vm, target) : status;
    case OP_JZ:
        status = fetch_u16(vm, &target);
        if (status != OHVM_OK) {
            return status;
        }
        status = pop(vm, &left);
        return status != OHVM_OK || left != 0 ? status : jump_to(vm, target);
    case OP_CALL:
        status = fetch_u16(vm, &target);
        if (status != OHVM_OK) {
            return status;
        }
        if (vm->call_size == OHVM_CALL_CAPACITY) {
            return OHVM_CALL_OVERFLOW;
        }
        if (target >= vm->code_size) {
            return OHVM_BAD_JUMP;
        }
        vm->calls[vm->call_size++] = vm->ip;
        vm->ip = target;
        return OHVM_OK;
    case OP_RET:
        if (vm->call_size == 0) {
            return OHVM_CALL_UNDERFLOW;
        }
        vm->ip = vm->calls[--vm->call_size];
        return OHVM_OK;
    case OP_PRINT:
        status = pop(vm, &left);
        if (status == OHVM_OK) {
            fprintf(vm->output, "%d\n", left);
        }
        return status;
    default:
        return OHVM_UNKNOWN_OPCODE;
    }
}

OhvmStatus ohvm_run(Ohvm *vm, size_t max_steps)
{
    for (size_t step = 0; step < max_steps; ++step) {
        OhvmStatus status = ohvm_step(vm);
        if (status != OHVM_OK || vm->halted) {
            return status;
        }
    }
    return OHVM_STEP_LIMIT;
}

const char *ohvm_status_name(OhvmStatus status)
{
    static const char *const names[] = {
        "ok", "I/O error", "invalid bytecode", "program too large",
        "instruction pointer out of bounds", "unknown opcode",
        "stack overflow", "stack underflow", "call stack overflow",
        "call stack underflow", "division by zero", "invalid jump",
        "step limit reached"
    };
    return status <= OHVM_STEP_LIMIT ? names[status] : "unknown error";
}