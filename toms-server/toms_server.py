import argparse
from decryption import *
from collection import *

def main():
    
    #encryptedHex = 'C299A8577104768664B96A33D7B1A33E4CDED752D73C0C9F5FB0CC84690DDC125F2CF8B51288D5754C7ECF666D55DB7D8F027DE446505B2A038043F90BD29E62FC2FDD347D9FE5F7D7F4289B762731ABC1DA4434146E1AD182D5CA0BBB0D1EE60CAFFAC0025E10F126583307714A16D63787C6C69E70B204E02C66EE327BC05394909983EB1D1B3DCBF5FD332886105FDE9FC05A2122D4140E86234A5A10F29EDFC507DDAD29CEECE15642232CDCE2EBB30045199C1AF050DD2813B07FF0C44541A36D900AC45F7EFC6E8E8C4C68223C6E7A8FB843F3E9BB7114E832A51C06C810714211E6D786DBA8E113CF9EEFCCEE41EC87F2AEB245928F9096A9163EE0A2'
    #hexIV = '200F0C21EEE34177039C6E835D84A58B'
    #decrypt_log(encryptedHex, hexKey, hexIV)

    hexKey = input("Please enter your 256-bit AES key:\t")

    while(True):

        clean_request_data = ""
        for char in get_raw_post_data():
            if(ord(char) != 0):
                clean_request_data += char

        seperated_request_data = clean_request_data.split('data=')

        print(seperated_request_data[1])

        #extract good stuff from clean_post_data and begin decryption and log file creation



if __name__ == "__main__":
    main()