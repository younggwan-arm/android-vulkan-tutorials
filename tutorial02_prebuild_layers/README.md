Tutorial 02 - use validation layers - 1
===========
Pre-requirement:
---------------
1. Download prebuilt validation layer android-binaries-*.zip from [Khronos repo](https://github.com/KhronosGroup/Vulkan-ValidationLayers/releases/latest)
1. unzip to $this-project-root/vvl-libs

Added steps from prvious example
-------------------------------
1. Add validation layers into gradle and pack them into apk
1. Enable all validation layers/extensions found on the system
1. Use vulkan wrappers in common/vulkan_wrapper directory

Verification
============
Planted error: this sample sets [`VkDeviceQueueCreateInfo::pQueuePriorities`](https://github.com/googlesamples/android-vulkan-tutorials/blob/master/tutorial02_prebuild_layers/app/src/main/jni/main.cpp#L190) to nullptr,
which will trigger validation layers to notify us from registered callback function
`vkDebugReportCallbackEX_impl();` putting a breakpoint with Android Studo, observe
the breakpoint being triggered.

In logcat, you would see the error message like:
```
E/Vulkan-Debug-Message:: Validation -- Validation Error: [ VUID-VkDeviceQueueCreateInfo-pQueuePriorities-parameter ] Object 0: VK_NULL_HANDLE, type = VK_OBJECT_TYPE_DEVICE; | MessageID = 0xd6d720c6 | vkCreateDevice: required parameter pCreateInfo->pQueueCreateInfos[0].pQueuePriorities specified as NULL. The Vulkan spec states: pQueuePriorities must be a valid pointer to an array of queueCount float values (https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#VUID-VkDeviceQueueCreateInfo-pQueuePriorities-parameter)
```

