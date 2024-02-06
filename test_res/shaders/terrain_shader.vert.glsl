#version 450 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Normal;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in vec2 texCoord;

uniform mat4 u_MVP;

uniform sampler2D u_Texture0; // grad / height map

out vec3 v_Color;

void main()
{
    vec4 grad_height = texture(u_Texture0, texCoord);

    gl_Position = u_MVP * (a_Position + vec4(0.0f, grad_height.w, 0.0f, 0.0f));
    v_Color = grad_height.xyz;
    //v_Color = vec3(0.0, 1.0, 0.0);
}