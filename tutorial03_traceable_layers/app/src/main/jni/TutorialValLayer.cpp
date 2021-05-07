// Copyright 2016 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "TutorialValLayer.hpp"

#include <android/log.h>

#include <cassert>

// Android log function wrappers
static const char* kTAG = "Vulkan-Tutorial02";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

// Vulkan call wrapper
#define CALL_VK(func)                                                 \
  if (VK_SUCCESS != (func)) {                                         \
    __android_log_print(ANDROID_LOG_ERROR, "Tutorial ",               \
                        "Vulkan error. File[%s], line[%d]", __FILE__, \
                        __LINE__);                                    \
    assert(false);                                                    \
  }

/**
 * Validation Layer name
 */
static const char* kValLayerName = "VK_LAYER_KHRONOS_validation";

/**
 * Debug Extension names.
 */
static const char* kDbgReportExtName = "VK_EXT_debug_report";
static const char* kDbgReportUtilsName = "VK_EXT_debug_utils";

// Simple Dbg Callback function to be used by Vk engine
static VkBool32 VKAPI_PTR vkDebugReportCallbackEX_impl(
    VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
    uint64_t object, size_t location, int32_t messageCode,
    const char* pLayerPrefix, const char* pMessage, void* pUserData) {
  // pUserData is not usable as we did not set it up when we were registering
  // the callback
  if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
    __android_log_print(ANDROID_LOG_INFO, "Vulkan-Debug-Message: ", "%s -- %s",
                        pLayerPrefix, pMessage);
  }
  if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
    __android_log_print(ANDROID_LOG_WARN, "Vulkan-Debug-Message: ", "%s -- %s",
                        pLayerPrefix, pMessage);
  }
  if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    __android_log_print(ANDROID_LOG_WARN,
                        "Vulkan-Debug-Message-(Perf): ", "%s -- %s",
                        pLayerPrefix, pMessage);
  }
  if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    __android_log_print(ANDROID_LOG_ERROR, "Vulkan-Debug-Message: ", "%s -- %s",
                        pLayerPrefix, pMessage);
  }
  if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
    __android_log_print(ANDROID_LOG_DEBUG, "Vulkan-Debug-Message: ", "%s -- %s",
                        pLayerPrefix, pMessage);
  }

  return VK_FALSE;
}

/**
 * Helper function cloneString(): allocation and copy a string,
 * src is never null.
 */
static char* cloneString(const char* src) {
  char* dst = new char[strlen(src) + 1];
  strcpy(dst, src);
  return dst;
}

/**
 * Helper function AddExtNames():
 *   move extension names in the given properties array into extension cache.
 */
static bool addExtNames(std::vector<char*>& extStore, uint32_t count,
                        VkExtensionProperties* properties) {
  if (!properties) return false;
  for (uint32_t i = 0; i < count; i++) {
    char* name = cloneString(properties[i].extensionName);
    extStore.push_back(name);
  }
  return true;
}

/**
 * Constructor: initialize layer cache
 */
LayerAndExtensions::LayerAndExtensions(void) {
  layersCache_.clear();
  extCache_.clear();

  // Retrieve instance extensions from the underline Vulkan implementation
  std::vector<char*> extNames;
  uint32_t count = 0;
  CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));
  if (count) {
    VkExtensionProperties* prop = new VkExtensionProperties[count];
    CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &count, prop));
    addExtNames(extNames, count, prop);
    delete[] prop;
  }

  uint32_t layerCount;
  VkLayerProperties* layerProp = nullptr;
  CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
  if (layerCount) {
    layerProp = new VkLayerProperties[layerCount];
    CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, layerProp));
  }
  std::vector<char*> layers;
  for (uint32_t i = 0; i < layerCount; i++) {
    char* name = cloneString(layerProp[i].layerName);
    layers.push_back(name);

    CALL_VK(vkEnumerateInstanceExtensionProperties(name, &count, nullptr));
    if (count == 0) continue;

    VkExtensionProperties* prop = new VkExtensionProperties[count];
    CALL_VK(vkEnumerateInstanceExtensionProperties(name, &count, prop));
    addExtNames(extNames, count, prop);
    delete[] prop;
  }
  layersCache_[VK_NULL_HANDLE] = layers;
  extCache_[VK_NULL_HANDLE] = extNames;

  if (layerProp) delete[] layerProp;
}

/**
 * Report available layer names
 * @return: an array of layer name pointers, NULL if no layers are available
 */
const char* const* LayerAndExtensions::getLayerNames(void) {
  auto layers = layersCache_.find(VK_NULL_HANDLE);
  if (layers == layersCache_.end() || layers->second.empty()) {
    return nullptr;
  }

  return static_cast<char**>(layers->second.data());
}

