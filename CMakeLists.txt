cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(SnakeGame_PicoW C CXX ASM)
pico_sdk_init()

add_executable(SnakeGame_PicoW 
    SnakeGame_PicoW.c 
    src/hardware.c
    src/display.c
    src/snake.c
    src/wifi_client.c
)

target_include_directories(SnakeGame_PicoW PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(SnakeGame_PicoW 
    pico_stdlib 
    hardware_adc 
    hardware_i2c
    hardware_pwm 
    pico_cyw43_arch_lwip_poll
)

pico_enable_stdio_usb(SnakeGame_PicoW 1)
pico_add_extra_outputs(SnakeGame_PicoW)
