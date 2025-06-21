/** !
 * x86_64 code generator 
 * 
 * @file code_gen/x86_64.h 
 * 
 * @author Jacob Smith 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <time.h>

// POSIX
#include <sys/mman.h>

// log module
#include <log/log.h>

// sync module
#include <sync/sync.h>

// pack module
#include <pack/pack.h>

// code gen
#include <code_gen/code_gen.h>

// enumeration definitions
enum x86_64_op_e 
{
    X86_64_REG_8,
    X86_64_REG_16,
    X86_64_REG_32,
    X86_64_REG_64,
    X86_64_REG_128,
    X86_64_REG_256,
    X86_64_REG_512,
    X86_64_IMM64,
};

enum x86_64_reg_e
{
    RAX = 0b000,
    RCX = 0b001,
    RDX = 0b010,
    RBX = 0b011,
    RSP = 0b100,
    RBP = 0b101,
    RSI = 0b110,    
    RDI = 0b111,
    XMM0 = 0b000,
    XMM1 = 0b001,
    XMM2 = 0b010,
    XMM3 = 0b011,
    XMM4 = 0b100,
    XMM5 = 0b101,
    XMM6 = 0b110,
    XMM7 = 0b111,
    YMM0 = 0b000,
    YMM1 = 0b001,
    YMM2 = 0b010,
    YMM3 = 0b011,
    YMM4 = 0b100,
    YMM5 = 0b101,
    YMM6 = 0b110,
    YMM7 = 0b111,
    ZMM0 = 0b000,
    ZMM1 = 0b001,
    ZMM2 = 0b010,
    ZMM3 = 0b011,
    ZMM4 = 0b100,
    ZMM5 = 0b101,
    ZMM6 = 0b110,
    ZMM7 = 0b111
};

// structure definitions
struct x86_64_code_gen_s
{
    size_t         size;
    void          *p_base;
    unsigned char *p_offset;
};

struct x86_64_op_s
{
    enum x86_64_op_e _type;

    union
    {
        enum x86_64_reg_e  _reg;
        unsigned long long _imm64;
    };
};


// type definitions
typedef struct x86_64_op_s       x86_64_op;
typedef struct x86_64_code_gen_s x86_64_code_gen;

// constructors
int x86_64_code_gen_construct ( x86_64_code_gen **pp_x86_64_code_gen, void *p_out, size_t size );
int x86_64_code_gen_node_construct ( x86_64_code_gen *p_code_gen, char *instruction, char *op1, char *op2, char *op3 );

// Stack
size_t x86_64_gen_push_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_pop_reg  ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );

// Bitwise
size_t x86_64_gen_and_r64_r64  ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
size_t x86_64_gen_or_r64_r64   ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
size_t x86_64_gen_xor_r64_r64  ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
size_t x86_64_gen_test_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
size_t x86_64_gen_not_reg      ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg ); 
size_t x86_64_gen_shl_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 );
size_t x86_64_gen_shr_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 );
size_t x86_64_gen_rol_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 );
size_t x86_64_gen_ror_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 );

// Arithmetic
size_t x86_64_gen_add_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
size_t x86_64_gen_sub_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
size_t x86_64_gen_neg_reg     ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_mul_reg     ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_imul_reg    ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_div_reg     ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_idiv_reg    ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_inc_reg     ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_dec_reg     ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );

// Flow control
size_t x86_64_gen_jmp_reg  ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_call_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg );
size_t x86_64_gen_ret      ( x86_64_code_gen *p_code_gen );

// Move
size_t x86_64_gen_mov_reg_imm64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, long long imm64 );
size_t x86_64_gen_mov_r64_r64   ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );

// Misc
size_t x86_64_gen_nop     ( x86_64_code_gen *p_code_gen );
size_t x86_64_gen_syscall ( x86_64_code_gen *p_code_gen );

// AVX
size_t x86_64_avx_gen_add_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 );
