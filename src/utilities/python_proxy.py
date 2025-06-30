import socket
from threading import Thread
import os

# Placeholder for parser module
import importlib
import parser  # assumes a module named parser.py exists

class Proxy2Server(Thread):
    def __init__(self, host, port):
        super().__init__()
        self.client = None  # Set externally
        self.port = port
        self.host = host
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((self.host, self.port))

    def run(self):
        while True:
            try:
                data = self.server.recv(4096)
                if not data:
                    break
                try:
                    importlib.reload(parser)
                    parser.parse(data, self.port, 'server')
                except Exception as e:
                    print(f"[Proxy2Server] Parser error: {e}")
                if self.client:
                    self.client.sendall(data)
            except Exception as e:
                print(f"[Proxy2Server] Error: {e}")
                break


class Client2Proxy(Thread):
    def __init__(self, host, port):
        super().__init__()
        self.server = None  # Set externally
        self.port = port
        self.host = host
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))
        self.sock.listen(1)
        print(f"[Client2Proxy] Listening on {self.host}:{self.port}")
        self.client, addr = self.sock.accept()
        print(f"[Client2Proxy] Connection from {addr}")

    def run(self):
        while True:
            try:
                data = self.client.recv(4096)
                if not data:
                    break
                try:
                    importlib.reload(parser)
                    parser.parse(data, self.port, 'client')
                except Exception as e:
                    print(f"[Client2Proxy] Parser error: {e}")
                if self.server:
                    self.server.sendall(data)
            except Exception as e:
                print(f"[Client2Proxy] Error: {e}")
                break


class Proxy(Thread):
    def __init__(self, from_host, to_host, port):
        super().__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port = port

    def run(self):
        while True:
            print(f"[Proxy({self.port})] Setting up")
            try:
                c2p = Client2Proxy(self.from_host, self.port)
                p2s = Proxy2Server(self.to_host, self.port)

                c2p.server = p2s.server
                p2s.client = c2p.client

                c2p.start()
                p2s.start()

                c2p.join()
                p2s.join()
            except Exception as e:
                print(f"[Proxy({self.port})] Error: {e}")
            print(f"[Proxy({self.port})] Restarting connection...")


if __name__ == "__main__":
    proxy_port = 3001
    master_server = Proxy('0.0.0.0', '127.0.0.1', proxy_port)
    master_server.start()

    try:
        while True:
            cmd = input('$ ')
            if cmd.strip() == 'quit':
                os._exit(0)
    except KeyboardInterrupt:
        print("\nExiting...")
        os._exit(0)
    except Exception as e:
        print("Error: {}".format(e))
