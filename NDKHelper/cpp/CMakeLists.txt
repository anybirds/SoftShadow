# build native_app_glue as a static lib
cmake_minimum_required(VERSION 3.4.1)

include(AndroidNdkModules)
android_ndk_import_module_native_app_glue()

add_library(NdkHelper
        STATIC
        JNIHelper.cpp
        )
set_target_properties(NdkHelper
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:NdkHelper,INCLUDE_DIRECTORIES>
        )
target_include_directories(NdkHelper
        PUBLIC
        .
        )

target_link_libraries(NdkHelper
        PUBLIC
        native_app_glue
        GLESv3
        EGL
        log
        android
        atomic
        )