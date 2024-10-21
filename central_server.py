
import socket

SERVER_PORT = 8080
SERVER_IP = '0.0.0.0'

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((SERVER_IP, SERVER_PORT))
    server_socket.listen(5)
    print(f"Server started, listening on {SERVER_IP}:{SERVER_PORT}")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")
        data = client_socket.recv(1024).decode('utf-8')
        if data:
            print(f"Received data: {data}")
            with open("monitoring_logs.txt", "a") as log_file:
                log_file.write(f"{addr}: {data}\n")
        client_socket.close()

if __name__ == "__main__":
    start_server()
