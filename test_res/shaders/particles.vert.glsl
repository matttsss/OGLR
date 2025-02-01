#version 450 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

layout (std140, binding = 2) uniform u_ParticleSettings {
    vec4 cam_pos;
    float point_size;
    float near_plane;
};

uniform mat4 u_MVP;

void main()
{
    gl_PointSize = near_plane * point_size / length(a_position - cam_pos);
    gl_Position = u_MVP * a_position;
    gl_FrontColor = a_color;
}