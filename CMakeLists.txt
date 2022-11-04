cmake_minimum_required(VERSION 3.4.3)
project(lv2plugins
    VERSION 0.1.0
    LANGUAGES CXX C)

################################################################################
## Compiler setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

################################################################################
##Configuring dependencies
include(FetchContent)

## Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.1.0
)

FetchContent_MakeAvailable(
    Catch2
)

## lv2 
find_package(LV2 REQUIRED)

################################################################################
## show all variables for debugging purpose
function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()
dump_cmake_variables()

################################################################################
# custom build step for generating plugin manifests
add_custom_target(generate_manifest COMMAND ${CMAKE_SOURCE_DIR}/scripts/generate_manifest.sh ${CMAKE_CURRENT_BINARY_DIR})

################################################################################
## Target : common-audio : Static library with various utilities
file(GLOB_RECURSE COMMON_AUDIO_SRC ${CMAKE_SOURCE_DIR}/src/common-audio/*.cpp)
add_library(common-audio STATIC ${COMMON_AUDIO_SRC})
target_include_directories(common-audio PRIVATE ${CMAKE_SOURCE_DIR}/deps)
set_target_properties(common-audio PROPERTIES POSITION_INDEPENDENT_CODE ON)

################################################################################
## Target : Autopitch Plugin
file(GLOB_RECURSE AUTOPITCH_SRC ${CMAKE_SOURCE_DIR}/src/autopitch/*.cpp)
add_library(autopitch MODULE ${AUTOPITCH_SRC})
target_include_directories(autopitch PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(autopitch PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/autopitch.lv2)
target_link_libraries(autopitch PRIVATE common-audio)
add_dependencies(autopitch generate_manifest)
install(DIRECTORY ${PROJECT_BINARY_DIR}/autopitch.lv2 DESTINATION $ENV{HOME}/.lv2)

################################################################################
## Target : ThreeDee Plugin
file(GLOB_RECURSE THREEDEE_SRC ${CMAKE_SOURCE_DIR}/src/threedee/*.cpp)
add_library(threedee MODULE ${THREEDEE_SRC})
target_include_directories(threedee PRIVATE ${CMAKE_SOURCE_DIR}/src ${OPENAL_INCLUDE_DIR})
set_target_properties(threedee PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/threedee.lv2)
target_link_libraries(threedee PRIVATE common-audio)
add_dependencies(threedee generate_manifest)
install(DIRECTORY ${PROJECT_BINARY_DIR}/threedee.lv2 DESTINATION $ENV{HOME}/.lv2)

################################################################################
## Unit tests
file(GLOB_RECURSE UNITTESTS_SRC ${CMAKE_SOURCE_DIR}/src/tests/*.cpp)
add_executable(tests ${UNITTESTS_SRC})
target_link_libraries(tests
    Catch2::Catch2
    common-audio
    dl
)
target_include_directories(tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/deps
    ${CMAKE_SOURCE_DIR}/src
)
include(CTest)
include(Catch)
catch_discover_tests(tests)
enable_testing()
