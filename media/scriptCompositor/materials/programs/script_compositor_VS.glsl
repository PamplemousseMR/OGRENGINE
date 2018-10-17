#version 330

layout(location = 0) in vec4 position;
layout(location = 8) in vec2 uv0;

uniform mat4 u_worldViewProj;

out vec2 v_f2TextCor_Fs;

void main()
{
    gl_Position = u_worldViewProj * position;
    v_f2TextCor_Fs = uv0;
}
