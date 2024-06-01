#!/bin/bash

#Delete old files.
rm plain.txt cipher.txt plain_copy.txt

# Generate keys.
if ! test -f keys.kp; then
	(while sleep 60; do echo "[$(date)] Generating Keys..."; done) &
	PID=$!
	./keygen.x keys.kp
	kill $PID
fi

# Create plaintext.
echo "Hello, nobody!" > plain.txt
cp plain.txt plain_copy.txt
echo "Plaintext:"
cat plain.txt

# Encrypt.
./encrypt.x plain.txt -kp keys.kp -o cipher.txt
echo "Ciphertext:"
hexdump -x cipher.txt

# Delete plaintext.
rm plain.txt

#Decrypt
./decrypt.x cipher.txt keys.kp
echo "Plaintext:"
cat plain.txt

#Comparing
diff plain.txt plain_copy.txt
