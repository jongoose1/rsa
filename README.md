READ ME!

![READ ME!](https://c.tenor.com/zEtUpuHrIzwAAAAC/tenor.gif)

# Compile
`make`

# Install
`make install`

# Key Generation
`keygen keypair_file [number_threads]`\
You will be prompted to create a password to encrypt your secret key.

To put your public key in a separate file for sharing:\
`cppk keypair_file public_key_file`

# Encrypt
To encrypt a file with a public key:\
`encrypt plaintext_file ciphertext_file public_key_file`

To encrypt with a key pair use the -kp option:\
`encrypt plaintext_file ciphertext_file -kp keypair_file`

To encrypt from stdin use the -s option:\
`encrypt -s ciphertext_file public_key_file`\
If using the keyboard as input, use CTRL+D (EOF) to signify the end of your plaintext.

Outputs can be piped to encrypt:\
`ls | encrypt -s ciphertext_file public_key_file`

# Decrypt
`decrypt ciphertext_file keypair_file`\
You will be prompted to enter your password.

The output of decrypt can be redirected:\
`decrypt ciphertext_file keypair_file > plaintext_file`\
When the output is redirected, password prompt will be muted to prevent it from being included in the output. You still need to enter your password (and hit \<Enter\>) for decryption to begin. 

# Sign
`sign file signature_file keypair_file`

# Verify
To verify with public key:\
`verify file signature_file public_key_file`

To verify with key pair use the -kp option:\
`verify file signature_file -kp keypair_file`
