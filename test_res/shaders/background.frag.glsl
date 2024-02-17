#version 450

layout (location = 0) out vec4 o_Color;

const vec4 c_UpColor   = vec4(0.4, 0.4, 0.9, 1.0);
const vec4 c_DownColor = vec4(0.1, 0.1,  0.1,  1.0);

layout (std140, binding = 0) uniform AimSettings {
    vec3 u_aim;
    int  u_FrameBuffWidth;
    vec3 u_uBase;
    int  u_FrameBuffHeight;
    vec3 u_vBase;
    int  pad2;
};


void main() {
    const ivec2 frameBuffSize = ivec2(u_FrameBuffWidth, u_FrameBuffHeight);
    const vec2 UVCentered = (gl_FragCoord.xy - 0.5 * frameBuffSize) / frameBuffSize;

    const vec3 rayDir = normalize(u_aim + UVCentered.x * u_uBase + UVCentered.y * u_vBase);

    float lerpFactor = 0.5 * (rayDir.y + 1);

    o_Color = mix(c_DownColor, c_UpColor, lerpFactor);
}