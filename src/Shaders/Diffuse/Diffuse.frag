#version 450

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;

layout(binding = 0) uniform UniformBufferObject
{
    bool useNormalMapping;
    float normalStrength;
    float detailNormalTiling;
    float detailNormalStrength;
    vec3 lightPosition;
    vec3 viewPosition;
    vec4 lightColor;
    mat4 model;
    mat4 view;
    mat4 projection;
} uniformObject;
layout(binding = 1) uniform sampler2D albedo;
layout(binding = 2) uniform sampler2D normal;
layout(binding = 3) uniform sampler2D metallicRoughness;
layout(binding = 4) uniform sampler2D emissive;

layout(location = 0) out vec4 outFragColor;

vec3 getPerturbedNormal()
{
    mat3 TBN = mat3(in_tangent,normalize(cross(in_normal,in_tangent)), in_normal);
    vec3 sampledNormal = ((texture(normal, in_uv) * 2.0) - 1).xyz;
    vec3 perturbedNormal = normalize(TBN * sampledNormal);
    return perturbedNormal;
}

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.05f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 perturbedNormal = uniformObject.useNormalMapping ? getPerturbedNormal() : in_normal;
    // diffuse
    vec3 lightDir = normalize(uniformObject.lightPosition - in_pos.xyz);
    float diff = max(dot(perturbedNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    outFragColor = vec4(diffuse, 1.0) * texture(albedo, in_uv);
}