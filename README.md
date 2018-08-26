# Vulkan sprite rendering engine
This is my first attempt at doing something with Vulkan, and is based on the Vulkan
tutorial found at 
https://vulkan-tutorial.com/

## Dependencies

### Vulkan
https://vulkan.lunarg.com/

### GLFW
On macOS:
```bash
brew install glfw --HEAD
```

### glm
On Linux:
```bash
sudo apt install libglm-dev
```

### stb_image

 
## Running on Mac
```bash
VK_LAYER_PATH=~/vulkansdk-macos-1.1.82.0/macOS/etc/vulkan/explicit_layer.d
VK_ICD_FILENAMES=~/vulkansdk-macos-1.1.82.0/macOS/etc/vulkan/icd.d/MoltenVK_icd.json
```
 