#version 330

uniform sampler2D u_tRenderTarget0;
uniform sampler2D u_tRenderTarget1;
uniform sampler2D u_tRenderTarget2;
uniform sampler2D u_tRenderTarget3;

in vec2 v_f2TextCor_Fs;

layout(location = 0) out vec4 v_f4OutCol;

void main()
{
    if(v_f2TextCor_Fs.x < 0.5f && v_f2TextCor_Fs.y < 0.5f)
    {
        v_f4OutCol = texture2D(u_tRenderTarget0, v_f2TextCor_Fs*2);
    }
    else if(v_f2TextCor_Fs.x < 0.5f && v_f2TextCor_Fs.y >= 0.5f)
    {
        v_f4OutCol = texture2D(u_tRenderTarget1, v_f2TextCor_Fs*2);
    }
    else if(v_f2TextCor_Fs.x >= 0.5f && v_f2TextCor_Fs.y < 0.5f)
    {
        v_f4OutCol = texture2D(u_tRenderTarget2, v_f2TextCor_Fs*2);
    }
    else
    {
        v_f4OutCol = texture2D(u_tRenderTarget3, v_f2TextCor_Fs*2);
    }
}
