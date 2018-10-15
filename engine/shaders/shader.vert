#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    vec2 extent;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    float halfWidth = ubo.extent.x / 2.0f;
    float halfHeight = ubo.extent.y / 2.0f;
    gl_Position = vec4(inPosition.x / halfWidth - 1.0f, inPosition.y / halfHeight - 1.0f, 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}


