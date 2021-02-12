#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_tangent;

layout(binding = 0) uniform UniformBufferObject
{
    bool useNormalMapping;
    vec3 viewPosition;
    vec3 lightPosition;
    vec4 lightColor;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 invModel;
} uniformObject;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_uv;
layout (location = 2) out vec3 out_pos;
layout(location = 3) out vec3 out_tangent;

void main()
{
    out_uv = in_uv;
    out_tangent = in_tangent;
    out_normal = normalize(uniformObject.model * vec4(in_normal, 0.0)).xyz;
    out_pos = (uniformObject.model * vec4(in_position, 1.0)).xyz;
    gl_Position = uniformObject.projection * uniformObject.view * uniformObject.model * vec4(in_position, 1.0);
}