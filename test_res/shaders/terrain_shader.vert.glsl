#version 450 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Normal;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in vec2 texCoord;

uniform mat4 u_MVP;

uniform sampler2D u_HeightMap;

out vec3 v_Color;

void main()
{
    vec4 height = texture(u_HeightMap, texCoord);
    gl_Position = u_MVP * (a_Position + height);
    v_Color = height.rgb;
}