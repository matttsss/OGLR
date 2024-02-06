#version 450

layout(local_size_x = 1,  local_size_y = 1) in;

uniform mat4 u_Transform;

layout(r32f, binding = 0) uniform image2D u_Texture0; // Height map
layout(rgb32f, binding = 1) uniform image2D u_Texture1; // Gradient map

void main()
{

    // Get pos
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileSize = imageSize(u_Texture0);
    if (vertexId.x >= tileSize.x || vertexId.y >= tileSize.y)
        return;

    vec2 vertexPosInPlane = vec2(vertexId) / vec2(tileSize);
    vec3 vertexPosInSpace = vec3(vertexPosInPlane.x, 0.0f, vertexPosInPlane.y) + u_Transform[3].xyz;

    imageStore(u_Texture0, vertexId, 0.0f);
    imageStore(u_Texture0, vertexId, vec3(0.0f, 0.5f, 0.5f));

}