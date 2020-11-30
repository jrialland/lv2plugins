cmake_minimum_required(VERSION 3.4.3)
project(lv2plugins
    VERSION 0.1.0
    LANGUAGES CXX C)

################################################################################
## Compiler setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

################################################################################
##Configuring dependencies

# LV2 dependency
find_package(LV2 REQUIRED)

## Qt5 dependency
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)

## Catch2 dependency
find_package(Catch2 REQUIRED)

################################################################################
# custom build step for generating plugin manifests
add_custom_target(generate_manifest COMMAND ${CMAKE_SOURCE_DIR}/scripts/generate_manifest.py ${CMAKE_CURRENT_BINARY_DIR})

################################################################################
## Unit tests
file(GLOB_RECURSE UNITTESTS_SRC ${CMAKE_SOURCE_DIR}/src/unittests/*.cpp)
file(GLOB_RECURSE COMMON_AUDIO_SRC ${CMAKE_SOURCE_DIR}/src/common-audio/*.cpp)
add_executable(unittests ${COMMON_AUDIO_SRC} ${UNITTESTS_SRC})
target_link_libraries(unittests Catch2::Catch2)
target_include_directories(unittests PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/deps)
include(CTest)
include(Catch)
catch_discover_tests(unittests)

################################################################################
## common-audio : Static library for all the lv2 audio-related code
file(GLOB_RECURSE COMMON_AUDIO_SRC ${CMAKE_SOURCE_DIR}/src/common-audio/*.cpp)
add_library(common-audio STATIC ${COMMON_AUDIO_SRC})
target_include_directories(common-audio PRIVATE ${CMAKE_SOURCE_DIR}/deps)
set_target_properties(common-audio PROPERTIES POSITION_INDEPENDENT_CODE ON)

################################################################################
## common-ui : Static library for all the lv2 ui-related code
file(GLOB_RECURSE COMMON_UI_SRC ${CMAKE_SOURCE_DIR}/src/common-ui/*.cpp)
add_library(common-ui STATIC ${COMMON_UI_SRC})
set_target_properties(common-ui PROPERTIES POSITION_INDEPENDENT_CODE ON)
qt5_use_modules(common-ui Widgets)

################################################################################
###############
## Autopitch ##
###############

file(GLOB_RECURSE AUTOPITCH_SRC ${CMAKE_SOURCE_DIR}/src/autopitch/*.cpp)
add_library(autopitch MODULE ${AUTOPITCH_SRC})
target_include_directories(autopitch PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(autopitch PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/autopitch.lv2)
target_link_libraries(autopitch PRIVATE common-audio)
add_dependencies(autopitch generate_manifest)
install(DIRECTORY ${PROJECT_BINARY_DIR}/autopitch.lv2 DESTINATION $ENV{HOME}/.lv2)

file(GLOB_RECURSE AUTOPITCH_UI_SRC ${CMAKE_SOURCE_DIR}/src/autopitch-UI/*.cpp)
add_library(autopitch-UI MODULE ${AUTOPITCH_UI_SRC})
target_include_directories(autopitch-UI PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(autopitch-UI PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/autopitch-UI.lv2)
target_link_libraries(autopitch-UI PRIVATE common-ui)
add_dependencies(autopitch-UI generate_manifest)
install(DIRECTORY ${PROJECT_BINARY_DIR}/autopitch-UI.lv2 DESTINATION $ENV{HOME}/.lv2)

