#version 450

layout (location = 0) in vec3 in_normal;
layout (location = 1) in vec2 in_uv;

layout(binding= 1 ) uniform sampler2D uniform_texSampler;

layout(location = 0) out vec4 out_fragColor;


void main(){
    out_fragColor = texture(uniform_texSampler, in_uv);
}