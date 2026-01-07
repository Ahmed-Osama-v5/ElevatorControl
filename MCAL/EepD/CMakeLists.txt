# DIO Driver CMake configuration
set(DRIVER_NAME EepD)

# Collect driver sources
file(GLOB_RECURSE ${DRIVER_NAME}_SOURCES
    "src/*.c"
)

# Create driver library

add_library(${DRIVER_NAME} ${${DRIVER_NAME}_SOURCES})

# Include directories for this driver
target_include_directories(${DRIVER_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)
