from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from binascii import unhexlify

def decrypt_data(encryptedLog, aesKey, IV):

    byteCipher = unhexlify(encryptedLog)
    byteKey = unhexlify(aesKey)
    byteIV = unhexlify(IV)
    
    aesDecryptor = AES.new(byteKey, AES.MODE_CBC, byteIV)

    originalData = unpad(aesDecryptor.decrypt(byteCipher), AES.block_size)
    
    return originalData
   