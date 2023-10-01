#version 450 core

layout (location = 0) out vec4 o_Color;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

void main()
{
	o_Color = texture(u_Texture, v_TexCoord);
}