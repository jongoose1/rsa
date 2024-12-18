READ ME!

![READ ME!](https://c.tenor.com/zEtUpuHrIzwAAAAC/tenor.gif)

Bad RSA implementation. 4096 bit modulus. Password-based secret key encryption (stream cipher using JG2 hashing as PRNG).  
Linux only.

# Compile

`make`

# Install

`make install`  
This copies binaries to `~/bin`. If this directory is not included in your `$PATH`, run:  
`PATH="$HOME/bin:$PATH"`

# Key Generation

`keygen keypair_file [number_of_threads]`  
You will be prompted to create a password to encrypt your secret key.

To put your public key in a separate file for sharing:  
`cppk keypair_file public_key_file`

# Encrypt

`encrypt plaintext_file ciphertext_file public_key_file`

To encrypt with a key pair use the -kp option:  
`encrypt plaintext_file ciphertext_file -kp keypair_file`

To encrypt from stdin use the -s option:  
`encrypt -s ciphertext_file public_key_file`  
If using the keyboard as input, use CTRL+D (EOF) to signify the end of your plaintext.

# Decrypt

`decrypt ciphertext_file keypair_file`  
You will be prompted to enter your password.

# Sign

`sign file signature_file keypair_file`

# Verify

`verify file signature_file public_key_file`

To verify with key pair use the -kp option:  
`verify file signature_file -kp keypair_file`

# Tips

Outputs can be piped to encrypt:  
`ls | encrypt -s ciphertext_file public_key_file`

The output of decrypt can be redirected or piped:  
`decrypt ciphertext_file keypair_file > plaintext_file`  
`decrypt ciphertext_file keypair_file | grep pattern`  
When the output is redirected or piped, the password prompt will be muted to prevent it from being included in the output. You still need to enter your password (and hit \<Enter\>) for decryption to begin.

You can use these together to edit an encrypted file without ever writing the plaintext to disk:  
`decrypt ciphertext_file keypair_file | sed 's/find/replace/' | encrypt -s new_ciphertext_file -kp keypair_file`  
Note the difference between `ciphertext_file` and `new_ciphertext_file`. You cannot directly overwrite the ciphertext file as it is being read by decrypt. Doing so will corrupt your ciphertext beyond recovery. D:  

Decrypting takes a very long time. I do not recommend using this program for large files.
