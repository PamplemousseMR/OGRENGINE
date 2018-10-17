#version 330

uniform sampler2D u_tRenderTarget;

in vec2 v_f2TextCor_Fs;

layout(location = 0) out vec4 v_f4OutCol;

void main()
{
    vec3 greyscale = vec3(dot(texture2D(u_tRenderTarget, v_f2TextCor_Fs).rgb, vec3(0.3, 0.59, 0.11)));
    v_f4OutCol = vec4(greyscale, 1.0);
}
