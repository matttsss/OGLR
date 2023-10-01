#version 450 core

//#extension GL_EXT_debug_printf : require

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Normal;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in vec2 texCoord;

uniform mat4 u_MVP;

out vec3 v_Color;

void main()
{
    //debugPrintfEXT("Hi");
    gl_Position = u_MVP * a_Position;
    v_Color = a_Color;
}