from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from binascii import unhexlify

def decrypt_data(cipher_text, aes_key, aes_iv):

    byte_cipher = unhexlify(cipher_text)
    byte_key = unhexlify(aes_key)
    byte_iv = unhexlify(aes_iv)
    
    aes_decryptor = AES.new(byte_key, AES.MODE_CBC, byte_iv)

    plain_text = unpad(aes_decryptor.decrypt(byte_cipher), AES.block_size)
    
    return plain_text
   