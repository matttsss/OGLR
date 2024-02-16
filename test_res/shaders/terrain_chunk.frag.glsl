#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Normal;
in vec4 v_Color;

const vec3 sunDirection = normalize(vec3(-0.5, -1.0, 0.0));
const vec4 sunRadiance = vec4(0.7, 0.7, 0.7, 1.0);
const vec4 ambientRadiance = vec4(0.1, 0.1, 0.1, 1.0);


void main()
{
    float cosTheta = clamp(dot(-sunDirection, v_Normal.xyz), 0, 1);
    o_Color = v_Color * (sunRadiance * cosTheta + ambientRadiance);
    //o_Color = v_Color;
}