# Vulkan sprite rendering engine
This is my first attempt at doing something with Vulkan, and is based on the Vulkan
tutorial found at https://vulkan-tutorial.com/

## Dependencies

### GLFW
On macOS:
```bash
brew install glfw --HEAD
```
 
### Vulkan
https://vulkan.lunarg.com/

## Running on Mac
```bash
VK_LAYER_PATH=~/vulkansdk-macos-1.1.82.0/macOS/etc/vulkan/explicit_layer.d
VK_ICD_FILENAMES=~/vulkansdk-macos-1.1.82.0/macOS/etc/vulkan/icd.d/MoltenVK_icd.json
```
 