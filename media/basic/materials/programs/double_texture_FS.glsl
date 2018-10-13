#version 330

uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;

in vec2 o_textCoord;

out vec4 fragColor;

void main()
{
    fragColor = texture2D(u_texture_1, o_textCoord) * texture2D(u_texture_2, o_textCoord);
}
