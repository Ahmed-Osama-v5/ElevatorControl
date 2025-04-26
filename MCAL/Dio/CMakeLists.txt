# DIO Driver CMake configuration
set(DRIVER_NAME Dio)

# Collect driver sources
file(GLOB_RECURSE ${DRIVER_NAME}_SOURCES
    "src/*.c"
    "cfg/*.c"
)

# Create driver library
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE ${${DRIVER_NAME}_SOURCES})

# Include directories for this driver
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cfg
)

#target_link_libraries(${PROJECT_NAME} PUBLIC)

# Set compile options for the driver

#target_compile_options(${PROJECT_NAME} PRIVATE
#    -mmcu=${MCU_NAME}
#    -DF_CPU=${F_CPU}
#    -Os
#    -Wall
#    -Wextra
#)
