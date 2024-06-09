#!/bin/bash

#Delete old files.
rm plain.txt cipher.txt plain_copy.txt

# Generate keys.
./keygen keys.kp

# Create plaintext.
echo "Hello, nobody!" > plain.txt
cp plain.txt plain_copy.txt
echo "Plaintext:"
cat plain.txt

# Encrypt.
./encrypt plain.txt -kp keys.kp -o cipher.txt
echo "Ciphertext:"
hexdump -x cipher.txt

# Delete plaintext.
rm plain.txt

#Decrypt
./decrypt cipher.txt keys.kp
echo "Plaintext:"
cat plain.txt

#Comparing
diff plain.txt plain_copy.txt
