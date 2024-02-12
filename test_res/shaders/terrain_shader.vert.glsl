#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Color;

uniform mat4 u_MVP;
layout (binding = 0) uniform sampler2D u_Texture0; // normal / height map

out vec3 v_Color;
out vec3 v_Normal;

void main()
{
    vec4 nhValue = texture(u_Texture0, a_TexCoord);

    vec4 localPos = vec4(a_Position.x, nhValue.w, a_Position.y, 1.0);
    //vec4 localPos = vec4(a_Position.x, 0.0, a_Position.y, 1.0);

    gl_Position = u_MVP * localPos;
    v_Color = a_Color;
    v_Normal = nhValue.xyz;
}