#version 450 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_speed;
layout(location = 2) in vec4 a_color;

layout (std140, binding = 2) uniform u_ParticleSettings {
    ivec2 viewport_size;
    float point_size;
};

uniform mat4 u_proj;
uniform mat4 u_view;

void main()
{
    gl_Position = u_proj * u_view * a_position;
    gl_PointSize = viewport_size.y * u_proj[1][1] * point_size / gl_Position.w;
    gl_FrontColor = a_color;
}