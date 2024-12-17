#!/bin/bash

# Make.
make

# Generate keys.
./keygen keys.kp

# Create plaintext.
echo "Hello, nobody!" > plain.txt
echo "Plaintext:"
cat plain.txt

# Sign Plaintext
./sign plain.txt signature.txt keys.kp

#Verify Signature
./verify plain.txt signature.txt -kp keys.kp

# Encrypt.
./encrypt plain.txt cipher.txt -kp keys.kp

echo "Ciphertext:"
hexdump -x cipher.txt

# Delete plaintext.
rm plain.txt

#Decrypt.
./decrypt cipher.txt keys.kp > plain2.txt
echo "Plaintext:"
cat plain2.txt

#Delete old files.
rm cipher.txt plain2.txt keys.kp signature.txt
