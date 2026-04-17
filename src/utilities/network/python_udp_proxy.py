import socket
from threading import Thread
import importlib
import parser
import os


class UDPPortForwarder(Thread):
    def __init__(self, listen_host="0.0.0.0",
                 client_port=3000,
                 server_host="127.0.0.1",
                 server_port=3001):
        super().__init__(daemon=True)

        self.server_addr = (server_host, server_port)

        # Socket facing client
        self.client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.client_sock.bind((listen_host, client_port))

        # Socket facing upstream server
        self.server_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_sock.bind((listen_host, client_port + 1))

        self.last_client = None

        print(f"[Proxy] client-facing: {listen_host}:{client_port}")
        print(f"[Proxy] forwarding to: {server_host}:{server_port}")

    def client_to_server(self):
        while True:
            try:
                data, client_addr = self.client_sock.recvfrom(4096)
                self.last_client = client_addr

                try:
                    importlib.reload(parser)
                    parser.parse(data, 3000, "client")
                except Exception as e:
                    print(f"[C->S parser] {e}")

                self.server_sock.sendto(data, self.server_addr)

            except Exception as e:
                print(f"[C->S error] {e}")

    def server_to_client(self):
        while True:
            try:
                data, server_addr = self.server_sock.recvfrom(4096)

                try:
                    importlib.reload(parser)
                    parser.parse(data, 3001, "server")
                except Exception as e:
                    print(f"[S->C parser] {e}")

                if self.last_client:
                    self.client_sock.sendto(data, self.last_client)

            except Exception as e:
                print(f"[S->C error] {e}")

    def run(self):
        Thread(target=self.client_to_server, daemon=True).start()
        Thread(target=self.server_to_client, daemon=True).start()

        while True:
            pass


if __name__ == "__main__":
    proxy = UDPPortForwarder(
        client_port=3001,
        server_port=3000
    )
    proxy.start()

    try:
        while True:
            cmd = input("$ ")
            if cmd.strip() == "quit":
                os._exit(0)
    except KeyboardInterrupt:
        os._exit(0)