# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(psico_teste C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(psico_teste
        src/main.c
        src/menu.c
        src/testes_psi.c
        src/teste_cores.c
        src/teste_obstaculos.c
        src/teste_previsao.c
        src/teste_logica.c

        src/modules/buttons.c
        src/modules/buzzers.c
        src/modules/display.c
        src/modules/joystick.c        
        src/modules/led_matrix.c
        src/modules/multicore_handler.c
        src/modules/rgb_led.c  
        src/modules/wifi_rtc.c
    
        src/drivers/ssd1306.c)

pico_set_program_name(psico_teste "Psico Teste")
pico_set_program_version(psico_teste "0.1")

# Generate PIO header
pico_generate_pio_header(psico_teste ${CMAKE_CURRENT_LIST_DIR}/src/pio/ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(psico_teste 1)
pico_enable_stdio_usb(psico_teste 1)

# Add the standard library to the build
target_link_libraries(psico_teste
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(psico_teste PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/config
)

# Add any user requested libraries
target_link_libraries(psico_teste 
        hardware_i2c
        hardware_adc
        hardware_pio
        hardware_dma
        hardware_clocks
        hardware_pwm
        pico_multicore
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_rtc       
        )

pico_add_extra_outputs(psico_teste)