#version 450 core

layout (location = 0) out vec4 o_Color;

const vec4 low_color = vec4(1.f, 0.f, 0.f, 1.f);
const vec4 high_color = vec4(0.f, 1.f, 0.f, 1.f);

const float max_speed = 1.f;

in flat float speed;

void main()
{
    if (length(gl_PointCoord - vec2(0.5, 0.5)) > 0.5) {
        discard;
    }

    float lerp_f = min(speed / max_speed, 1.f);

    o_Color = mix(low_color, high_color, lerp_f);
}