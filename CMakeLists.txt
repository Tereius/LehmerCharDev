cmake_minimum_required(VERSION 2.8.12)

project(LehmerCharDev)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(FindKernelHeaders)

set(SRC_FILE lehmerCharDev.ko)

if(NOT "${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
    message(SEND_ERROR "Only the generator \"Unix Makefiles\" is supported")
endif()

set(KBUILD_CMD "${CMAKE_MAKE_PROGRAM}" -C ${KERNELHEADERS_DIR} M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR})

# Generate the Kbuild file through cmake.
FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Kbuild "obj-m := lehmerCharDev.o")

add_custom_command(OUTPUT ${SRC_FILE}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS lehmerCharDev.c VERBATIM)
        
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/inst.sh" "#!/bin/bash
dev=\"lehmer_character_device\"
major=\"$(sudo grep \"$dev\" /proc/devices | cut -d ' ' -f 1)\"
sudo mknod \"/dev/$dev\" c \"$major\" 0
")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/uinst.sh" "#!/bin/bash
dev=\"lehmer_character_device\"
major=\"$(grep \"$dev\" /proc/devices | cut -d ' ' -f 1)\"
mknod \"/dev/$dev\" c \"$major\" 0
")
        
add_custom_command(TARGET addDev
    COMMAND source "${CMAKE_CURRENT_BINARY_DIR}/inst.sh"
    VERBATIM
)
     
#insmod /character_device.ko
#dev="lkmc_character_device"
#major="$(grep "$dev" /proc/devices | cut -d ' ' -f 1)"
#mknod "/dev/$dev" c "$major" 0
#cat /dev/lkmc_character_device
#=> abcd
#rm /dev/lkmc_character_device
#rmmod character_device
     
add_custom_target(driver ALL DEPENDS ${SRC_FILE})
