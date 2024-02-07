#version 450

layout(local_size_x = 8,  local_size_y = 8) in;

layout(r32f,    binding = 0) readonly  uniform image2D u_Texture0; // Height map
layout(rgba32f, binding = 1) writeonly uniform image2D u_Texture1; // Normal map

void main()
{

    // Get pos
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileSize = imageSize(u_Texture0);
    if (vertexId.x >= tileSize.x || vertexId.y >= tileSize.y)
        return;

    float lH = imageLoad(u_Texture0, vertexId - ivec2(1, 0)).x;
    float rH = imageLoad(u_Texture0, vertexId + ivec2(1, 0)).x;
    float uH = imageLoad(u_Texture0, vertexId - ivec2(0, 1)).x;
    float dH = imageLoad(u_Texture0, vertexId + ivec2(0, 1)).x;

    vec3 normal = normalize(vec3(2*(rH - lH), 2*(dH - uH), -4));
    imageStore(u_Texture1, vertexId, vec4(normal, 1.0));
    //imageStore(u_Texture1, vertexId, vec4(1.0, 1.0, 1.0, 1.0));

}