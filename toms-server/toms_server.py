import argparse
from decryption import *
from collection import *

def main():
    
    #decrypt_log(encryptedHex, hexKey, hexIV)

    hex_key = input("Please enter your 256-bit AES key: ")

    while(True):

        clean_request_data = ""
        for char in get_raw_post_data():
            if(ord(char) != 0):
                clean_request_data += char

        seperated_request_data = clean_request_data.split('data=')

        encrypted_data = seperated_request_data[1]

        hex_iv = encrypted_data[0:31]
        encrypted_log = encrypted_data[32:len(encrypted_data)-1]

        #decrypted_log = decrypt_data(encrypted_log, hex_key, hex_iv)
        
        print(seperated_request_data)
        print()
        print(encrypted_data)
        print()
        print(encrypted_log)



        #extract good stuff from clean_post_data and begin decryption and log file creation



if __name__ == "__main__":
    main()