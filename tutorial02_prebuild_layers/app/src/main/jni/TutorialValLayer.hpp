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
  char** getLayerNames(void);
  bool isLayerSupported(const char* name);

  uint32_t getExtensionCount(ExtensionType type, void* handle);
  char** getExtensionNames(ExtensionType type, void* handle);
  bool isExtensionSupported(ExtensionType type, void* handle, const char* extName);

  bool hookDbgReportExt(VkInstance instance);


  void dumpLayers(void);        // print layer names to logcat
  void dumpExtensions(void);    // print extension to logcat

 private:
  VkInstance instance_;
  VkDebugReportCallbackEXT vkCallbackHandle_;

  std::map<void*, std::vector<char*>> layersCache_;
  std::map<void*, std::vector<char*>> extCache_;

  // helper functions
  void initDevExtensions(void*);
};

#endif  // __VALLAYER_HPP__
