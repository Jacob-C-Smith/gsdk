/** ! 
 * Ed25519 implementation
 * 
 * @file ed25519.c
 * 
 * @author Jacob Smith
 */

// header
#include <core/ed25519.h>

// core
#include <core/log.h>
#include <core/interfaces.h>

// type definitions
typedef unsigned _BitInt(256) i256;
typedef unsigned _BitInt(512) i512;

// Helper to construct 256-bit integers
static i256 make_i256(unsigned long long p3, unsigned long long p2, unsigned long long p1, unsigned long long p0) {
    return ((i256)p3 << 192) | ((i256)p2 << 128) | ((i256)p1 << 64) | (i256)p0;
}

// q = 2^255 - 19
static i256 get_q() {
    return make_i256(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFEDULL);
}

// l = 2^252 + 27742317777372353535851937790883648493
static i256 get_l() {
    return make_i256(1ULL << 60, 0, 0x14DEF9DEA2F79CD6ULL, 0x5812631A5CF5D3EDULL);
}

// d = -121665 * inv(121666) mod q
static i256 get_d() {
    return make_i256(0x52036cee2b6ffe73ULL, 0x8cc740797779e898ULL, 0x00700a4d4141d8abULL, 0x75eb4dca135978a3ULL);
}

// I = expmod(2, (q-1)/4, q)
static i256 get_I() {
    return make_i256(0x2b8324804fc1df0bULL, 0x2b4d00993dfbd7a7ULL, 0x2f431806ad2fe478ULL, 0xc4ee1b274a0ea0b0ULL);
}

// Bx
static i256 get_Bx() {
    return make_i256(0x216936d3cd6e53feULL, 0xc0a4e231fdd6dc5cULL, 0x692cc7609525a7b2ULL, 0xc9562d608f25d51aULL);
}

// By
static i256 get_By() {
    return make_i256(0x6666666666666666ULL, 0x6666666666666666ULL, 0x6666666666666666ULL, 0x6666666666666658ULL);
}

static i256 add_mod(i256 a, i256 b, i256 m) {
    return (a + b) % m;
}

static i256 sub_mod(i256 a, i256 b, i256 m) {
    return (a - b + m) % m;
}

static i256 mul_mod(i256 a, i256 b, i256 m) {
    return ((i512)a * b) % m;
}

static i256 sqr_mod(i256 a, i256 m) {
    return mul_mod(a, a, m);
}

// Modular exponentiation
i256 expmod(i256 b, i256 e, i256 m) {
    i256 t = 1;
    while (e > 0) {
        if (e % 2 == 1) t = mul_mod(t, b, m);
        b = sqr_mod(b, m);
        e /= 2;
    }
    return t;
}

// Modular inverse
i256 inv(i256 x) {
    i256 q_val = get_q();
    return expmod(x, q_val - 2, q_val);
}

// xrecover
i256 xrecover(i256 y) {
    i256 q_val = get_q();
    i256 d_val = get_d();
    
    // xx = (y^2 - 1) * inv(d*y^2 + 1)
    i256 y2 = sqr_mod(y, q_val);
    i256 num = sub_mod(y2, 1, q_val);
    i256 den = add_mod(mul_mod(d_val, y2, q_val), 1, q_val);
    
    i256 xx = mul_mod(num, inv(den), q_val);
    
    i256 x = expmod(xx, (q_val + 3) / 8, q_val);
    
    i256 x2 = sqr_mod(x, q_val);
    if (sub_mod(x2, xx, q_val) != 0) {
        x = mul_mod(x, get_I(), q_val);
    }
    
    if (x % 2 != 0) x = q_val - x;
    return x;
}

// Edwards point addition
void edwards(i256 P[2], i256 Q[2], i256 R[2]) {
    i256 x1 = P[0], y1 = P[1];
    i256 x2 = Q[0], y2 = Q[1];
    i256 d_val = get_d();
    i256 q_val = get_q();
    
    // x3 = (x1*y2 + x2*y1) / (1 + d*x1*x2*y1*y2)
    // y3 = (y1*y2 + x1*x2) / (1 - d*x1*x2*y1*y2)
    
    i256 x1y2 = mul_mod(x1, y2, q_val);
    i256 x2y1 = mul_mod(x2, y1, q_val);
    i256 y1y2 = mul_mod(y1, y2, q_val);
    i256 x1x2 = mul_mod(x1, x2, q_val);
    
    i256 temp = mul_mod(d_val, mul_mod(x1x2, y1y2, q_val), q_val);
    
    i256 x_num = add_mod(x1y2, x2y1, q_val);
    i256 x_den = add_mod(1, temp, q_val);
    
    i256 y_num = add_mod(y1y2, x1x2, q_val);
    i256 y_den = sub_mod(1, temp, q_val);
    
    R[0] = mul_mod(x_num, inv(x_den), q_val);
    R[1] = mul_mod(y_num, inv(y_den), q_val);
}

