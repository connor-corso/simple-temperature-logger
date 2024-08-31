cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(simple_sensor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w CACHE STRING "Board type")

pico_sdk_init()

add_executable(simple_sensor
    simple_sensor.c
    memoryInfo.c
    networkCommunication.c
    sensors.c
    )

# For adding the dht20 library found in lib
add_subdirectory("lib")
target_include_directories(simple_sensor PUBLIC "lib")
target_link_directories(simple_sensor PUBLIC "lib")

# Add the standard include files to the build
target_include_directories(simple_sensor PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
   #${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_enable_stdio_uart(simple_sensor 1)
pico_enable_stdio_usb(simple_sensor 1)

target_link_libraries(simple_sensor
        pico_stdlib
        hardware_adc
        hardware_i2c
        pico_cyw43_arch_lwip_threadsafe_background
        dht20
        pico_lwip_mbedtls
        pico_mbedtls
    )
pico_add_extra_outputs(simple_sensor)