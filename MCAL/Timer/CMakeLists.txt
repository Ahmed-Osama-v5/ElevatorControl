# Timer Driver CMake configuration
set(DRIVER_NAME Timer)

# Collect driver sources
file(GLOB_RECURSE ${DRIVER_NAME}_SOURCES
    "src/*.c"
    "cfg/*.c"
)

target_sources(${PROJECT_NAME} PRIVATE ${${DRIVER_NAME}_SOURCES})

# Include directories for this driver
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cfg
)
