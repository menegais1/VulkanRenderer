#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} uniformObject;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_uv;
layout (location = 2) out vec3 out_pos;
layout(location = 3) out vec3 out_light;

void main()
{
    out_uv = in_uv;
    out_normal = in_normal;
    gl_Position = uniformObject.projection * uniformObject.view * uniformObject.model * vec4(in_position, 1.0);
    vec3 lightPos = vec3(1.0, -1.0, 1.0);
    out_pos = gl_Position.xyz;
    out_light = (uniformObject.projection * uniformObject.view * uniformObject.model * vec4(lightPos, 1.0)).xyz;
}