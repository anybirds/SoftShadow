cmake_minimum_required(VERSION 3.6.0)

# set up common compile options
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -fno-rtti")

# build the ndk-helper library
get_filename_component(ndkHelperDir ../../../../NDKHelper ABSOLUTE)
get_filename_component(ndkHelperSrc ../../../../NDKHelper/cpp ABSOLUTE)
add_subdirectory(${ndkHelperSrc}
        ${ndkHelperDir}/ndkHelperBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})
if ((NOT EXISTS ${ndkHelperDir}/stb) OR
(NOT EXISTS ${ndkHelperDir}/stb/stb_image.h))
    execute_process(COMMAND git clone
            https://github.com/nothings/stb.git
            stb
            WORKING_DIRECTORY ${ndkHelperDir})
endif()

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(helloworld SHARED
        helloworld.cpp)

target_include_directories(helloworld PRIVATE ${ndkHelperDir})

target_link_libraries(helloworld
        PRIVATE
        NdkHelper
        )