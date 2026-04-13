# SnakeGame (Raspberry Pi Pico W)

Este repositório contém uma implementação do jogo "Snake" para Raspberry Pi Pico W. O firmware controla o display e os controles locais e, ao final de uma partida, envia o score para um servidor UDP na rede.

## Conteúdo
- `CMakeLists.txt` – configuração do build com Pico SDK.
- `SnakeGame_PicoW.c` – ponto de entrada e loop do jogo.
- `src/` – código fonte do projeto:
  - `hardware.c`, `hardware.h` – inicialização e leitura de botões/joystick, sons.
  - `display.c`, `display.h` – funções para desenhar no display.
  - `snake.c`, `snake.h` – lógica do jogo (movimento, colisões, comida).
  - `wifi_client.c` – envio de score via UDP (lwIP).
  - `wifi_config.h` – SSID, senha e IP/porta do servidor (editar para seus testes).
- `server/server.py` – servidor UDP simples em Python para receber scores.
- `relatorio.tex` – rascunho de relatório em LaTeX (se gerado).

## Pré-requisitos (Windows)
- Pico SDK e toolchain (arm-none-eabi-gcc)
- `cmake`, `ninja`
- Ferramentas opcionais: `picotool`, `openocd` (para gravação)
- VS Code com extensões recomendadas: CMake Tools, C/C++
- Python 3 para rodar o servidor de teste

Observação: este workspace já inclui tarefas do VS Code que usam `${env:USERPROFILE}/.pico-sdk/...`. Verifique se você instalou o Pico SDK nesse local ou ajuste as tarefas conforme necessário.

## Configurar credenciais e servidor
1. Abra `src/wifi_config.h` e atualize `WIFI_SSID` e `WIFI_PASS` com sua rede Wi‑Fi.
2. Ajuste `SERVER_IP` para o IP do computador que executará o servidor Python (pode ser o IP da sua máquina local na rede) e `SERVER_PORT` para a porta desejada.

Exemplo (já presente no repositório):
```
#define WIFI_SSID "NOME_WIFI"
#define WIFI_PASS "SENHA_WIFI"
#define SERVER_IP "IP_SERVIDOR"
#define SERVER_PORT 5000
```

## Executando o servidor de teste (PC)
No computador que receberá os scores:

```powershell
python -m venv .venv
.venv\Scripts\activate
python server/server.py
```

O servidor imprimirá mensagens recebidas e enviará ACKs de volta ao Pico W.

## Compilar e gravar (VS Code)
Recomendo usar as tarefas já configuradas na workspace.

Usando a paleta de comandos do VS Code (Ctrl+Shift+P) → `Tasks: Run Task`:
- `Compile Project` — compila com `ninja -C build`.
- `Run Project` — usa `picotool` para carregar o firmware no dispositivo (se disponível).
- `Flash` — usa `openocd` para programar via probe (se você tiver probe compatível).

Ou via terminal:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
# ou
ninja -C build
```

Gravar com `picotool` (exemplo):

```powershell
%USERPROFILE%\\.pico-sdk\\picotool\\...\\picotool.exe load build\\SnakeGame_PicoW.uf2 -fx
```

Observação: a workspace contém um `pico_enable_stdio_usb(SnakeGame_PicoW 1)`, portanto os `printf()` do firmware aparecem via USB serial. Abra um monitor serial (ex.: PuTTY, minicom, terminal integrado) na porta COM correspondente para ver logs.

## Fluxo de teste sugerido
1. Ajuste `src/wifi_config.h` com SSID/senha e `SERVER_IP`.
2. No PC, inicie `server/server.py`.
3. Compile o firmware (`Compile Project`).
4. Grave o firmware (`Run Project` ou `Flash`).
5. Observe a saída serial; inicie o jogo no Pico, cause um Game Over e verifique que o servidor recebeu `score=NNN`.

## Solução de problemas
- Se o Pico não conectar ao Wi‑Fi: verifique SSID/senha, e mensagens no serial (há prints em `wifi_client.c`).
- Se o servidor não receber pacotes: verifique `SERVER_IP` (use IP do PC), desative temporariamente firewall ou libere UDP/porta.
