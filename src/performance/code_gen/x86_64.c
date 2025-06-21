/** !
 * x86_64 code generator 
 * 
 * @file x86_64.c
 * 
 * @author Jacob Smith 
 */

// header
#include <code_gen/x86_64.h>

// Preprocessor macros
#define X86_64_REG_QUANTITY 8

// function definitions

///////////
// Stack //
///////////
size_t x86_64_gen_push_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%i8",
        0x50 | _reg
    );
}

size_t x86_64_gen_pop_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%i8",
        0x58 | _reg
    );
}

/////////////
// Bitwise //
/////////////
size_t x86_64_gen_and_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x21,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_gen_or_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x09,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_gen_xor_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x31,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_gen_test_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x85,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_gen_not_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xF7,
        0xD0 | _reg
    );
}

size_t x86_64_gen_shl_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0x48,
        0xC1,
        0xE0 | _reg,
        imm8
    );
}

size_t x86_64_gen_shr_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0x48,
        0xC1,
        0xE8 | _reg,
        imm8
    );
}

size_t x86_64_gen_rol_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0x48,
        0xC1,
        0xC0 | _reg,
        imm8
    );
}

size_t x86_64_gen_ror_reg_imm8 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, unsigned char imm8 )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0x48,
        0xC1,
        0xC8 | _reg,
        imm8
    );
}

////////////////
// Arithmetic //
////////////////
size_t x86_64_gen_add_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x01,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_gen_sub_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x29,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_gen_neg_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xF7,
        0xD8 | _reg
    );
}

size_t x86_64_gen_mul_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xF7,
        0xE0 | _reg
    );
}

size_t x86_64_gen_imul_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xF7,
        0xE8 | _reg
    );
}

size_t x86_64_gen_div_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xF7,
        0xF0 | _reg
    );
}

size_t x86_64_gen_idiv_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xF7,
        0xF8 | _reg
    );
}

size_t x86_64_gen_inc_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xFF,
        0xC0 | _reg
    );
}

size_t x86_64_gen_dec_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0xFF,
        0xC8 | _reg
    );
}

//////////////////
// Flow Control //
//////////////////
size_t x86_64_gen_jmp_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%2i8",
        0xFF,
        0xE0 | _reg
    );
}

size_t x86_64_gen_call_reg ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%2i8",
        0xFF,
        0xD0 | _reg
    );
}

size_t x86_64_gen_ret ( x86_64_code_gen *p_code_gen )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%i8", 0xC3);
}

//////////
// Move //
//////////
size_t x86_64_gen_mov_reg_imm64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg, long long imm64 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%2i8%i64",
        0x48,
        0xB8 | _reg,
        imm64
    );
}

size_t x86_64_gen_mov_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x89,
        0xC0 | _reg2 << 3 | _reg1
    );
}

size_t x86_64_avx2_gen_mov_r64_r64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    
    // success
    return pack_pack(p_code_gen->p_offset, "%3i8",
        0x48,
        0x89,
        0xC0 | _reg2 << 3 | _reg1
    );
}


//////////
// Misc //
//////////
size_t x86_64_gen_nop ( x86_64_code_gen *p_code_gen )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%i8", 0x90);
}

size_t x86_64_gen_syscall ( x86_64_code_gen *p_code_gen )
{

    // success
    return pack_pack(p_code_gen->p_offset, "%2i8",
        0x0F,
        0x05
    );
}

size_t x86_64_avx_gen_mov_reg128_reg64 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2 )
{
    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xF9,
        0x6F,
        0xC0 | _reg1 << 3
             | _reg2
    );
}

size_t x86_64_avx_gen_add_i32x4_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC1 | b << 3,
        0xFE,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_add_f32x8_reg256_reg256_reg256 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC1 | b << 3,
        0x58,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_add_i64x2_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC0 | b << 3,
        0x58,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_add_f64x2_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC1 | b << 3,
        0x58,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_add_f32x4_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC0 | b << 3,
        0x58,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_sub_f32x4_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC0 | b << 3,
        0x5C,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_mul_f32x4_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC0 | b << 3,
        0x59,
        0xC0 | a << 3
             | c
    );
}

