#version 330

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 o_textCoord;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(texture1, o_textCoord) * texture2D(texture2, o_textCoord);
}