#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec3 a_Color;

uniform mat4 u_MVP;
layout (binding = 0) uniform sampler2D u_NHMap; // normal / height map

out vec3 v_Color;
out vec2 v_TexCoor;

void main()
{
    float height = texture(u_NHMap, a_Position).w;

    vec4 localPos = vec4(a_Position.x, height, a_Position.y, 1.0);
    //vec4 localPos = vec4(a_Position.x, 0.0, a_Position.y, 1.0);

    gl_Position = u_MVP * localPos;
    v_Color = a_Color;
    v_TexCoor = a_Position;
}