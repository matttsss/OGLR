#version 450 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec2 texCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_MVP * a_Position;
	v_TexCoord = texCoord;
}