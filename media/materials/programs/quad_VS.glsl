#version 330

layout(location = 0) in vec4 position;
uniform mat4 u_worldViewProj;
in vec2 uv0;
out vec2 o_textCoord;

void main()                    
{
	gl_Position = u_worldViewProj * position;
	o_textCoord = uv0;
}

