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
#ifndef __VALLAYER_HPP__
#define __VALLAYER_HPP__
#include <vulkan_wrapper.h>
#include <iostream>
#include <map>
#include <vector>

/**
 * an enum to indicate request for vkInstance or vkPhysicalDevice
 */
enum class ExtensionType {
    LAYER_EXTENSION,
    DEVICE_EXTENSION,
};

/** A Helper class to manage validation layers and extensions
 * Supposed usage:
 *   1) At instance creation time, validation layer could be enabled by enable all discovered layers
 *        getLayerCount()
 *        getLayerNames()
 *   2) DbgExtension: once instance is created, call
 *        hookDbgReportExt
 *   2) at device creation time, enable all extensions available by:
 *        getExtensionCount()
 *        getExtensionNames()
 */
class LayerAndExtensions {
 public:
  LayerAndExtensions(void);
  ~LayerAndExtensions();

  uint32_t getLayerCount(void);
  const char* const* getLayerNames(void);
  bool isLayerSupported(const char* name);

  uint32_t getExtensionCount(ExtensionType type, void* handle);
  /**
   * getExtensionNames(): return the extension names.
   *     The memory holding the names is valid
   *     as long as the object is not deleted.
   * @param handle a valid VkPhysicalDevice, or VK_NULL_HANDLE if this is for layer extensions
   * @return an array of strings for extension names.
   */
  const char* const* getExtensionNames(ExtensionType type, void* handle);
  bool isExtensionSupported(const char* extName, ExtensionType type, void* handle);

  /*
   * Check for the supported debug report extension, could be:
   *  - VK_EXT_DEBUG_UTILS_EXTENSION_NAME
   *  - VK_EXT_DEBUG_REPORT_EXTENSION_NAME
   *  - nullptr (not supported)
   *  With Validation layer release 1.2.176.1, the first 2 extension are implemented,
   *  so VK_EXT_DEBUG_UTILS_EXTENSION_NAME should be returned.
   */
  const char*  getDbgReportExtName(void);
  bool hookDbgReportExt(VkInstance instance);


  void printLayers(void);        // print layer names to logcat
  void printExtensions(void);    // print extension to logcat
  /**
   * printExtensions()  dump the extensions regarding to layers or implicitly implemented in this
   *     Vulkan implementation.
   * @param layerName the name of layer, or nullptr for this Vulkan implementation
   * @param device VkPhysicalDevice handle if device extensions are of interest too;
   *        or VK_NULL_HANDLE for instance extensions only.
   */
  void printExtensions(const char* layerName, VkPhysicalDevice device); // print extensions in the given layer

 private:
  VkInstance instance_;
  VkDebugReportCallbackEXT vkCallbackHandle_;

  std::map<void*, std::vector<char*>> layersCache_;
  std::map<void*, std::vector<char*>> extCache_;

  // helper functions
  void initDevExtensions(void*);
};

#endif  // __VALLAYER_HPP__
