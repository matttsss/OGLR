#version 450

layout(local_size_x = 8,  local_size_y = 8) in;

layout(r32f,    binding = 0) readonly  uniform image2D u_Input;
layout(rgba32f, binding = 1) writeonly uniform image2D u_NHMap; // Normal map

void main()
{

    // Get pos
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileSize = imageSize(u_Input);
    if (vertexId.x >= tileSize.x || vertexId.y >= tileSize.y)
        return;

    float height = imageLoad(u_Input, vertexId).x;
    float lH = imageLoad(u_HeightMap, vertexId - ivec2(1, 0)).x;
    float rH = imageLoad(u_HeightMap, vertexId + ivec2(1, 0)).x;
    float uH = imageLoad(u_HeightMap, vertexId - ivec2(0, 1)).x;
    float dH = imageLoad(u_HeightMap, vertexId + ivec2(0, 1)).x;

    vec3 normal = normalize(vec3(-(rH - lH)*tileSize.x, 2,  -(dH - uH)*tileSize.y));
    imageStore(u_NHMap, vertexId, vec4(normal, height));

}