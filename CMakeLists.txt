cmake_minimum_required(VERSION 3.4.3)

project(jrplugins
    VERSION 0.1.0
    LANGUAGES CXX C)

################################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

################################################################################
# LV2 dependency
find_package(LV2 REQUIRED)

## Qt5 dependency
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
  
# generate manifest
add_custom_target(generate_manifest COMMAND ${CMAKE_SOURCE_DIR}/scripts/generate_manifest.py ${CMAKE_CURRENT_BINARY_DIR})

#################
## Common code ##
#################
file(GLOB_RECURSE COMMON_SRC ${CMAKE_SOURCE_DIR}/src/common/*.cpp)
add_library(common STATIC ${COMMON_SRC})
set_target_properties(common PROPERTIES POSITION_INDEPENDENT_CODE ON)

###############
## Autopitch ##
###############
file(GLOB_RECURSE AUTOPITCH_SRC ${CMAKE_SOURCE_DIR}/src/autopitch/*.cpp)
add_library(autopitch MODULE ${AUTOPITCH_SRC})
target_include_directories(autopitch PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(autopitch PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/autopitch.lv2)
target_link_libraries(autopitch PRIVATE common)
add_dependencies(autopitch generate_manifest)

################################################################################
install(DIRECTORY ${PROJECT_BINARY_DIR}/autopitch.lv2 DESTINATION $ENV{HOME}/.lv2)
