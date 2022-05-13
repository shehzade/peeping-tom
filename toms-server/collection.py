from nclib import TCPServer

def get_raw_post_data():
    
    post_server = TCPServer(('127.0.0.1', 80))
    
    for request in post_server:

        raw_post_data = request.recv()
        return raw_post_data.decode('utf-8')