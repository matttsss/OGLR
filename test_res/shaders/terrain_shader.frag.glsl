#version 450 core

layout (location = 0) out vec4 o_Color;

in vec3 v_Color;
in vec3 v_Normal;

const vec3 sunDirection = normalize(vec3(1.0, -1.0, 0.0));
const vec3 sunRadiance = vec3(5.6, 5.522, 1.5);


void main()
{

    float lightIntensity = max(0, dot(-sunDirection, normalize(v_Normal)));
    o_Color = vec4(lightIntensity * v_Color * sunRadiance, 1.0f);
    //o_Color = vec4(v_Normal, 1.0f);
}