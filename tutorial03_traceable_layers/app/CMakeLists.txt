cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${APP_GLUE_DIR})
add_library( app-glue STATIC ${APP_GLUE_DIR}/android_native_app_glue.c)

# build vulkan app
set(SRC_DIR ${ANDROID_NDK}/sources/third_party/vulkan/src)
set(WRAPPER_DIR ../../common/vulkan_wrapper)

add_library(vktuts SHARED
            src/main/jni/TutorialValLayer.cpp
            src/main/jni/main.cpp
            ${WRAPPER_DIR}/vulkan_wrapper.cpp)

include_directories(${WRAPPER_DIR} ${SRC_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror \
                     -Wno-unused-variable \
                     -DVK_USE_PLATFORM_ANDROID_KHR \
                     -DUSE_DEBUG_EXTENTIONS")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
target_link_libraries(vktuts app-glue log android)