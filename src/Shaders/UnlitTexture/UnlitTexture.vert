#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout(binding = 1) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} uniformObject;

layout(location = 0) out vec4 out_normal;
layout(location = 1) out vec2 out_uv;

void main()
{
    out_uv = in_uv;
    gl_Position =
        uniformObject.projection *
        uniformObject.view *
        uniformObject.model * vec4(in_position, 1.0);
}