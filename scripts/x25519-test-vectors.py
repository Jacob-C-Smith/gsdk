import json

def hex_to_c_array(hex_str):
    if not hex_str:
        return "{ 0 }"
    return "{ " + ", ".join([f"0x{hex_str[i:i+2]}" for i in range(0, len(hex_str), 2)]) + " }"

def hex_len(hex_str):
    return len(hex_str) // 2

data = { }
with open('scripts/vectors/wycheproof/x25519_test.json', 'r') as f:
    data = json.load(f)

data = data["testGroups"][0]["tests"]

print("""/** !
 * x25519 test vectors
 * 
 * @file src/crypto/x25519/x25519_test.h
 * 
 * @author Jacob Smith
 */

// gsdk
/// crypto
#include <crypto/x25519.h>

struct x25519_test_vector_s
{
    x25519_public_key    _public;
    x25519_private_key   _private;
    x25519_shared_secret _shared;
};

typedef struct x25519_test_vector_s x25519_test_vector;
""")

print("x25519_test_vector _x25519_test_vectors[] =")
print("{")
for i, di in enumerate(data):
    public  = di["public"]
    private = di["private"]
    shared  = di["shared"]
    
    print(f"    [{i}] = (x25519_test_vector)")
    print("    {")
    print(f"        ._public  = {hex_to_c_array(public)},")
    print(f"        ._private = {hex_to_c_array(private)},")
    print(f"        ._shared  = {hex_to_c_array(shared)},")
    print("    },")
print("};")
