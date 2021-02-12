#version 450
layout (location = 0) in vec3 in_normal;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_pos;
layout (location = 3) in vec3 in_tangent;

layout(binding = 0) uniform UniformBufferObject
{
    bool useNormalMapping;
    vec3 lightPosition;
    vec3 viewPosition;
    vec4 lightColor;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 invModel;
} uniformObject;

layout(binding = 1) uniform sampler2D albedoSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D metallicRoughnessSampler;
layout(binding = 4) uniform sampler2D emissiveSampler;

layout(location = 0) out vec4 outFragColor;

float PI = 3.14159265359f;

float ggxDistribution(vec3 normal, vec3 halfway, float roughness);
float ggxSchlickGeometry(float normalViewDot, float roughness);
float smithGeometry(vec3 normal, vec3 view, vec3 light, float roughness);
vec3 schlickFresnel(float cosTheta, vec3 F0);
vec3 getPerturbedNormal();

void main()
{
    /* Normal calculation */
    vec3 normal = uniformObject.useNormalMapping ? getPerturbedNormal() : in_normal;

    /* Uniform Extraction*/
    vec3 viewPosition = uniformObject.viewPosition;
    vec3 lightPos = uniformObject.lightPosition;
    vec3 lightColor = uniformObject.lightColor.xyz * uniformObject.lightColor.w;

    /* According to GLTF 2.0 spec: R is Metallic and G is Roughness */
    /* However, I inspected the channels and confirmed my suspicions with blender */
    /* The actual encoding is: R = AO, G = Roughness & B = Metallic*/
    vec3 albedo = pow(texture(albedoSampler, in_uv).rgb, vec3(2.2f));
    float ao = texture(metallicRoughnessSampler, in_uv).r;
    float roughness = texture(metallicRoughnessSampler, in_uv).g;
    float metallic = texture(metallicRoughnessSampler, in_uv).b;
    vec3 emissive = texture(emissiveSampler, in_uv).rgb;

    /* Refractive Index */
    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);

    /* One point light */
    vec3 lightDir = normalize(lightPos - in_pos);
    vec3 viewDir = normalize(viewPosition - in_pos);
    vec3 halfway = normalize(lightDir + viewDir);
    float distance = length(lightPos - in_pos);
    float attenuantion = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuantion;

    /* Cook-Torrance BRDF. I tried moving to aux methods but many calculations are later used */
    float d = ggxDistribution(normal, halfway, roughness);
    vec3 f = schlickFresnel(max(dot(halfway, viewDir), 0.0f), F0);
    float g = smithGeometry(normal, viewDir, lightDir, roughness);

    /* Specular Component */
    vec3 kS = f;

    /* Diffuse Component*/
    vec3 kD = vec3(1.0f) - kS;
    kD *= 1.0f - metallic;

    vec3 numerator = d * f * g;
    float denominator = 4.0f * max(dot(viewDir, normal), 0.0) * max(dot(lightDir, normal), 0.0);
    vec3 specular = numerator / max(denominator, 0.001f); // Just avoiding zero division

    float normalLightDot = max(dot(normal, lightDir), 0.0f);

    vec3 reflectance = (kD * albedo / PI + specular) * radiance * normalLightDot;

    vec3 ambient = vec3(0.01f) * albedo * ao;
    vec3 color = ambient + reflectance;

    /* Gamma correction */
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f));

    /* Emissive is added after gamma correction*/
    color += emissive;
    outFragColor = vec4(color, 1.0);
}

float ggxDistribution(vec3 normal, vec3 halfway, float roughness)
{
    float a = roughness * roughness;
    float normalDotHalf = max(dot(normal, halfway), 0.0f);
    float normalDotHalfSq = normalDotHalf * normalDotHalf;
    float d = normalDotHalfSq * (a - 1.0f) + 1.0f;
    return a / (PI * d * d);
}

float ggxSchlickGeometry(float normalViewDot, float roughness)
{
    float r = roughness + 1.0f;
    float k = r * r / 8.0f;
    float denominator = normalViewDot * (1.0f - k) + k;
    return normalViewDot / denominator;
}

float smithGeometry(vec3 normal, vec3 view, vec3 light, float roughness)
{
    float normalViewDot = max(dot(normal, view), 0.0f);
    float normalLightDot = max(dot(normal, light), 0.0f);
    float ggx2 = ggxSchlickGeometry(normalViewDot, roughness);
    float ggx1 = ggxSchlickGeometry(normalLightDot, roughness);
    return ggx1 * ggx2;
}

vec3 schlickFresnel(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(max(1.0f - cosTheta, 0.0f), 5.0f);
}

vec3 getPerturbedNormal()
{
    mat3 TBN = mat3(in_tangent, normalize(cross(in_normal, in_tangent)), in_normal);
    vec3 sampledNormal = ((texture(normalSampler, in_uv) * 2.0) - 1).xyz;
    vec3 perturbedNormal = normalize(TBN * sampledNormal);
    return perturbedNormal;
}