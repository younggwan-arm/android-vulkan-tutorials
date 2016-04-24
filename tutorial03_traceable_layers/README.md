Tutorial 03 - use validation layers - 2
===========
Add gradle build for validation layers as app's jniLib dependencies
so inaries are packed into APK automatically and traceale at run-time
(layers could be stepped into when debugging application)
Enable all validation layers/extensions found on the system
Using vulkan wrappers in common/vulkan_wrapper directory	
**Windows build extra steps**	
if you see errors while building validation layers, try the following workaround:	
- copy your ndk to be directly under c:\
- configure your ndk.dir inside local.properties to your new ndk location
this is due to the fact that commend path is above 260 when source is inside ndk

