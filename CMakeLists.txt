cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE STRING "arm-none-eabi-gcc" FORCE)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE STRING "arm-none-eabi-g++" FORCE)
SET(AS arm-none-eabi-gcc  CACHE STRING "arm-none-eabi-as")
SET(CMAKE_AR arm-none-eabi-ar CACHE STRING "arm-none-eabi-ar" FORCE)
SET(CMAKE_LD arm-none-eabi-ld CACHE STRING "arm-none-eabi-ld" FORCE)
SET(NM arm-none-eabi-nm CACHE STRING "arm-none-eabi-nm" FORCE)
SET(OBJCOPY arm-none-eabi-objcopy CACHE STRING "arm-none-eabi-objcopy" FORCE)
SET(OBJDUMP arm-none-eabi-objdump CACHE STRING "arm-none-eabi-objdump" FORCE)
SET(READELF arm-none-eabi-readelf CACHE STRING "arm-none-eabi-readelf" FORCE)
SET(CMAKE_RANLIB arm-none-eabi-ranlib CACHE STRING "arm-none-eabi-ranlib" FORCE)

project(PCGame_Controller CXX)
enable_language(C CXX ASM)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

# set compiler option
set(COMMON_COMPILER_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32F413xx -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF\"$(@:%.o=%.d)\" -u _printf_float -u _scanf_float")
set(CMAKE_C_FLAGS "${COMMON_COMPILER_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "-O2 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS "${COMMON_COMPILER_FLAGS} -D_GLIBCXX_USE_C99 -Wno-missing-field-initializers -std=gnu++17")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_ASM_FLAGS "${COMMON_COMPILER_FLAGS} -x assembler-with-cpp")
set(CMAKE_ASM_FLAGS_DEBUG "-O2 -g")
set(CMAKE_ASM_FLAGS_RELEASE "-O2")

set(CMAKE_VERBOSE_MAKEFILE OFF)

# Echo compile flags
message("Build type:\n\t${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Compile flags:\n\t${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Compile flags:\n\t${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
else()
    message("Compile flags:\n\t${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -specs=nano.specs -T${CMAKE_SOURCE_DIR}/STM32F413XX_FLASH.ld  -lc -lm -lnosys  -Wl,-Map=${CMAKE_PROJECT_NAME}.map,--cref -Wl,--gc-sections --specs=nosys.specs  -Wl,--print-memory-usage")
# includes
include_directories("USB_DEVICE/App" "USB_DEVICE/Target" "Core/Inc" "Drivers/CMSIS/Device/ST/STM32F4xx/Include" "Drivers/CMSIS/Include" "Drivers/STM32F4xx_HAL_Driver/Inc" "Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" "include" "Middlewares/ST/STM32_USB_Device_Library/Core/Inc" "Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc")
list(REMOVE_ITEM USERS_SOURCE "${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc/usbd_hid.h")

# sources
file(GLOB PERIPH_LIB_SOURCE "Drivers/STM32F4xx_HAL_Driver/Src/*.c"  "*.s" "USB_DEVICE/App/*.c" "USB_DEVICE/Target/*.c" "Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c" "Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/*.c")
file(GLOB USERS_SOURCE "*.cpp" "Core/Src/*.c" "src/*.cpp" "src/*.c")
list(REMOVE_ITEM USERS_SOURCE "${CMAKE_SOURCE_DIR}/Core/Src/main.c" "${CMAKE_SOURCE_DIR}/Src/stm32f4xx_it.c")
list(REMOVE_ITEM PERIPH_LIB_SOURCE "${CMAKE_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c")

add_executable(${PROJECT_NAME}.elf  ${PERIPH_LIB_SOURCE} ${USERS_SOURCE})

add_custom_target(${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy --output-format=ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS ${PROJECT_NAME}.elf)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.hex)


if(EXISTS /proc/sys/fs/binfmt_misc/WSLInterop)
   # WINDOWS(WSL)
   add_custom_target(write COMMAND make -s -j8 COMMAND /mnt/c/Program\ Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD -d ./${PROJECT_NAME}.elf -s)
   add_custom_target(write_only COMMAND /mnt/c/Program\ Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD -d ./${PROJECT_NAME}.elf -s)
elseif(EXISTS /Applications/STMicroelectronics)
   # MAC (NOT TESTED)
   add_custom_target(write COMMAND make -s -j8 COMMAND /Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI -c port=SWD -d ./${PROJECT_NAME}.elf -s)
   add_custom_target(write_only COMMAND /Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI -c port=SWD -d ./${PROJECT_NAME}.elf -s)
else()
   # Native LINUX
    add_custom_target(write COMMAND make -s -j8 COMMAND /mnt/c/Program\ Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD -d ./${PROJECT_NAME}.elf -s)
   add_custom_target(write_only COMMAND /mnt/c/Program\ Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD -d ./${PROJECT_NAME}.elf -s)
endif()
