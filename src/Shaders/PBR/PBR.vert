#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 invModel;
    vec4 viewPosition;
} uniformObject;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_uv;
layout (location = 2) out vec3 out_pos;

void main()
{
    out_uv = in_uv;
    out_normal = normalize(uniformObject.model * vec4(in_normal, 0.0)).xyz;
    out_pos = (uniformObject.model * vec4(in_position, 1.0)).xyz;
    gl_Position = uniformObject.projection * uniformObject.view * uniformObject.model * vec4(in_position, 1.0);
}