/**
 * Report out available layers on the system + inside APK's native libs
 * @return available layers for the instance.
 */
uint32_t LayerAndExtensions::getLayerCount(void) {
  uint32_t count = 0;
  auto layers = layersCache_.find(VK_NULL_HANDLE);
  if (layers == layersCache_.end()) {
    return count;
  }

  count = layers->second.size();
  LOGI("InstLayerCount = %d", count);
  return count;
}

const char* const* LayerAndExtensions::getExtensionNames(ExtensionType type,
                                                         void* handle) {
  if (type == ExtensionType::LAYER_EXTENSION) {
    assert(handle == VK_NULL_HANDLE);
  } else {
    initDevExtensions(handle);
  }
  auto it = extCache_.find(handle);
  if (it == extCache_.end() || it->second.empty()) {
    LOGE("No cache for %p in %s", handle, __FUNCTION__);
    return nullptr;
  }

  return static_cast<char**>(it->second.data());
}

/**
 * Query for the available extensions, including the ones inside available
 * layers
 * @param type one of ExtensionType enums
 * @param handle either VK_NULL_HANDLE for instance extensions, or physical
 * device handles
 * @return available extensions for the queried type.
 */
uint32_t LayerAndExtensions::getExtensionCount(ExtensionType type,
                                               void* handle) {
  if (type == ExtensionType::LAYER_EXTENSION) {
    assert(handle == VK_NULL_HANDLE);
  } else {
    initDevExtensions(handle);
  }
  auto it = extCache_.find(handle);
  if (it == extCache_.end()) {
    LOGE("No cache for %p in %s", handle, __FUNCTION__);
    return 0;
  }
  return static_cast<uint32_t>(it->second.size());
}

bool LayerAndExtensions::isExtensionSupported(const char* extName,
                                              ExtensionType type,
                                              void* handle) {
  if (type == ExtensionType::LAYER_EXTENSION) {
    assert(handle == VK_NULL_HANDLE);
  } else {
    initDevExtensions(handle);
  }
  auto it = extCache_.find(handle);
  if (it == extCache_.end()) {
    LOGE("No cache for %p in %s", handle, __FUNCTION__);
    return false;
  }
  for (auto name : it->second) {
    if (!strcmp(name, extName)) {
      return true;
    }
  }
  return false;
}

/**
 * Check whether the layer is supported. layers are common to instance and
 * devices, this simply check whether the given layer name is inside the
 * pre-built layerCache_
 * @param layerName the layer to check for supportability
 * @return true: supported, false: otherwise
 */
bool LayerAndExtensions::isLayerSupported(const char* layerName) {
  auto layers = layersCache_.find(VK_NULL_HANDLE);
  if (layers == layersCache_.end()) {
    // Internal cache is not built up, due to no layers found.
    return false;
  }

  // check for supportability
  for (auto name : layers->second) {
    if (!strcmp(name, layerName)) return true;
  }

  return false;
}

/**
 * Build up device extension cache, add to extCache_. Cache only gets built
 * once.
 * @param device VkPhysicalDevice
 */
void LayerAndExtensions::initDevExtensions(void* device) {
  auto it = extCache_.find(device);
  if (it != extCache_.end()) {
    // already in cache, no need to re-build.
    return;
  }
  VkPhysicalDevice physicalDev = reinterpret_cast<VkPhysicalDevice>(device);

  // get all supported layers props
  uint32_t count = 0;
  VkLayerProperties* properties = nullptr;
  CALL_VK(vkEnumerateDeviceLayerProperties(physicalDev, &count, nullptr));
  if (count) {
    properties = new VkLayerProperties[count];
    CALL_VK(vkEnumerateDeviceLayerProperties(physicalDev, &count, properties));
  }

  // Retrieve names from layers and cache them.
  std::vector<char*> layerNames;
  std::vector<char*> extNames;
  for (int i = 0; i < count; i++) {
    LOGI("layerName: %s for device %p", properties[i].layerName, physicalDev);
    char* name = cloneString(properties[i].layerName);
    layerNames.push_back(name);

    // Pull extensions supported by the layer and append to the extension list
    uint32_t extCount = 0;
    CALL_VK(vkEnumerateDeviceExtensionProperties(physicalDev, name, &extCount,
                                                 nullptr));
    if (extCount) {
      VkExtensionProperties* extProp = new VkExtensionProperties[extCount];
      CALL_VK(vkEnumerateDeviceExtensionProperties(physicalDev, layerNames[i],
                                                   &extCount, extProp));
      addExtNames(extNames, extCount, extProp);
      delete[] extProp;
    }
  }
  layersCache_[physicalDev] = layerNames;
  if (properties) delete[] properties;

  // Get all implicitly supported extension properties at this physical device
  // level
  uint32_t extCount = 0;
  CALL_VK(vkEnumerateDeviceExtensionProperties(physicalDev, nullptr, &extCount,
                                               nullptr));
  if (extCount) {
    VkExtensionProperties* extProp = new VkExtensionProperties[extCount];
    CALL_VK(vkEnumerateDeviceExtensionProperties(physicalDev, nullptr,
                                                 &extCount, extProp));
    addExtNames(extNames, extCount, extProp);
    delete[] extProp;
  }

  extCache_[physicalDev] = extNames;
}

