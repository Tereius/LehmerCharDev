# Find the kernel release
execute_process(
  COMMAND uname -r
  OUTPUT_VARIABLE KERNEL_RELEASE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Find the headers
find_path(KERNELHEADERS_DIR
  include/linux/user.h
  PATHS /usr/src/linux-headers-${KERNEL_RELEASE} /lib/modules/${KERNEL_RELEASE}/build
)

if(KERNELHEADERS_DIR)
    include_directories(SYSTEM ${KERNELHEADERS_DIR})
    message(STATUS "Using Kernel headers: ${KERNELHEADERS_DIR}")
else()
    message(SEND_ERROR "Kernel headers for ${KERNEL_RELEASE} not found")
endif()
