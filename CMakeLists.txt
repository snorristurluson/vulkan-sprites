cmake_minimum_required(VERSION 3.12)
project(vulkan-sprites)

set(CMAKE_CXX_STANDARD 17)

link_libraries(glfw)
link_libraries(vulkan)


# macOS specific settings
# Needs MoltenVK, plus brew installed packages go in a different location
if(APPLE)
    set(VULKAN_DIR $ENV{HOME}/vulkansdk-macos-1.1.82.0/macOS)
    include_directories(${VULKAN_DIR}/include)
    link_directories(${VULKAN_DIR}/lib)
    link_libraries(MoltenVK)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
elseif(UNIX)
    set(VULKAN_DIR "$ENV{HOME}/vulkansdk-linux-1.1.82.1/x86_64")
    include_directories($ENV{HOME}/telemetry/3.0.1.1461/include)
    link_directories($ENV{HOME}/telemetry/3.0.1.1461/lib)
    link_libraries(rad_tm_linux_x64)
    add_compile_definitions(TELEMETRY_ENABLED=1)
endif(UNIX)


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

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

include_directories(engine)

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
        samples/second/SecondApp.cpp
        samples/second/SecondApp.h
)
target_link_libraries(second engine)
target_link_libraries(second Threads::Threads)

add_executable(
        text
        samples/text/main.cpp
        samples/text/TextApp.cpp samples/text/TextApp.h)
target_link_libraries(text engine Threads::Threads ${FREETYPE_LIBRARIES})

add_executable(
        textureatlas
        samples/textureatlas/main.cpp
        samples/textureatlas/TextureAtlasApp.cpp
        samples/textureatlas/TextureAtlasApp.h
)
target_link_libraries(textureatlas engine)
target_link_libraries(textureatlas Threads::Threads stdc++fs)

add_library(breakoutlib
        samples/breakout/BreakOutApp.cpp
        samples/breakout/BreakOutApp.h
        samples/breakout/AppState.cpp
        samples/breakout/AppState.h
        samples/breakout/SplashScreen.cpp
        samples/breakout/SplashScreen.h
        samples/breakout/MainMenu.cpp
        samples/breakout/MainMenu.h
        samples/breakout/Gameplay.cpp
        samples/breakout/Gameplay.h
        samples/breakout/GameOver.cpp
        samples/breakout/GameOver.h
        samples/breakout/Movable.cpp
        samples/breakout/Movable.h
        samples/breakout/Paddle.cpp
        samples/breakout/Paddle.h
        samples/breakout/Ball.cpp
        samples/breakout/Ball.h)

add_executable(
        breakout
        samples/breakout/main.cpp
)
target_link_libraries(breakout breakoutlib engine ${FREETYPE_LIBRARIES})

add_executable(
        breakout_test
        samples/breakout/Paddle_test.cpp
        samples/breakout/Ball_test.cpp)
target_link_libraries(breakout_test breakoutlib engine ${FREETYPE_LIBRARIES} gtest_main)

add_executable(
        Renderer_test
        engine/Area_test.cpp
        engine/AreaAllocator_test.cpp
        engine/AreaList_test.cpp
        engine/Renderer_test.cpp
        engine/TextureAtlas_test.cpp
        engine/FontManager_test.cpp
)
target_link_libraries(Renderer_test gtest_main engine ${FREETYPE_LIBRARIES})
