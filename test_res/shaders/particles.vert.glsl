#version 450 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_MVP;

void main()
{
    gl_PointSize = 10.0;
    gl_Position = u_MVP * a_position;
    gl_FrontColor = a_color;
}