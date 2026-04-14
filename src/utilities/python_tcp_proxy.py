import socket
from threading import Thread
import importlib
import parser
import os


class Pipe(Thread):
    def __init__(self, src, dst, listen_port, direction):
        super().__init__(daemon=True)
        self.src = src
        self.dst = dst
        self.listen_port = listen_port
        self.direction = direction

    def run(self):
        while True:
            try:
                data = self.src.recv(4096)
                if not data:
                    break

                try:
                    importlib.reload(parser)
                    parser.parse(data, self.listen_port, self.direction)
                except Exception as e:
                    print(f"[{self.direction}] Parser error: {e}")

                self.dst.sendall(data)

            except Exception as e:
                print(f"[{self.direction}] Error: {e}")
                break

        try:
            self.src.close()
        except:
            pass

        try:
            self.dst.close()
        except:
            pass


class Proxy(Thread):
    def __init__(self, listen_host, listen_port, target_host, target_port):
        super().__init__(daemon=True)
        self.listen_host = listen_host
        self.listen_port = listen_port
        self.target_host = target_host
        self.target_port = target_port

    def run(self):
        server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_sock.bind((self.listen_host, self.listen_port))
        server_sock.listen(5)

        print(f"[Proxy] Listening on {self.listen_host}:{self.listen_port}")
        print(f"[Proxy] Forwarding to {self.target_host}:{self.target_port}")

        while True:
            client_sock, addr = server_sock.accept()
            print(f"[Proxy] Client connected from {addr}")

            target_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            target_sock.connect((self.target_host, self.target_port))

            upstream = Pipe(
                client_sock,
                target_sock,
                self.listen_port,
                "client->server"
            )

            downstream = Pipe(
                target_sock,
                client_sock,
                self.listen_port,
                "server->client"
            )

            upstream.start()
            downstream.start()


if __name__ == "__main__":
    proxy = Proxy(
        listen_host="0.0.0.0",
        listen_port=3001,
        target_host="127.0.0.1",
        target_port=3000
    )

    proxy.start()

    try:
        while True:
            cmd = input("$ ")
            if cmd.strip() == "quit":
                os._exit(0)

    except KeyboardInterrupt:
        print("\nExiting...")
        os._exit(0)