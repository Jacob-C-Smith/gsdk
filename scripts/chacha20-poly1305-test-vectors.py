import json

def hex_to_c_array(hex_str):
    if not hex_str:
        return "{ 0 }"
    return "{ " + ", ".join([f"0x{hex_str[i:i+2]}" for i in range(0, len(hex_str), 2)]) + " }"

def hex_len(hex_str):
    return len(hex_str) // 2

data = { }
with open('scripts/vectors/wycheproof/chacha20_poly1305_test.json', 'r') as f:
    data = json.load(f)

data = data["testGroups"][0]["tests"]

print("""/** !
 * aead test vectors
 * 
 * @file src/crypto/aead_test/aead_test.h
 * 
 * @author Jacob Smith
 */

// gsdk
/// crypto
#include <crypto/chacha20.h>
#include <crypto/poly1305.h>
#include <crypto/aead.h>

struct aead_test_vector_s
{
    char   _key[32];
    char   _nonce[12];
    char   _tag[16];
    char   _aad[513];
    size_t aad_len;
    char   _plain[513];
    size_t plain_len;
    char   _cipher[513];
    size_t cipher_len;
    bool   valid;
};

typedef struct aead_test_vector_s aead_test_vector;
""")

print("aead_test_vector _aead_test_vectors[] =")
print("{")
for i, di in enumerate(data):
    key    = di["key"]
    iv     = di["iv"]
    aad    = di["aad"]
    msg    = di["msg"]
    ct     = di["ct"]
    tag    = di["tag"]
    result = di["result"]
    
    print(f"    [{i}] = (aead_test_vector)")
    print("    {")
    print(f"        ._key       = {hex_to_c_array(key)},")
    print(f"        ._nonce     = {hex_to_c_array(iv)},")
    print(f"        ._tag       = {hex_to_c_array(tag)},")
    print(f"        ._aad       = {hex_to_c_array(aad)},")
    print(f"        .aad_len    = {hex_len(aad)},")
    print(f"        ._plain     = {hex_to_c_array(msg)},")
    print(f"        .plain_len  = {hex_len(msg)},")
    print(f"        ._cipher    = {hex_to_c_array(ct)},")
    print(f"        .cipher_len = {hex_len(ct)},")
    print(f"        .valid      = {'true' if result == 'valid' else 'false'}")
    print("    },")
print("};")
