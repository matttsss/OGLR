#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Color;

uniform mat4 u_MVP;
layout (binding = 0) uniform sampler2D u_Texture0; // height map
layout (binding = 1) uniform sampler2D u_Texture1; // normal map

out vec3 v_Color;
out vec3 v_Normal;

void main()
{
    float height = texture(u_Texture0, a_TexCoord).x;
    vec3  normal = texture(u_Texture1, a_TexCoord).xyz;

    vec4 localPos = vec4(a_Position.x, height, a_Position.y, 1.0);

    gl_Position = u_MVP * localPos;
    v_Color = a_Color;
    v_Normal = normal;
}