from nclib import TCPServer

def get_raw_post_data():
    
    post_server = TCPServer(('0.0.0.0', 80))
    
    for request in post_server:
        raw_post_data = request.recv()
        return raw_post_data.decode('utf-8')