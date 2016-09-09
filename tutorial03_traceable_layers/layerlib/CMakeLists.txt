cmake_minimum_required(VERSION 3.4.1)
include(ExternalProject)

# build layer_utils as a static lib
set(NDK_SRC_DIR ${ANDROID_NDK}/sources)
set(SRC_DIR ${NDK_SRC_DIR}/third_party/vulkan/src)
set(LAYER_DIR ${SRC_DIR}/build-android/generated)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wno-unused-function \
                     -Wno-unused-const-variable -Wno-c++98-compat-pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(${SRC_DIR}/include
                    ${SRC_DIR}/layers
                    ${SRC_DIR}/loader)
add_library(layer_utils STATIC
        ${LAYER_DIR}/common/vk_layer_config.cpp
        ${LAYER_DIR}/common/vk_layer_extension_utils.cpp
        ${LAYER_DIR}/common/vk_layer_utils.cpp)

# build core_validation layers which including shader validation
set(core_validation_SRCS
        ${LAYER_DIR}/layer-src/core_validation/core_validation.cpp
        ${LAYER_DIR}/common/vk_layer_table.cpp )
if(NOT (${ANDROID_NDK_REVISION} LESS 13))
    list(APPEND core_validation_SRCS
         ${LAYER_DIR}/layer-src/core_validation/descriptor_sets.cpp
         ${LAYER_DIR}/layer-src/core_validation/vk_safe_struct.cpp)
endif(NOT (${ANDROID_NDK_REVISION} LESS 13))

# piggyback on the previous CXX_FLAGS
include_directories(${LAYER_DIR}/include ${SRC_DIR}/../glslang)
add_library(VkLayer_core_validation SHARED ${core_validation_SRCS})
target_link_libraries(VkLayer_core_validation log android layer_utils)

if(NOT (${ANDROID_NDK_REVISION} LESS 13))
    # ndk-r13 validation layers use shaderc, need to compile it
    include_directories(${NDK_SRC_DIR}/third_party/shaderc/third_party/glslang
                        ${NDK_SRC_DIR}/third_party/shaderc/third_party/spirv-tools/include)

    set(SHADERC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/shaderc)
    set(lib_shaderc ${SHADERC_OUT_DIR}/gnustl_static/${ANDROID_ABI}/libshaderc.a)
    set(SHADERC_SRC_DIR ${NDK_SRC_DIR}/third_party/shaderc)
    if(CMAKE_HOST_WIN32)
        ExternalProject_Add(BuildShaderc
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ${ANDROID_NDK}/ndk-build.cmd --jobs=8 NDK_DEBUG=1
                          NDK_PROJECT_PATH=${SHADERC_SRC_DIR}
                          NDK_OUT=./shaderc NDK_LIBS_OUT=${SHADERC_OUT_DIR}
                          NDK_APPLICATION_MK=${CMAKE_CURRENT_SOURCE_DIR}/Application.mk
                          APP_BUILD_SCRIPT=${SHADERC_SRC_DIR}/Android.mk
                          libshaderc_combined
            LOG_CONFIGURE 1
            BUILD_ALWAYS 1
            LOG_BUILD    1
            BUILD_BYPRODUCTS ${lib_shaderc}
            INSTALL_COMMAND "")
    else(CMAKE_HOST_WIN32)
        ExternalProject_Add(BuildShaderc
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
            CONFIGURE_COMMAND mkdir -p ${SHADERC_OUT_DIR}
            BUILD_COMMAND ${ANDROID_NDK}/ndk-build --jobs=8 NDK_DEBUG=1
                          NDK_PROJECT_PATH=${SHADERC_SRC_DIR}
                          NDK_LIBS_OUT=${SHADERC_OUT_DIR}
                          NDK_APPLICATION_MK=${CMAKE_CURRENT_SOURCE_DIR}/Application.mk
                          APP_BUILD_SCRIPT=${SHADERC_SRC_DIR}/Android.mk
                          libshaderc_combined     # make the combined lib to use
            BUILD_ALWAYS 1    # Force build
            BUILD_BYPRODUCTS ${lib_shaderc}
            INSTALL_COMMAND "")
    endif(CMAKE_HOST_WIN32)

    add_dependencies(VkLayer_core_validation BuildShaderc)
    target_link_libraries(VkLayer_core_validation ${lib_shaderc})
endif(NOT (${ANDROID_NDK_REVISION} LESS 13))

# device_limit layer is for ndk-r12 only, NDK-R13 merged it into core_validation
if(${ANDROID_NDK_REVISION} LESS 13)
add_library(VkLayer_device_limits SHARED
            ${LAYER_DIR}/layer-src/device_limits/device_limits.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_device_limits log android layer_utils)
endif(${ANDROID_NDK_REVISION} LESS 13)

add_library(VkLayer_image SHARED
            ${LAYER_DIR}/layer-src/image/image.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_image log android layer_utils)

add_library(VkLayer_parameter_validation SHARED
            ${LAYER_DIR}/layer-src/parameter_validation/parameter_validation.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_parameter_validation log android layer_utils)

add_library(VkLayer_object_tracker SHARED
            ${LAYER_DIR}/layer-src/object_tracker/object_tracker.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_object_tracker log android layer_utils)

add_library(VkLayer_threading SHARED
            ${LAYER_DIR}/layer-src/threading/threading.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_threading log android layer_utils)

add_library(VkLayer_unique_objects SHARED
            ${LAYER_DIR}/layer-src/unique_objects/unique_objects.cpp
            ${LAYER_DIR}/layer-src/unique_objects/vk_safe_struct.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_unique_objects log android layer_utils)

add_library(VkLayer_swapchain SHARED
            ${LAYER_DIR}/layer-src/swapchain/swapchain.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_swapchain log android layer_utils)
