# Vulkan sprite rendering engine
The primary driver for this project is to learn to use Vulkan, but I 
also want to try some things in the overall architecture
and infrastructure of a game engine.

Follow [my blog](https://snorristurluson.github.io/) for
discussions about my learnings along the way.

The initial iteration of the renderer is based on the Vulkan tutorial
found at https://vulkan-tutorial.com/ - I highly recommend it if you
are new to Vulkan, and I've looked to
[Sascha Willems](https://github.com/SaschaWillems/Vulkan) for 
reference as well.

## Dependencies

### Vulkan
https://vulkan.lunarg.com/

Install according to the instructions per platform and adjust the CMakeLists.txt file
accordingly.

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

### spdlog
On Linux:
```bash
sudo apt install libspdlog-dev
```

 
## Running on Mac
```bash
VK_LAYER_PATH=~/vulkansdk-macos-1.1.82.0/macOS/etc/vulkan/explicit_layer.d
VK_ICD_FILENAMES=~/vulkansdk-macos-1.1.82.0/macOS/etc/vulkan/icd.d/MoltenVK_icd.json
```
Remember to do this for any target you want to run, including tests.
 