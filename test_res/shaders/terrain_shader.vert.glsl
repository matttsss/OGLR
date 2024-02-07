#version 450 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Normal;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in vec2 texCoord;

uniform mat4 u_MVP;
layout (binding = 0) uniform sampler2D u_Texture0; // height map
layout (binding = 1) uniform sampler2D u_Texture1; // normal map

out vec3 v_Color;

void main()
{
    float height = texture(u_Texture0, texCoord).x;
    vec3  normal = texture(u_Texture1, texCoord).xyz;

    gl_Position = u_MVP * (a_Position + vec4(0.0f, height, 0.0f, 0.0f));
    v_Color = 1.3 * normal;
    //v_Color = vec3(1.0, 1.0, 0.0);

}