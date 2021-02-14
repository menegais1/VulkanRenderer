#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_tangent;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 invModel;
    float fresnel;
    float geometry;
} uniformObject;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_uv;
layout (location = 2) out vec3 out_pos;
layout(location = 3) out vec3 out_light;
layout(location = 4) out vec3 out_tangent;
void main()
{
    out_uv = in_uv;
    out_normal = in_normal;
    gl_Position = uniformObject.projection * uniformObject.view * uniformObject.model * vec4(in_position, 1.0);
    out_pos = in_position;
    out_tangent = in_tangent;

    vec3 lightPos = vec3(0, 0, 2);
    out_light = lightPos;
}