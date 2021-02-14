#version 450

layout (location = 0) in vec3 in_normal;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_pos;
layout (location = 3) in vec3 in_light;
layout (location = 4) in vec3 in_tangent;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 invModel;
    float fresnel;
    float geometry;
} uniformObject;
layout(binding = 1) uniform sampler2D albedo;
layout(binding = 2) uniform sampler2D normal;
layout(binding = 3) uniform sampler2D metallicRoughness;
layout(binding = 4) uniform sampler2D emissive;

layout(location = 0) out vec4 outFragColor;

vec3 getPerturbedNormal()
{
//    vec3 tangent = normalize(in_tangent);
//    vec3 n = normalize(in_normal);
    mat3 TBN = mat3(in_tangent, normalize(cross(in_normal, in_tangent)), in_normal);
    vec3 sampledNormal = ((texture(normal, in_uv) * 2.0) - 1).xyz;
    vec3 perturbedNormal = normalize(TBN * sampledNormal);
    return perturbedNormal;
}

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.05f * uniformObject.geometry;
    vec3 ambient = ambientStrength * lightColor;

    vec3 perturbedNormal = getPerturbedNormal();
    // diffuse
    vec3 model_light = (uniformObject.invModel * vec4(in_light,1)).xyz;
    vec3 lightDir = normalize(model_light - in_pos.xyz);
    float diff = max(dot(perturbedNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    outFragColor = vec4(perturbedNormal, 1.0) * texture(albedo, in_uv);
}