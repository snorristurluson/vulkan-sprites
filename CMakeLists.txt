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
    set(VULKAN_DIR ~/vulkansdk-macos-1.1.82.0/macOS)
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

add_executable(vulkan-sprites main.cpp app.cpp app.h Renderer.cpp Renderer.h Vertex.cpp Vertex.h Texture.cpp Texture.h stb_image.h ShaderLib.cpp ShaderLib.h)

add_executable(Renderer_test Renderer_test.cpp Renderer.cpp Renderer.h)
target_link_libraries(Renderer_test gtest_main)

# Shaders
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "shaders/*.frag"
        "shaders/*.vert"
        )

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            COMMAND cat ${SPIRV} | xxd -i > ${GLSL}.array
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(vulkan-sprites Shaders)
add_dependencies(Renderer_test Shaders)

add_custom_command(TARGET vulkan-sprites PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vulkan-sprites>/shaders/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_BINARY_DIR}/shaders"
        "$<TARGET_FILE_DIR:vulkan-sprites>/shaders"
        )
