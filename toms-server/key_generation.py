import requests

def get_aes_256_key():

    site_url = 'https://www.allkeysgenerator.com/Default.aspx/GetEncryptionKeyResults'
    body_payload = {'bit' : '256' , 'hex' : 'true' , 'amount': '1'}
    request_headers = {'Host' : 'www.allkeysgenerator.com' , 
                       'Content-Type' : 'application/json; charset=UTF-8'}

    new_aes_256_key = requests.post(site_url, json = body_payload, headers = request_headers)
    
    formatted_aes_256_key = str(new_aes_256_key.text[9:len(new_aes_256_key.text)-5])

    return formatted_aes_256_key
