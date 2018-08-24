cmake_minimum_required(VERSION 3.12)
project(vulkan-sprites)

set(CMAKE_CXX_STANDARD 17)

set(STB_DIR ~/stb)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
link_libraries(glfw)

include_directories(${STB_DIR})

link_libraries(vulkan)
if(APPLE)
    set(VULKAN_DIR ~/vulkansdk-macos-1.1.82.0/macOS)
    include_directories(${VULKAN_DIR}/include)
    link_directories(${VULKAN_DIR}/lib)
    link_libraries(MoltenVK)
endif(APPLE)

add_executable(vulkan-sprites main.cpp app.cpp app.h Renderer.cpp Renderer.h Vertex.cpp Vertex.h)
