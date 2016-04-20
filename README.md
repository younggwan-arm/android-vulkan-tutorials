Tutorial Samples
================
A set of samples to illustrate Vulkan API on Android
Note: to build on windows, please use branch "build-windows" from this repo to temporarily workaround windows clang problem

Pre-requisites
--------------
- Android Studio 2.1 preview 6
- Android NDK
    * [NDK-r12](https://github.com/android-ndk/ndk/wiki), compile as is
    * [NDK-r11](http://developer.android.com/ndk/downloads/index.html) needs shaderc from ndk-r12, and validation layer code from [github](https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers) *android_layers* branch
    * NDK-R10x and earlier needs shaderc, validation layer source code, and vulkan headers from ndk-r11 --copy headers from NDK-r11 *android-24/your-abi/usr/include/vulkan* to your own include directory
    * Project build.gradle needs to be modified to compile with ndk version that is NOT R12

Getting Started
---------------
1. [Download Android Studio](http://developer.android.com/sdk/index.html)
1. [Download Android NDK-R12](https://github.com/android-ndk/ndk/wiki)
1. [Download source code](http://www.github.com/googlesamples/android-vulkan-tutorials)
1. Launch Android Studio.
1. "Import Project" of the interested tutorial project to Android Studio
1. Expand "Gradle" panel inside Android Studio, right hand edge of IDE,
then "mathfu" --> "Tasks" --> "other" --> "get_mathfu".
1. Select *"app"* inside Android Studio *"Project"* pane.
1. Click *Tools/Android/Sync Project with Gradle Files*.
1. Click *Run/Run 'app'*.


Tutorial List
-------------
1. tutorial01_load_vulkan
    - create a vulkan device
1. tutorial02_prebuilt_layers
    - create a vulkan device with vulkan validation layers
1. tutorial03_traceable_layers
    - create a vulkan device with validation layers that could debug into
1. tutorial04_first_window
    - create a vulkan window with WSI 
1. tutorial05_triangle
    - draw a simple triangle with android shaderc feature


License
-------
Copyright 2016 Google, Inc.

Licensed to the Apache Software Foundation (ASF) under one or more contributor
license agreements.  See the NOTICE file distributed with this work for
additional information regarding copyright ownership.  The ASF licenses this
file to you under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy of
the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
License for the specific language governing permissions and limitations under
the License.





