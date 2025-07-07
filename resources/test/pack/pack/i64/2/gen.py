from itertools import product
import struct

def generate_arrays(n, elements):
    result = []
    for arr in product(elements, repeat=n):
        result.append(list(arr))
    return result

if __name__ == "__main__":
    n = 2
    elements = [1, 2, 3]
    arrays = generate_arrays(n, elements)
    print(arrays)
    for arr in arrays:
        path = "_".join(map(str, arr))
        with open(f'{path}.bin', 'wb') as f:
            f.write(struct.pack('<' + 'q'*len(arr), *arr))