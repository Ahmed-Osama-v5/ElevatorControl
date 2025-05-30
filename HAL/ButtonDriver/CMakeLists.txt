set(MODULE_NAME ButtonDriver)

# Collect driver sources
file(GLOB_RECURSE ${MODULE_NAME}_SOURCES
    "ButtonDriver.c",
    "ButtonDriver_lcfg.c"
)

add_library(${MODULE_NAME} ${${MODULE_NAME}_SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
)

target_link_libraries(${MODULE_NAME} PUBLIC Dio)