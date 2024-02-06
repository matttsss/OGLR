#version 450

layout(local_size_x = 1,  local_size_y = 1) in;

uniform mat4 u_Transform;

layout(rgba32f, binding = 0) writeonly uniform image2D u_Texture0; // Gradient / Height map

void main()
{

    // Get pos
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileSize = imageSize(u_Texture0);
    if (vertexId.x >= tileSize.x || vertexId.y >= tileSize.y)
        return;

    vec2 vertexPosInPlane = vec2(vertexId) / vec2(tileSize);
    vec3 vertexPosInSpace = vec3(vertexPosInPlane.x, 0.0f, vertexPosInPlane.y) + u_Transform[3].xyz;

    float x = vertexPosInSpace.x - 0.5f;
    float y = vertexPosInSpace.z - 0.5f;

    vec3 normal = normalize(vec3(-y, 1, -x));
    vec4 grad_height = vec4(abs(normal), x*y);

    imageStore(u_Texture0, vertexId, grad_height);

}