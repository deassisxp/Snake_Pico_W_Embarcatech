import socket
from datetime import datetime
from typing import Optional

HOST = "0.0.0.0"
PORT = 5000


def parse_score(payload: str) -> Optional[int]:
    if not payload.startswith("score="):
        return None
    raw = payload.split("=", 1)[1].strip()
    if not raw.isdigit():
        return None
    return int(raw)


def main() -> None:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((HOST, PORT))
    sock.settimeout(1.0)

    print(f"[UDP] Ouvindo em {HOST}:{PORT}")
    print("Pressione Ctrl+C para encerrar.")

    last_score = None
    total_messages = 0

    try:
        while True:
            try:
                data, addr = sock.recvfrom(1024)
            except socket.timeout:
                continue

            payload = data.decode(errors="ignore").strip()
            now = datetime.now().strftime("%H:%M:%S")
            total_messages += 1

            score = parse_score(payload)
            if score is None:
                print(f"[{now}] {addr} -> payload invalido: '{payload}'")
                ack = b"NACK invalid payload"
            else:
                last_score = score
                print(f"[{now}] {addr} -> score recebido: {score}")
                ack = f"ACK score={score}".encode()

            sock.sendto(ack, addr)
    except KeyboardInterrupt:
        print("\nEncerrando servidor...")
    finally:
        sock.close()
        print(f"Mensagens recebidas: {total_messages}")
        print(f"Ultimo score valido: {last_score}")


if __name__ == "__main__":
    main()
