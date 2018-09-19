cmake_minimum_required(VERSION 3.12)
project(vulkan-sprites)

set(CMAKE_CXX_STANDARD 17)

link_libraries(glfw)
link_libraries(vulkan)

if(UNIX)
    set(VULKAN_DIR "$ENV{HOME}/vulkansdk-linux-1.1.82.1/x86_64")
endif(UNIX)

# macOS specific settings
# Needs MoltenVK, plus brew installed packages go in a different location
if(APPLE)
    set(VULKAN_DIR $ENV{HOME}/vulkansdk-macos-1.1.82.0/macOS)
    include_directories(${VULKAN_DIR}/include)
    link_directories(${VULKAN_DIR}/lib)
    link_libraries(MoltenVK)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif(APPLE)

set(GLSL_VALIDATOR "${VULKAN_DIR}/bin/glslangValidator")


# Download and unpack googletest at configure time
configure_file(CMakeLists-googletest.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

add_subdirectory(engine)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(
        first
        samples/first/main.cpp
        samples/first/FirstApp.cpp samples/first/FirstApp.h
)
target_link_libraries(first engine)
target_link_libraries(first Threads::Threads)

add_executable(
        second
        samples/second/main.cpp
        samples/second/SecondApp.cpp samples/second/SecondApp.h)
target_link_libraries(second engine)
target_link_libraries(second Threads::Threads)

add_executable(
        Renderer_test
        engine/Renderer_test.cpp
)
target_link_libraries(Renderer_test engine)
target_link_libraries(Renderer_test gtest_main)

