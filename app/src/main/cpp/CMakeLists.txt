cmake_minimum_required(VERSION 3.6.0)

# set up common compile options
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -fno-rtti")

set(LibsDir ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs)
set(NDKHelperDir ${LibsDir}/NDKHelper)
set(NDKHelperSrcDir ${NDKHelperDir}/cpp)
set(AssimpDir ${LibsDir}/assimp)
set(FreeTypeDir ${LibsDir}/freetype-2.10.2)

if ((NOT EXISTS ${LibsDir}/glm) OR
(NOT EXISTS ${LibsDir}/glm/glm/glm.h))
    execute_process(COMMAND git clone
            https://github.com/g-truc/glm
            glm
            WORKING_DIRECTORY ${LibsDir})
endif()

# build the ndk-helper library
add_subdirectory( ${NDKHelperSrcDir}
        ${NDKHelperDir}/build/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

add_subdirectory( ${FreeTypeDir}
        ${FreeTypeDir}/build/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

# add external libraries
add_library(assimp SHARED IMPORTED)
# must handle release build cases where the .so file name does not have 'd' postfix.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(assimp PROPERTIES IMPORTED_LOCATION
            ${AssimpDir}/build/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libassimpd.so)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(assimp PROPERTIES IMPORTED_LOCATION
            ${AssimpDir}/build/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libassimp.so)
endif()

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB_RECURSE SoftShadowSrc "*.h" "*.cpp")
add_library(SoftShadow SHARED
        ${SoftShadowSrc})

target_include_directories(SoftShadow PRIVATE
        ${LibsDir}
        ${AssimpDir}/include
        ${AssimpDir}/build/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/include
        ${LibsDir}/glm
        ${FreeTypeDir}/build/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/include)

target_link_libraries(SoftShadow
        PRIVATE
        NdkHelper
        assimp
        freetype
        )