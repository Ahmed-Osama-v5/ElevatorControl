set(MODULE_NAME ElevatorController)

# Collect driver sources
file(GLOB_RECURSE ${MODULE_NAME}_SOURCES
    "src/*.c"
)

add_library(${MODULE_NAME} ${${MODULE_NAME}_SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
)

target_link_libraries(${MODULE_NAME} PUBLIC 
    CallHandler
    LEDController
    MotionController
    Diagnostics
    Menu

    ButtonDriver
    Display
    SensorManager
    RelayManager
    SegmentDriver

    EepD
    Timer
)