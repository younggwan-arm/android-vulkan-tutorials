Tutorial Samples
================
A set of samples to illustrate Vulkan API on Android with Android Studio		
To build on windows for tutorial02/03, copy/install ndk-r12 ( or better ) to a directory close to root dir ( c: ) to workaround command path 260 character limit issue;see totorial02/03's build.gradle for details. To Build samples:
- Inside Andriod, menu File --> New --> Import Project
- Find your sample's build.gradle and open it
This will force andrioid studio to create local.properties file to config your SDK and NDK location; if you see error during importing, open local.properties file and correct the NDK and SDK location, then re-sync.


Other Resources:	
Additional Android Studio/NDK samples:    
- [Android Vulkan API Basic Samples](https://github.com/googlesamples/vulkan-basic-samples)
- [Google Play Game Samples](https://github.com/playgameservices/cpp-android-basic-samples)
- [Google Android NDK Samples](https://github.com/googlesamples/android-ndk)

Contents
-------------
1. tutorial01_load_vulkan
- create a vulkan device
1. tutorial02_prebuilt_layers
- create a vulkan device with vulkan validation layers
1. tutorial03_traceable_layers
- create a vulkan device with validation layers that could be traced.
Refer to README.md under its directory
1. tutorial04_first_window
- create a vulkan window with WSI 
1. tutorial05_triangle
- draw a simple triangle with Android Studio integrated shaderc feature
1. tutorial06_texture
- draw textured triangle with [CDep](https://github.com/google/cdep) packed shaderc pre-built binary
- glsl shaders are compiled at app run time (similar to openGL traditional shader compiling model)


Pre-requisites
--------------
- Android N device, API >= 24
- Android Studio 2.2 beta1 or better
- Android NDK
    * [NDK-r12](https://github.com/android-ndk/ndk/wiki), compile as is
    * SDK with the latest cmake package

Test Matrix
------------
| Andrid Studio Version | cmake in SDK| NDK      | Result  |
|-----------------------|-------------|----------|---------|
| 2.2.0-beta1           | 3.6.3133135 | NDK-r12  | Yes     |
| 2.2.0-beta1           | 3.6.3133135 | r13-beta1| NO      |

Known Issue:
 - r13-beta layer source needed to be updated
 - Studio Beta cmake 3.6.3133135 having issues with r13-beta

Getting Started
---------------
1. [Download Android Studio](http://tools.android.com/download/studio/canary)
1. [Download Android NDK-R12+](https://github.com/android-ndk/ndk/wiki)
1. [Download source code](http://www.github.com/googlesamples/android-vulkan-tutorials)
1. Launch Android Studio.
1. "Import Project" of the interested tutorial project to Android Studio
1. Expand "Gradle" panel inside Android Studio, right hand edge of IDE,
then "mathfu" --> "Tasks" --> "other" --> "get_mathfu".
1. Select *"app"* inside Android Studio *"Project"* pane.
1. Click *Tools/Android/Sync Project with Gradle Files*.
1. Click *Run/Run 'app'*.


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





