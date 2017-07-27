cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${APP_GLUE_DIR})
add_library( app-glue STATIC ${APP_GLUE_DIR}/android_native_app_glue.c)

# build vulkan app
set(VK_WRAPPER_DIR ../../../../../common/vulkan_wrapper)
set(VK_VAL_LAYER_SRC_DIR ${ANDROID_NDK}/sources/third_party/vulkan/src)

add_library(vktuts SHARED
            TutorialValLayer.cpp
            main.cpp
            ${VK_WRAPPER_DIR}/vulkan_wrapper.cpp)

include_directories(${VK_WRAPPER_DIR} ${VK_VAL_LAYER_SRC_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror       \
                     -Wno-unused-variable -DUSE_DEBUG_EXTENTIONS \
                     -DVK_USE_PLATFORM_ANDROID_KHR")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

target_link_libraries(vktuts app-glue log android)