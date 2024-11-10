###########################################################################

cmake_minimum_required (VERSION 3.17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/arm_setup/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/src/hardware/platform")

set(CMAKE_TOOLCHAIN_FILE ./arm_setup/cmake/arm-gcc-toolchain.cmake)
set(PROJECT blinky)

include(device)
include(utils)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT} LANGUAGES C CXX ASM)

option(ENABLE_FOTA "Enable firmware update over the air" OFF)
option(ENABLE_RTT "Enable SEGGER RTT for debugging" OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "")
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

add_subdirectory(3d_party EXCLUDE_FROM_ALL)
add_subdirectory(src/hardware/platform)

set(INCLUDE_DIRS
    3d_party/cmsis_device_stm_l4/Include
    3d_party/cmsis_5/CMSIS/Core/Include
    3d_party/stm32l4xx_hal_driver/Inc
    src
    src/application
    src/device
    src/device/user_indication
    src/hardware
    src/hardware/output_pin
    src/hardware/driver/spi
    src/_common
    src/hardware/qpcpp_callbacks
)


## Application source files
# C++ source files
set(APP_CPP_SRCS
    src/system_manager.cpp
    src/main.cpp
    src/_common/io_functions.cpp
    src/_common/common.cpp


    src/application/application_manager.cpp
    src/application/blinky/blinky.cpp
    src/application/sensor/sensor.cpp
    src/application/startup/startup.cpp
    
    src/device/device_manager.cpp
    src/device/user_indication/user_indication.cpp
    src/device/sensors/test/test.cpp
    
    src/hardware/hardware_manager.cpp
    src/hardware/output_pin/output_pin.cpp
    src/hardware/driver/spi/spi.cpp
    src/hardware/qpcpp_callbacks/qpcpp_callbacks.cpp
    src/hardware/qpcpp_callbacks/qpcpp_error.cpp
)

include_directories(${INCLUDE_DIRS})

add_executable( ${PROJECT} 
                ${APP_CPP_SRCS})

target_include_directories(${PROJECT} PRIVATE include)

target_link_libraries(${PROJECT} PRIVATE stm32)

target_link_libraries(${PROJECT} PRIVATE qpcpp)

target_linker_script(${PROJECT} "${CMAKE_CURRENT_LIST_DIR}/src/hardware/linker/blinky.ld")

set(WARNINGS
    all
	extra
	maybe-uninitialized
	strict-overflow=2
	switch
	write-strings
)

list(TRANSFORM WARNINGS PREPEND "-W")
list(TRANSFORM DISABLE_WARNINGS PREPEND "-Wno-")
target_compile_options(${PROJECT} PRIVATE "${WARNINGS}" "${DISABLE_WARNINGS}")
if (FATAL_ERRORS)
    target_compile_options(${PROJECT} PRIVATE "-Wfatal-errors")
endif ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile")

set (JLINK_IP_FILE "${CMAKE_CURRENT_LIST_DIR}/jlink_ip.cmake")
if (EXISTS "${JLINK_IP_FILE}")
    include("${JLINK_IP_FILE}")
endif ()

set(LAUNCH_FILE_INTERMEDIATE "${CMAKE_CURRENT_BINARY_DIR}/launch.json.in")
configure_file("${CMAKE_CURRENT_LIST_DIR}/arm_setup/cmake/launch.json.in" ${LAUNCH_FILE_INTERMEDIATE} @ONLY)

file(GENERATE
    OUTPUT "${CMAKE_CURRENT_LIST_DIR}/.vscode/launch.json"
    INPUT ${LAUNCH_FILE_INTERMEDIATE}
)