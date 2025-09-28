cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME standard_robot_bb)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined library search paths
)

file(GLOB_RECURSE SOURCES
        "Core/Src/*.c"
        "USER/BSP/Src/*.c"
        "USER/Components/Devices/Src/*.c"
        "USER/Components/Algorithm/Src/*.c"
        "USER/Components/Controller/Src/*.c"
        "USER/Components/Support/Src/*.c"
        "USER/Application/Task/Src/*.c"
        "USER/Application/Santa/Src/*.c"


        # 可按需添加其他目录
)


# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user sources here
        ${SOURCES}
)


# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined include paths
        Inc
        Src
        USER/Application/Santa/Inc
        USER/Application/Santa/Src

        USER/Components/Devices/Inc
        USER/Components/Algorithm/Inc
        USER/Components/Controller/Inc
        USER/Components/Support/Inc

        USER/Application/Task/Inc

        USER/Components/Devices/Src
        USER/Components/Algorithm/Src
        USER/Components/Controller/Src
        USER/Components/Support/Src
        USER/Application/Task/Src
        USER/BSP/Inc
        Middlewares/ST/STM32_USB_Device_Library/Core/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        USE_HAL_DRIVER
        STM32F407xx       # MCU型号宏
        ARM_MATH_CM4      # 或 ARM_MATH_CM7，看你具体用的内核
        __FPU_USED=1U
        __FPU_PRESENT=1U
        ARM_MATH_MATRIX_CHECK
        ARM_MATH_ROUNDING
)

target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE


)
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx
        # Add user defined libraries
)

add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ${CMAKE_PROJECT_NAME}.elf -Oihex ${CMAKE_PROJECT_NAME}.hex
        COMMENT "Generating hex file for STM32"
)

# 通用 thumb + cortex-m4 设置
set(COMMON_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# C
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
# C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
# ASM
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_FLAGS}")
