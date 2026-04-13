# Servidor UDP para receber score do Snake

## Requisitos
- Python 3 instalado

## Como rodar
No terminal, dentro da pasta `server`:

```powershell
python server.py
```

O servidor vai ouvir em `0.0.0.0:5000` e imprimir mensagens como:

```text
[20:41:03] ('192.168.0.123', 54321) -> score recebido: 20
```

Ele tambem responde com `ACK score=<valor>` para cada pacote valido.

Para encerrar com seguranca, use `Ctrl+C`.