// Scalar multiplication
void scalarmult(i256 P[2], i256 e, i256 R[2]) {
    R[0] = 0; R[1] = 1; // Identity
    i256 Q[2] = { P[0], P[1] };
    
    while (e > 0) {
        if (e % 2 == 1) {
            i256 T[2];
            edwards(R, Q, T);
            R[0] = T[0]; R[1] = T[1];
        }
        i256 T[2];
        edwards(Q, Q, T);
        Q[0] = T[0]; Q[1] = T[1];
        e /= 2;
    }
}

// Helper to get bit at index
int bit(const unsigned char *h, int i) {
    return (h[i / 8] >> (i % 8)) & 1;
}

// Encoding
void encodeint(i256 y, unsigned char *out) {
    for (int i = 0; i < 32; i++) {
        out[i] = (unsigned char)(y >> (i * 8));
    }
}

void encodepoint(i256 P[2], unsigned char *out) {
    i256 x = P[0];
    i256 y = P[1];
    encodeint(y, out);
    if ((x & 1) == 1) {
        out[31] |= 0x80;
    }
}

// Decoding
i256 decodeint(const unsigned char *s) {
    i256 sum = 0;
    for (int i = 0; i < 32; i++) {
        sum |= ((i256)s[i]) << (i * 8);
    }
    return sum;
}

int decodepoint(const unsigned char *s, i256 P[2]) {
    i256 y = decodeint(s);
    // Clear the sign bit from y
    y &= ~(((i256)1) << 255); 
    
    i256 q_val = get_q();
    i256 x = xrecover(y);
    if ((x & 1) != ((s[31] >> 7) & 1)) {
        x = q_val - x;
    }
    
    // Check if on curve
    // -x^2 + y^2 - 1 - d*x^2*y^2 = 0
    i256 d_val = get_d();
    i256 x2 = sqr_mod(x, q_val);
    i256 y2 = sqr_mod(y, q_val);
    
    i256 term1 = sub_mod(0, x2, q_val); // -x^2
    i256 term2 = y2;
    i256 term3 = sub_mod(0, 1, q_val); // -1
    i256 term4 = sub_mod(0, mul_mod(d_val, mul_mod(x2, y2, q_val), q_val), q_val); // -d*x^2*y^2
    
    i256 lhs = add_mod(add_mod(term1, term2, q_val), add_mod(term3, term4, q_val), q_val);
    
    if (lhs != 0) return 0; // Not on curve
    
    P[0] = x;
    P[1] = y;
    return 1;
}

// H(m) -> SHA512
void H(const unsigned char *m, size_t len, unsigned char *out) {
    sha512_state state;
    sha512_construct(&state);
    sha512_update(&state, m, len);
    sha512_final(&state, out);
}

// Hint(m)
i256 Hint(const unsigned char *m, size_t len) {
    unsigned char h[64];
    H(m, len, h);
    
    i512 sum = 0;
    for (int i = 0; i < 64; i++) {
        sum |= ((i512)h[i]) << (i*8);
    }
    
    i256 l_val = get_l();
    i256 res = sum % l_val;
    return res;
}

// Public Key Derivation
void public_key_derive(const unsigned char *sk, unsigned char *pk) {
    unsigned char h[64];
    H(sk, 32, h);
    
    i256 a = decodeint(h);
    // Clamp
    a &= ~(((i256)7)); // clear low 3 bits
    a &= ~(((i256)1) << 255); // clear bit 255
    a |= ((i256)1) << 254; // set bit 254
    
    i256 B[2] = { get_Bx(), get_By() };
    i256 A[2];
    scalarmult(B, a, A);
    encodepoint(A, pk);
}

int ed25519_key_pair_construct(ed25519_public_key **pp_public_key, ed25519_private_key **pp_private_key) {
    if (pp_public_key == NULL || pp_private_key == NULL) return 0;
    
    *pp_public_key = default_allocator(0, sizeof(ed25519_public_key));
    *pp_private_key = default_allocator(0, sizeof(ed25519_private_key));
    
    if (*pp_public_key == NULL || *pp_private_key == NULL) {
        if (*pp_public_key) default_allocator(*pp_public_key, 0);
        if (*pp_private_key) default_allocator(*pp_private_key, 0);
        *pp_public_key = NULL;
        *pp_private_key = NULL;
        return 0;
    }
    
    // Generate random seed
    FILE *urandom = fopen("/dev/urandom", "rb");
    if (urandom == NULL) {
        #ifndef NDEBUG
            log_error("ed25519_key_pair_construct: Failed to open /dev/urandom");
        #endif
        default_allocator(*pp_public_key, 0);
        default_allocator(*pp_private_key, 0);
        *pp_public_key = NULL;
        *pp_private_key = NULL;
        return 0;
    }
    
    if (fread((*pp_private_key)->data, 1, 32, urandom) != 32) {
        #ifndef NDEBUG
            log_error("ed25519_key_pair_construct: Failed to read 32 bytes from /dev/urandom");
        #endif
        fclose(urandom);
        default_allocator(*pp_public_key, 0);
        default_allocator(*pp_private_key, 0);
        *pp_public_key = NULL;
        *pp_private_key = NULL;
        return 0;
    }
    fclose(urandom);
    
    public_key_derive((*pp_private_key)->data, (*pp_public_key)->data);
    return 1;
}

