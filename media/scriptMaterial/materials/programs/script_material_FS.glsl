#version 440 core

in vec2 v_f2TextCor_Fs;
in vec3 v_f3NormalDir_Vs;
in vec3 v_f3CamDir_Vs;
in vec3 v_f3LightDir_Vs;

uniform sampler2D u_tAmbient;
uniform sampler2D u_tDiffuse;
uniform sampler2D u_tNormal;
uniform sampler2D u_tSpecular;

uniform vec3 u_f3AmbientCol;
uniform vec3 u_f3DiffuseCol;
uniform vec3 u_f3SpecularCol;
uniform float u_fSpecularExponent;

uniform vec3 u_f3LightAmbientCol;
uniform vec3 u_f3LightDiffuseCol;
uniform vec3 u_f3LightSpecularCol;

layout(location = 0) out vec4 v_f4OutCol;

void main()
{
    vec3 f3NormalDir_VsN = normalize(v_f3NormalDir_Vs);
    vec3 f3LightDir_VsN = normalize(v_f3LightDir_Vs);
    float cosTheta = clamp( dot(f3NormalDir_VsN, f3LightDir_VsN), 0, 1);
    vec3 f3CamDir_VsN = normalize(v_f3CamDir_Vs);
    vec3 f3CamReflectDir_VsN = reflect(-f3LightDir_VsN,f3NormalDir_VsN);
    float cosAlpha = clamp(dot(f3CamDir_VsN, f3CamReflectDir_VsN), 0, 1);

    // ambient
    vec3 f3AmbientCol = texture( u_tAmbient, vec2(v_f2TextCor_Fs.x, 1 - v_f2TextCor_Fs.y)).rgb;
    f3AmbientCol = f3AmbientCol * u_f3LightAmbientCol;

    // diffuse
    vec4 f4DiffuseCol = texture( u_tDiffuse, vec2(v_f2TextCor_Fs.x, 1 - v_f2TextCor_Fs.y));
    vec3 f3DiffuseCol = f4DiffuseCol.rgb;
    float fAlpha = f4DiffuseCol.a;
    f3DiffuseCol = f3DiffuseCol * u_f3LightDiffuseCol * cosTheta;

    // specular
    vec3 f3SpecularCol = texture( u_tSpecular, vec2(v_f2TextCor_Fs.x, 1 - v_f2TextCor_Fs.y)).rgb;
    f3SpecularCol = f3SpecularCol * u_f3LightSpecularCol * pow(cosAlpha, u_fSpecularExponent);

    v_f4OutCol = vec4(u_f3AmbientCol * f3AmbientCol + u_f3DiffuseCol * f3DiffuseCol + u_f3SpecularCol * f3SpecularCol ,fAlpha);
}
