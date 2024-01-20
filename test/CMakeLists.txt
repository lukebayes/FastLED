cmake_minimum_required(VERSION 3.13)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../targets/cortex-m0
  ${CMAKE_CURRENT_BINARY_DIR}/src
  )

# Specify the C standard
# set(CMAKE_C_STANDARD 11)

# Mitigate the dipshit cmake compiler test that fails on arm-none-eabi-gcc, g++ and as
# set(CMAKE_C_COMPILER_WORKS 1)
# set(CMAKE_CXX_COMPILER_WORKS 1)
# set(CMAKE_ASM_COMPILER_WORKS 1)

# Enable verbose makefiles
set(CMAKE_VERBOSE_MAKEFILE OFF)

#####################################################
# Configure CMAKE Project
#####################################################

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

enable_language( C CXX ASM )

project(fastled_test)

####################################################
# Car Pico W
####################################################

add_executable(fastled_test
  ${CMAKE_CURRENT_SOURCE_DIR}/main_test.c
  ${FASTLED_TEST_SOURCES}
  ${FASTLED_SOURCES}
  )

target_compile_definitions(fastled_test PRIVATE
  -DQADD8_C=1
  -DSCALE8_C=1
  -DQSUB8_C=1
  )

target_include_directories(fastled_test PRIVATE
  ${FASTLED_INCLUDES}
  ${FASTLED_TEST_INCLUDES}
  )

