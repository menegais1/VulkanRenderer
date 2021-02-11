#version 450

layout (location = 0) in vec3 in_normal;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_pos;
layout (location = 3) in vec3 in_light;

layout(binding = 1) uniform sampler2D albedo;
layout(binding = 2) uniform sampler2D normal;
layout(binding = 3) uniform sampler2D metallicRoughness;
layout(binding = 4) uniform sampler2D emissive;

layout(location = 0) out vec4 outFragColor;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.05f;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(in_light - in_pos.xyz);
    float diff = max(dot(in_normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    outFragColor = vec4(diffuse + ambient, 1.0) * texture(albedo, in_uv);
}