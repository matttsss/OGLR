#version 450 core

layout (location = 0) out vec4 o_Color;

layout (binding = 0) uniform sampler2D u_NHMap; // normal / height map


in vec3 v_Color;
in vec2 v_TexCoor;

const vec3 sunDirection = normalize(vec3(-1.0, -1.0, 0.0));
const vec3 sunRadiance = vec3(0.7, 0.5, 0.1);
const vec3 ambientRadiance = vec3(0.1, 0.1, 0.1);


void main()
{
    vec3 normal = texture(u_NHMap, v_TexCoor).xyz;
    float cosTheta = clamp(dot(-sunDirection, normal), 0, 1);
    o_Color = vec4(v_Color * (sunRadiance * cosTheta + ambientRadiance), 1.0f);
    //o_Color = vec4(abs(normal), 1.0f);

}