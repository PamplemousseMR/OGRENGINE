#version 330

uniform sampler2D u_tRenderTarget;

in vec2 v_f2TextCor_Fs;

layout(location = 0) out vec4 v_f4OutCol0;
layout(location = 1) out vec4 v_f4OutCol1;
layout(location = 2) out vec4 v_f4OutCol2;

void main()
{
    vec3 greyscale = vec3(dot(texture2D(u_tRenderTarget, v_f2TextCor_Fs).rgb, vec3(0.3, 0.59, 0.11)));
    v_f4OutCol0 = vec4(greyscale.r, 0.0, 0.0, 1.0);
    v_f4OutCol1 = vec4(0.0,greyscale.g, 0.0, 1.0);
    v_f4OutCol2 = vec4(0.0, 0.0, greyscale.b, 1.0);
}