int ed25519_key_pair_from_seed(ed25519_public_key **pp_public_key, ed25519_private_key **pp_private_key, const unsigned char *seed) {
    if (pp_public_key == NULL || pp_private_key == NULL || seed == NULL) return 0;
    
        *pp_public_key = default_allocator(0, sizeof(ed25519_public_key));
        *pp_private_key = default_allocator(0, sizeof(ed25519_private_key));
        
        if (*pp_public_key == NULL || *pp_private_key == NULL) { // Add NULL check
            #ifndef NDEBUG
                log_error("ed25519_key_pair_from_seed: Failed to allocate public/private key structures");
            #endif
            if (*pp_public_key) default_allocator(*pp_public_key, 0);
            if (*pp_private_key) default_allocator(*pp_private_key, 0);
            *pp_public_key = NULL;
            *pp_private_key = NULL;
            return 0;
        }
    
        memcpy((*pp_private_key)->data, seed, 32);    public_key_derive(seed, (*pp_public_key)->data);
    return 1;
}

int ed25519_sign(ed25519_signature *p_signature, const unsigned char *p_message, size_t message_len, const ed25519_public_key *p_public_key, const ed25519_private_key *p_private_key) {
    unsigned char h[64];
    H(p_private_key->data, 32, h);
    
    i256 a = decodeint(h);
    a &= ~(((i256)7));
    a &= ~(((i256)1) << 255);
    a |= ((i256)1) << 254;
    
    // r = Hint(h[32:] + m)
    unsigned char *temp = default_allocator(0, 32 + message_len);
    if (temp == NULL) { // Check for allocation failure
        #ifndef NDEBUG
            log_error("ed25519_sign: Failed to allocate temp buffer for r_hint");
        #endif
        return 0;
    }
    memcpy(temp, h + 32, 32);
    memcpy(temp + 32, p_message, message_len);
    
    i256 r = Hint(temp, 32 + message_len);
    default_allocator(temp, 0);
    
    i256 B[2] = { get_Bx(), get_By() };
    i256 R[2];
    scalarmult(B, r, R);
    
    unsigned char R_enc[32];
    encodepoint(R, R_enc);
    
    // S = (r + Hint(R_enc + pk + m) * a) % l
    temp = default_allocator(0, 32 + 32 + message_len);
    if (temp == NULL) { // Check for allocation failure
        #ifndef NDEBUG
            log_error("ed25519_sign: Failed to allocate temp buffer for h_ram");
        #endif
        return 0;
    }
    memcpy(temp, R_enc, 32);
    memcpy(temp + 32, p_public_key->data, 32);
    memcpy(temp + 64, p_message, message_len);
    
    i256 h_ram = Hint(temp, 32 + 32 + message_len);
    default_allocator(temp, 0);
    
    i256 l_val = get_l();
    i256 S = (r + (i512)h_ram * a) % l_val;
    
    memcpy(p_signature->data, R_enc, 32);
    encodeint(S, p_signature->data + 32);
    
    return 1;
}

int ed25519_verify(const ed25519_signature *p_signature, const unsigned char *p_message, size_t message_len, const ed25519_public_key *p_public_key) {
    if (p_signature == NULL || p_message == NULL || p_public_key == NULL) return 0;
    
    i256 R[2];
    if (!decodepoint(p_signature->data, R)) return 0;
    
    i256 A[2];
    if (!decodepoint(p_public_key->data, A)) return 0;
    
    i256 S = decodeint(p_signature->data + 32);
    i256 l_val = get_l();
    if (S >= l_val) return 0;
    
    // h = Hint(R + pk + m)
    unsigned char *temp = default_allocator(0, 32 + 32 + message_len);
    if (temp == NULL) { // Check for allocation failure
        #ifndef NDEBUG
            log_error("ed25519_verify: Failed to allocate temp buffer for h_ram");
        #endif
        return 0;
    }
    memcpy(temp, p_signature->data, 32);
    memcpy(temp + 32, p_public_key->data, 32);
    memcpy(temp + 64, p_message, message_len);
    
    i256 h_ram = Hint(temp, 32 + 32 + message_len);
    default_allocator(temp, 0);
    
    // Check if B*S == R + A*h
    i256 B[2] = { get_Bx(), get_By() };
    i256 SB[2];
    scalarmult(B, S, SB);
    
    i256 hA[2];
    scalarmult(A, h_ram, hA);
    
    i256 RhA[2];
    edwards(R, hA, RhA);
    
    if (SB[0] == RhA[0] && SB[1] == RhA[1]) return 1;
    
    return 0;
}