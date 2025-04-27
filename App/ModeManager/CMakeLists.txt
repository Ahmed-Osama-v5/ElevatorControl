set(MODULE_NAME ModeManager)

# Collect driver sources
file(GLOB_RECURSE ${MODULE_NAME}_SOURCES
    "src/*.c"
)


add_library(${MODULE_NAME} ${${MODULE_NAME}_SOURCES})

#target_sources(${PROJECT_NAME} PRIVATE ${${MODULE_NAME}_SOURCES})

target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
)