size_t x86_64_avx_gen_div_f32x4_reg128_reg128_reg128 ( x86_64_code_gen *p_code_gen, enum x86_64_reg_e _reg1, enum x86_64_reg_e _reg2, enum x86_64_reg_e _reg3 )
{

    unsigned char a = _reg1,
                  b = (~_reg2) & 0b111,
                  c = _reg3;

    // success
    return pack_pack(p_code_gen->p_offset, "%4i8",
        0xC5,
        0xC0 | b << 3,
        0x5E,
        0xC0 | a << 3
             | c
    );
}
// function definitions
int x86_64_code_gen_construct ( x86_64_code_gen **pp_code_gen, void *p_out, size_t size )
{

    // argument check
    if ( pp_code_gen == (void *) 0 ) goto no_x86_64_code_gen;
    if ( size        ==          0 ) goto no_size;

    // initialized data
    x86_64_code_gen *p_code_gen = malloc(size + sizeof(x86_64_code_gen));

    // error check
    if ( p_code_gen == (void *) 0 ) goto no_mem;

    // Store the size of the pool
    p_code_gen->size = size;

    // Allocate an executable page
    p_code_gen->p_base = mmap(NULL, 8192, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // error check
    if ( (void *) -1 == p_code_gen->p_base) goto no_mem; 
    
    // Store the offset
    p_code_gen->p_offset = p_code_gen->p_base;
    
    /*
    ///////////////////////////
    // Assemble the function //
    ///////////////////////////
    {

        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        // inc
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_inc_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
            
        // dec
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_dec_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // push
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_push_reg(p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // pop
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_pop_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
    }

    /////////////
    // Bitwise //
    /////////////
    {

        // not
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_not_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
            
        // and
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_and_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // or
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_or_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // xor
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_xor_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // test
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_test_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // shl
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_shl_reg_imm8 (p_code_gen, (enum x86_64_reg_e) i, i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // shr
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_shr_reg_imm8 (p_code_gen, (enum x86_64_reg_e) i, i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // rol
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_rol_reg_imm8 (p_code_gen, (enum x86_64_reg_e) i, i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);

        // ror
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_ror_reg_imm8 (p_code_gen, (enum x86_64_reg_e) i, i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
    }
    
    ////////////////
    // Arithmetic //
    ////////////////
    {

        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // add
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_add_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // sub
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_sub_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // neg
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_neg_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // mul
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_mul_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // imul
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_imul_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // div
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_div_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);

        // idiv
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_idiv_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
    }

    //////////
    // Jump //
    //////////
    {
        
        // jmp
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_jmp_reg (p_code_gen, (enum x86_64_reg_e) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
        // call
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_call_reg (p_code_gen, (enum x86_64_reg_e) i);

        // ret
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
    }

    //////////
    // Misc //
    //////////
    {
        // nop
        p_code_gen->p_offset += x86_64_gen_nop(p_code_gen);
        
        // ret
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
    }

    //////////
    // Move //
    //////////
    {
       
        // mov
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_mov_reg_imm64 (p_code_gen, (enum x86_64_reg_e) i, (long long) i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
            
        // mov
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            p_code_gen->p_offset += x86_64_gen_mov_reg_imm64 (p_code_gen, (enum x86_64_reg_e) i, (long long) -i);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        // mov
        for (size_t i = 0; i < X86_64_REG_QUANTITY; i++)
            for (size_t j = 0; j < X86_64_REG_QUANTITY; j++)
                p_code_gen->p_offset += x86_64_gen_mov_r64_r64 (p_code_gen, (enum x86_64_reg_e) i, (enum x86_64_reg_e) j);
        p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        
    }
    */

    // // Update page permissions 
    // if ( -1 == mprotect(p_code_gen->p_base, (size_t)sysconf(_SC_PAGESIZE), PROT_READ | PROT_EXEC) ) goto failed_to_update_permissions;

    // Set the size
    p_code_gen->size = ((size_t)p_code_gen->p_offset - (size_t)p_code_gen->p_base);

    // Output (for ndisasm) 
    if ( p_out )
        memcpy(p_out, p_code_gen->p_base, p_code_gen->size);

    // Store the allocator
    *pp_code_gen = p_code_gen;

    // success
    return 1;

    // error checking
    {
        
        // argument errors
        {
            no_x86_64_code_gen:
                #ifndef NDEBUG
                    log_error("[allocator] [linear] Null pointer provided for parameter \"p_code_gen\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_size:
                #ifndef NDEBUG
                    log_error("[allocator] [linear] Null pointer provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // POSIX errors
        {
            failed_to_update_permissions:
                #ifndef NDEBUG
                    log_error("[sys] [mman] Call to \"mprotect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x86_64_code_gen_node_construct ( x86_64_code_gen *p_code_gen, char *instruction, char *op1, char *op2, char *op3 ) 
{

    // initialized data
    x86_64_op _op1, _op2, _op3;

    // Parse operand 1 
    if ( op1 )
    {
             if ( 0 == strncmp(op1, "RAX" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RAX };
        else if ( 0 == strncmp(op1, "RCX" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RCX };
        else if ( 0 == strncmp(op1, "RDX" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RDX };
        else if ( 0 == strncmp(op1, "RBX" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RBX };
        else if ( 0 == strncmp(op1, "RSP" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RSP };
        else if ( 0 == strncmp(op1, "RBP" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RBP };
        else if ( 0 == strncmp(op1, "RSI" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RSI };
        else if ( 0 == strncmp(op1, "RDI" , 3) ) _op1 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RDI };

        else if ( 0 == strncmp(op1, "XMM0", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM0 };
        else if ( 0 == strncmp(op1, "XMM1", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM1 };
        else if ( 0 == strncmp(op1, "XMM2", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM2 };
        else if ( 0 == strncmp(op1, "XMM3", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM3 };
        else if ( 0 == strncmp(op1, "XMM4", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM4 };
        else if ( 0 == strncmp(op1, "XMM5", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM5 };
        else if ( 0 == strncmp(op1, "XMM6", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM6 };
        else if ( 0 == strncmp(op1, "XMM7", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM7 };

        else if ( 0 == strncmp(op1, "YMM0", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM0 };
        else if ( 0 == strncmp(op1, "YMM1", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM1 };
        else if ( 0 == strncmp(op1, "YMM2", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM2 };
        else if ( 0 == strncmp(op1, "YMM3", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM3 };
        else if ( 0 == strncmp(op1, "YMM4", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM4 };
        else if ( 0 == strncmp(op1, "YMM5", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM5 };
        else if ( 0 == strncmp(op1, "YMM6", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM6 };
        else if ( 0 == strncmp(op1, "YMM7", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM7 };

        else if ( 0 == strncmp(op1, "ZMM0", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM0 };
        else if ( 0 == strncmp(op1, "ZMM1", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM1 };
        else if ( 0 == strncmp(op1, "ZMM2", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM2 };
        else if ( 0 == strncmp(op1, "ZMM3", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM3 };
        else if ( 0 == strncmp(op1, "ZMM4", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM4 };
        else if ( 0 == strncmp(op1, "ZMM5", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM5 };
        else if ( 0 == strncmp(op1, "ZMM6", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM6 };
        else if ( 0 == strncmp(op1, "ZMM7", 4) ) _op1 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM7 };

        else                                     _op1 = (x86_64_op) { ._type = X86_64_IMM64  , ._imm64 = atoi(op1) };
    }

    // Parse operand 2 
    if ( op2 )
    {
             if ( 0 == strncmp(op2, "RAX" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RAX };
        else if ( 0 == strncmp(op2, "RCX" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RCX };
        else if ( 0 == strncmp(op2, "RDX" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RDX };
        else if ( 0 == strncmp(op2, "RBX" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RBX };
        else if ( 0 == strncmp(op2, "RSP" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RSP };
        else if ( 0 == strncmp(op2, "RBP" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RBP };
        else if ( 0 == strncmp(op2, "RSI" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RSI };
        else if ( 0 == strncmp(op2, "RDI" , 3) ) _op2 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RDI };

        else if ( 0 == strncmp(op2, "XMM0", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM0 };
        else if ( 0 == strncmp(op2, "XMM1", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM1 };
        else if ( 0 == strncmp(op2, "XMM2", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM2 };
        else if ( 0 == strncmp(op2, "XMM3", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM3 };
        else if ( 0 == strncmp(op2, "XMM4", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM4 };
        else if ( 0 == strncmp(op2, "XMM5", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM5 };
        else if ( 0 == strncmp(op2, "XMM6", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM6 };
        else if ( 0 == strncmp(op2, "XMM7", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM7 };

        else if ( 0 == strncmp(op2, "YMM0", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM0 };
        else if ( 0 == strncmp(op2, "YMM1", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM1 };
        else if ( 0 == strncmp(op2, "YMM2", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM2 };
        else if ( 0 == strncmp(op2, "YMM3", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM3 };
        else if ( 0 == strncmp(op2, "YMM4", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM4 };
        else if ( 0 == strncmp(op2, "YMM5", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM5 };
        else if ( 0 == strncmp(op2, "YMM6", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM6 };
        else if ( 0 == strncmp(op2, "YMM7", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM7 };

        else if ( 0 == strncmp(op2, "ZMM0", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM0 };
        else if ( 0 == strncmp(op2, "ZMM1", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM1 };
        else if ( 0 == strncmp(op2, "ZMM2", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM2 };
        else if ( 0 == strncmp(op2, "ZMM3", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM3 };
        else if ( 0 == strncmp(op2, "ZMM4", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM4 };
        else if ( 0 == strncmp(op2, "ZMM5", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM5 };
        else if ( 0 == strncmp(op2, "ZMM6", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM6 };
        else if ( 0 == strncmp(op2, "ZMM7", 4) ) _op2 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM7 };

        else                                     _op2 = (x86_64_op) { ._type = X86_64_IMM64  , ._imm64 = atoi(op2) };

    }

    // Parse operand 3 
    if ( op3 )
    {
             if ( 0 == strncmp(op3, "RAX" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RAX };
        else if ( 0 == strncmp(op3, "RCX" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RCX };
        else if ( 0 == strncmp(op3, "RDX" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RDX };
        else if ( 0 == strncmp(op3, "RBX" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RBX };
        else if ( 0 == strncmp(op3, "RSP" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RSP };
        else if ( 0 == strncmp(op3, "RBP" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RBP };
        else if ( 0 == strncmp(op3, "RSI" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RSI };
        else if ( 0 == strncmp(op3, "RDI" , 3) ) _op3 = (x86_64_op) { ._type = X86_64_REG_64 , ._reg   = RDI };

        else if ( 0 == strncmp(op3, "XMM0", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM0 };
        else if ( 0 == strncmp(op3, "XMM1", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM1 };
        else if ( 0 == strncmp(op3, "XMM2", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM2 };
        else if ( 0 == strncmp(op3, "XMM3", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM3 };
        else if ( 0 == strncmp(op3, "XMM4", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM4 };
        else if ( 0 == strncmp(op3, "XMM5", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM5 };
        else if ( 0 == strncmp(op3, "XMM6", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM6 };
        else if ( 0 == strncmp(op3, "XMM7", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_128, ._reg   = XMM7 };

        else if ( 0 == strncmp(op3, "YMM0", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM0 };
        else if ( 0 == strncmp(op3, "YMM1", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM1 };
        else if ( 0 == strncmp(op3, "YMM2", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM2 };
        else if ( 0 == strncmp(op3, "YMM3", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM3 };
        else if ( 0 == strncmp(op3, "YMM4", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM4 };
        else if ( 0 == strncmp(op3, "YMM5", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM5 };
        else if ( 0 == strncmp(op3, "YMM6", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM6 };
        else if ( 0 == strncmp(op3, "YMM7", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_256, ._reg   = YMM7 };

        else if ( 0 == strncmp(op3, "ZMM0", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM0 };
        else if ( 0 == strncmp(op3, "ZMM1", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM1 };
        else if ( 0 == strncmp(op3, "ZMM2", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM2 };
        else if ( 0 == strncmp(op3, "ZMM3", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM3 };
        else if ( 0 == strncmp(op3, "ZMM4", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM4 };
        else if ( 0 == strncmp(op3, "ZMM5", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM5 };
        else if ( 0 == strncmp(op3, "ZMM6", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM6 };
        else if ( 0 == strncmp(op3, "ZMM7", 4) ) _op3 = (x86_64_op) { ._type = X86_64_REG_512, ._reg   = ZMM7 };

        else                                     _op3 = (x86_64_op) { ._type = X86_64_IMM64  , ._imm64 = atoi(op3) };
    }

    // No operands
    if ( !( op1 && op2 && op3 ) )
    {
             if ( 0 == strcmp(instruction, "RET")     ) p_code_gen->p_offset += x86_64_gen_ret(p_code_gen);
        else if ( 0 == strcmp(instruction, "NOP")     ) p_code_gen->p_offset += x86_64_gen_nop(p_code_gen);
        else if ( 0 == strcmp(instruction, "SYSCALL") ) p_code_gen->p_offset += x86_64_gen_syscall(p_code_gen);
    }

    // One operand
    if ( op1 && !( op2 && op3 ) )
    {
        if ( _op1._type == X86_64_REG_64 )
        {
                 if ( 0 == strcmp(instruction, "PUSH") ) p_code_gen->p_offset += x86_64_gen_push_reg(p_code_gen, _op1._reg);
            else if ( 0 == strcmp(instruction, "POP")  ) p_code_gen->p_offset += x86_64_gen_pop_reg(p_code_gen, _op1._reg);
            else if ( 0 == strcmp(instruction, "INC")  ) p_code_gen->p_offset += x86_64_gen_inc_reg(p_code_gen, _op1._reg);
            else if ( 0 == strcmp(instruction, "DEC")  ) p_code_gen->p_offset += x86_64_gen_dec_reg(p_code_gen, _op1._reg);
        } 
    }
    
    // Two operand
    if ( op1 && op2 && !op3 )
    {

        // reg, reg
        if ( _op1._type == X86_64_REG_64 && _op2._type == X86_64_REG_64 )
        {
                 if ( 0 == strcmp(instruction, "MOV")  ) p_code_gen->p_offset += x86_64_gen_mov_r64_r64(p_code_gen, _op1._reg, _op2._reg);
            else if ( 0 == strcmp(instruction, "AND")  ) p_code_gen->p_offset += x86_64_gen_and_r64_r64(p_code_gen, _op1._reg, _op2._reg);
            else if ( 0 == strcmp(instruction, "OR")   ) p_code_gen->p_offset += x86_64_gen_or_r64_r64(p_code_gen, _op1._reg, _op2._reg);
            else if ( 0 == strcmp(instruction, "XOR")  ) p_code_gen->p_offset += x86_64_gen_xor_r64_r64(p_code_gen, _op1._reg, _op2._reg);
            else if ( 0 == strcmp(instruction, "TEST") ) p_code_gen->p_offset += x86_64_gen_xor_r64_r64(p_code_gen, _op1._reg, _op2._reg);
            else if ( 0 == strcmp(instruction, "ADD")  ) p_code_gen->p_offset += x86_64_gen_add_r64_r64(p_code_gen, _op1._reg, _op2._reg);
            else if ( 0 == strcmp(instruction, "SUB")  ) p_code_gen->p_offset += x86_64_gen_sub_r64_r64(p_code_gen, _op1._reg, _op2._reg);
        } 

        // reg, imm64
        else if ( _op1._type == X86_64_REG_64 && _op2._type == X86_64_IMM64 )
        {
            if ( 0 == strcmp(instruction, "MOV") ) p_code_gen->p_offset += x86_64_gen_mov_reg_imm64(p_code_gen, _op1._reg, _op2._imm64);
        }

        else if ( _op1._type == X86_64_REG_128 && _op2._type == X86_64_REG_64 )
        {
            if ( 0 == strcmp(instruction, "VMOVDQA") ) p_code_gen->p_offset += x86_64_avx_gen_mov_reg128_reg64(p_code_gen, _op1._reg, _op2._reg);
        }
    }

    // Three operand
    if ( op1 && op2 && op3 )
    {

        // reg, reg, reg
        if ( _op1._type == X86_64_REG_128 && _op2._type == X86_64_REG_128 && _op3._type == X86_64_REG_128 )
        {
                 if ( 0 == strcmp(instruction, "VPADDD") ) p_code_gen->p_offset += x86_64_avx_gen_add_i32x4_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);
            else if ( 0 == strcmp(instruction, "VPADDQ") ) p_code_gen->p_offset += x86_64_avx_gen_add_i64x2_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);

            else if ( 0 == strcmp(instruction, "VADDPS") ) p_code_gen->p_offset += x86_64_avx_gen_add_f32x4_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);
            else if ( 0 == strcmp(instruction, "VSUBPS") ) p_code_gen->p_offset += x86_64_avx_gen_sub_f32x4_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);
            else if ( 0 == strcmp(instruction, "VMULPS") ) p_code_gen->p_offset += x86_64_avx_gen_mul_f32x4_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);
            else if ( 0 == strcmp(instruction, "VDIVPS") ) p_code_gen->p_offset += x86_64_avx_gen_div_f32x4_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);

            else if ( 0 == strcmp(instruction, "VADDPD") ) p_code_gen->p_offset += x86_64_avx_gen_add_f64x2_reg128_reg128_reg128(p_code_gen, _op1._reg, _op2._reg, _op3._reg);
        } 

        // reg, reg, reg
        if ( _op1._type == X86_64_REG_256 && _op2._type == X86_64_REG_256 && _op3._type == X86_64_REG_256 )
        {
                 if ( 0 == strcmp(instruction, "VADDPS") ) p_code_gen->p_offset += x86_64_avx_gen_add_f32x8_reg256_reg256_reg256(p_code_gen, _op1._reg, _op2._reg, _op3._reg);
        } 

    }

    // success
    return 1;
}