# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/connor/pico/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(simple_sensor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(simple_sensor 
    simple_sensor.c
    memoryInfo.c
    networkCommunication.c
    sensors.c
    errors.c
    appGPIO.c )

pico_set_program_name(simple_sensor "simple_sensor")
pico_set_program_version(simple_sensor "0.1")

pico_enable_stdio_uart(simple_sensor 1)
pico_enable_stdio_usb(simple_sensor 0)

# Add the standard library to the build
target_link_libraries(simple_sensor
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(simple_sensor PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)


# For adding the dht20 library found in lib
add_subdirectory("lib")
target_include_directories(simple_sensor PUBLIC "lib")
target_link_directories(simple_sensor PUBLIC "lib")


# Add any user requested libraries
target_link_libraries(simple_sensor 
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_adc
        hardware_i2c
        dht20
        pico_lwip_mbedtls
        pico_mbedtls
        hardware_watchdog
        )

pico_add_extra_outputs(simple_sensor)

