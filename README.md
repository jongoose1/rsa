READ ME!

![READ ME!](https://c.tenor.com/zEtUpuHrIzwAAAAC/tenor.gif)

# Compile
`make`

# Install
`make install`

# Key Generation
`keygen keypair.kp`\
You will be prompted to create a password to encrypt your secret key.

To put your public key in a separate file for sharing:\
`cppk keypair.kp publickey.pk`

# Encrypt
To encrypt with public key:\
`encrypt plain.txt publickey.pk`

To encrypt with key pair use the -kp option:\
`encrypt plain.txt -kp keypair.kp`

Ciphertext will be stored in a file named cipher.txt by default.\
To set a custom output file use the -o option:\
`encrypt plain.txt publickey.pk -o custom_output.txt`

# Decrypt
`decrypt cipher.txt plain.txt keypair.kp`

# Sign
`sign plain.txt signature.txt keypair.kp`

# Verify
To verify with public key:\
`verify plain.txt signature.txt publickey.pk`

To verify with key pair:\
`verify plain.txt signature.txt -kp keypair.kp`
