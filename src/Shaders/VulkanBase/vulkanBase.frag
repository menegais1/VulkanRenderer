#version 450

layout(location = 0) in vec4 in_color;
layout(location = 1) in vec2 in_uv;
layout(set = 0,binding=0) uniform sampler2D uniform_texSampler;

layout(location = 0) out vec4 out_fragColor;


void main(){
    ///out_fragColor = vec4(1,1,1, 1);
    out_fragColor = texture(uniform_texSampler, in_uv);
}