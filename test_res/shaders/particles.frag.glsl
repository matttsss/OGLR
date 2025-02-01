#version 450 core

void main()
{
    if (length(gl_PointCoord - vec2(0.5, 0.5)) > 0.5) {
        discard;
    }

    gl_FragColor = vec4(gl_PointCoord.x, gl_PointCoord.y, 0.0, 1.0);
}