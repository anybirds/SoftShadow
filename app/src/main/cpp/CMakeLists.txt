cmake_minimum_required(VERSION 3.6.0)

# set up common compile options
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -fno-rtti")

get_filename_component(LibsDir ../../../libs ABSOLUTE)
get_filename_component(NDKHelperDir ${LibsDir}/NDKHelper ABSOLUTE)
get_filename_component(NDKHelperSrcDir ${NDKHelperDir}/cpp ABSOLUTE)

# build the ndk-helper library${NDKHelperSrc}
add_subdirectory( ${NDKHelperSrcDir}
        ${NDKHelperDir}/NDKHelperBin/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

# clone external library sources
if ((NOT EXISTS ${LibsDir}/stb) OR
(NOT EXISTS ${LibsDir}/stb/stb_image.h))
    execute_process(COMMAND git clone
            https://github.com/nothings/stb.git
            stb
            WORKING_DIRECTORY ${LibsDir})
endif()

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB_RECURSE SoftShadowSrc "*.h" "*.cpp")
add_library(SoftShadow SHARED
        ${SoftShadowSrc})

target_include_directories(SoftShadow PRIVATE ${LibsDir})

target_link_libraries(SoftShadow
        PRIVATE
        NdkHelper
        )