/**
 * layer and extension printing
 */
void LayerAndExtensions::printLayers(void) {
  if (layersCache_.empty()) return;
  for (auto& layer : layersCache_) {
    LOGI("Available Layers for %p: ", layer.first);
    for (auto& name : layer.second) {
      LOGI("%s", name);
    }
  }
}

void LayerAndExtensions::printExtensions(void) {
  if (extCache_.empty()) return;

  for (auto& item : extCache_) {
    LOGI("Extensions for %p:", item.first);
    for (auto& name : item.second) {
      LOGI("%s", name);
    }
  }
}

void LayerAndExtensions::printExtensions(const char* name,
                                         VkPhysicalDevice device) {
  uint32_t count = 0;

  const char* layerName = name ? name : "Vulkan Implementation";
  CALL_VK(vkEnumerateInstanceExtensionProperties(name, &count, nullptr));
  if (count == 0) return;

  VkExtensionProperties* prop = new VkExtensionProperties[count];
  CALL_VK(vkEnumerateInstanceExtensionProperties(name, &count, prop));

  if (count == 0) return;
  LOGI("Instance Extensions in layer: %s", layerName);
  for (uint32_t i = 0; i < count; i++) {
    LOGI("Name: %s, Ver: %#x", prop[i].extensionName, prop[i].specVersion);
  }
  delete[] prop;

  if (device == VK_NULL_HANDLE) return;
  CALL_VK(vkEnumerateDeviceExtensionProperties(device, name, &count, nullptr));
  if (count == 0) return;

  prop = new VkExtensionProperties[count];
  CALL_VK(vkEnumerateDeviceExtensionProperties(device, name, &count, prop));
  LOGI("Device Extensions in layer: %s", layerName);
  for (uint32_t i = 0; i < count; i++) {
    LOGI("Name: %s, Ver: %#x", prop[i].extensionName, prop[i].specVersion);
  }
  delete[] prop;
}

/**
 * Clean up caches upon exit.
 */
LayerAndExtensions::~LayerAndExtensions() {
  if (!layersCache_.empty()) {
    for (auto& layer : layersCache_) {
      for (auto& name : layer.second) {
        delete[] name;
      }
    }
  }

  if (!extCache_.empty()) {
    for (auto& item : extCache_) {
      for (auto& name : item.second) {
        delete[] name;
      }
    }
  }
}

/**
 * Register our vkDebugReportCallbackEX_impl function to Vulkan so we could
 * process callbacks.
 * @param instance
 * @return
 */
bool LayerAndExtensions::hookDbgReportExt(VkInstance instance) {
  if (!isExtensionSupported(kDbgReportExtName, ExtensionType::DEVICE_EXTENSION,
                            VK_NULL_HANDLE)) {
    return false;
  }
  if (!vkCreateDebugReportCallbackEXT) {
    vkCreateDebugReportCallbackEXT =
        (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugReportCallbackEXT");
    vkDestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugReportCallbackEXT");
    vkDebugReportMessageEXT =
        (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(
            instance, "vkDebugReportMessageEXT");
  }

  VkDebugReportCallbackCreateInfoEXT dbgInfo = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
      .pNext = nullptr,
      .flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
               VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
               VK_DEBUG_REPORT_ERROR_BIT_EXT,
      .pfnCallback = vkDebugReportCallbackEX_impl,
      .pUserData = nullptr,  // no userdata we this object is not in memory
                             // anymore when debug callback is happening
  };

  // Intend to keep the callback alive for the full Vulkan instance life cycle,
  // not caching the returned callback handle, not calling
  // vkDestroyDebugReportCallbackEXT either.
  VkDebugReportCallbackEXT callbackHandle;
  CALL_VK(vkCreateDebugReportCallbackEXT(instance, &dbgInfo, nullptr,
                                         &callbackHandle));
  return true;
}
