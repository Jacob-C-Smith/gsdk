# for i in $(seq 1 1000);
# do
#     ./build/tests/hash_test >> hash.out
# done
cat hash.out | grep -i "fnv tests took" | awk -F':' '{print $2}' | awk -F' ' '{printf("%s.%s\n",$2,$4)}' | sort -n | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g" > fnv
cat hash.out | grep -i "crc tests took" | awk -F':' '{print $2}' | awk -F' ' '{printf("%s.%s\n",$2,$4)}' | sort -n | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g" > crc
cat hash.out | grep -i "mmh tests took" | awk -F':' '{print $2}' | awk -F' ' '{printf("%s.%s\n",$1,$4)}' | sort -n | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g" > mmh
