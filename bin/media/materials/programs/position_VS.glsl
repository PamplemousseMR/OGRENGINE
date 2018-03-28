#version 330
layout(location = 0) in vec4 position;
in vec2 uv0;

uniform mat4 u_worldViewProj;

out vec2 o_textCoord;

void main()
{
	gl_Position = u_worldViewProj * position;
	o_textCoord = uv0;
}