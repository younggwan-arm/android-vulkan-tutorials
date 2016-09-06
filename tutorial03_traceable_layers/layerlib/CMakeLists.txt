cmake_minimum_required(VERSION 3.4.1)

if(ANDROID_NDK_REVISION GREATER 12)
  message(FATAL_ERROR "For NDK version 13 and above, please use idk-build support to build")
endif (ANDROID_NDK_REVISION GREATER 12)

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

# build vulkan layers
set(VK_WRAPPER_DIR ../../../../../common/vulkan_wrapper)

# piggy back on the previous CXX_FLAGS
include_directories(${LAYER_DIR}/include ${SRC_DIR}/../glslang)

add_library(VkLayer_core_validation SHARED
            ${LAYER_DIR}/layer-src/core_validation/core_validation.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp )
target_link_libraries(VkLayer_core_validation log android layer_utils)

add_library(VkLayer_device_limits SHARED
            ${LAYER_DIR}/layer-src/device_limits/device_limits.cpp
            ${LAYER_DIR}/common/vk_layer_table.cpp)
target_link_libraries(VkLayer_device_limits log android layer_utils)

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
