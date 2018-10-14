find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

add_library(
        engine
        ../extern/stb_image.h
        Renderer.cpp Renderer.h
        Vertex.cpp Vertex.h
        Texture.cpp Texture.h
        ShaderLib.cpp ShaderLib.h
        DebugMessenger.cpp DebugMessenger.h
        Logger.cpp Logger.h
        AreaAllocator.cpp AreaAllocator.h
        AreaList.cpp AreaList.h
        Area.cpp Area.h
        TextureAtlas.cpp TextureAtlas.h
        ITexture.h
        AtlasTexture.cpp AtlasTexture.h
        FontManager.cpp FontManager.h
        Glyph.cpp Glyph.h
        Font.cpp Font.h
)

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

add_dependencies(engine Shaders)

add_custom_command(TARGET engine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:engine>/shaders/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_BINARY_DIR}/shaders"
        "$<TARGET_FILE_DIR:engine>/shaders"
        )
