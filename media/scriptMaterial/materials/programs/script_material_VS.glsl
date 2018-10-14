#version 440 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;
layout(location = 8) in vec2 uv0;

uniform mat4 u_m4Model;
uniform mat4 u_m4View;
uniform mat4 u_m4Projection;

uniform vec3 u_f3LightPos_Vs;

out vec2 v_f2TextCor_Fs;
out vec3 v_f3NormalDir_Vs;
out vec3 v_f3CamDir_Vs;
out vec3 v_f3LightDir_Vs;

void main()
{
    gl_Position = u_m4Projection * u_m4View * u_m4Model * vec4(position,1);

    vec3 vertexPos_Vs = ( u_m4View * u_m4Model * vec4(position,1)).xyz;
    v_f3CamDir_Vs = vec3(0,0,0) - vertexPos_Vs;

    v_f3LightDir_Vs = u_f3LightPos_Vs-vertexPos_Vs;

    v_f3NormalDir_Vs = ( u_m4View * u_m4Model * vec4(normal,0)).xyz;

    v_f2TextCor_Fs = uv0;
}
