#version 450 core

layout (location = 0) out vec4 o_Color;

void main()
{
    if (length(gl_PointCoord - vec2(0.5, 0.5)) > 0.5) {
        discard;
    }

    o_Color = vec4(1.0);
}