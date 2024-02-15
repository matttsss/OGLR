#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec3 a_Color;

uniform mat4 u_MVP;
uniform uint u_Radius;

layout (binding = 0) uniform sampler2D u_NHMap; // normal / height map

out vec3 v_Color;
out vec2 v_PlanePos;

vec2 getOffset() {
    const uint side = 2 * u_Radius + 1;
    float xOffset = gl_InstanceID % side;
    float yOffset = (gl_InstanceID - xOffset) / side;

    xOffset -= 0.5 * side;
    yOffset -= 0.5 * side;
    return vec2(xOffset, yOffset);
}

void main()
{
    v_Color = a_Color;

    float height = texture(u_NHMap, a_Position).w;

    vec2 newPos = a_Position + getOffset();
    v_PlanePos = newPos + 0.5;

    vec4 localPos = vec4(newPos.x, height, newPos.y, 1.0);
    //vec4 localPos = vec4(a_Position.x, 0.0, a_Position.y, 1.0);

    gl_Position = u_MVP * localPos;
}