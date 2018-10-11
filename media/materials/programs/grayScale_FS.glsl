#version 330

uniform sampler2D RT;
in vec2 o_textCoord;

out vec4 color;

void main()
{
    vec3 greyscale = vec3(dot(texture2D(RT, o_textCoord).rgb, vec3(0.3, 0.59, 0.11)));
	color = vec4(greyscale, 1.0);
}
