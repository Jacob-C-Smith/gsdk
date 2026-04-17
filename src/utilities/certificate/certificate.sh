#!/bin/bash

# root
./build/utilities/ed25519_key_generator -o root.key
./build/utilities/certificate_create -o root.cer -s RootCA -k root.key -ca
./build/utilities/certificate_sign -o root.cer -k root.key
./build/utilities/certificate_verify -o root.cer

# intermediate
./build/utilities/ed25519_key_generator -o inter.key
./build/utilities/certificate_create -o inter.cer -s InterCA -k inter.key -i root.cer -ca
./build/utilities/certificate_sign -o inter.cer -k root.key
./build/utilities/certificate_verify -o inter.cer -i root.cer

# leaf
./build/utilities/ed25519_key_generator -o leaf.key
./build/utilities/certificate_create -o leaf.cer -s Leaf -k leaf.key -i inter.cer
./build/utilities/certificate_sign -o leaf.cer -k inter.key
./build/utilities/certificate_verify -o leaf.cer -i inter.cer
