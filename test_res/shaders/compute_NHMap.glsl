#version 450
#define M_PI 3.14159265358979323846

layout(local_size_x = 8,  local_size_y = 8) in;

uniform ivec2 u_Offset;

layout(rgba32f, binding = 0) writeonly uniform image2D u_NHMap;

layout (std140) uniform u_TerrainSettings {
    uint octaves;
    float angle;
};

layout (std140) uniform u_ChunkSettings {
    uint resolution;
    vec2 centerPos;
    vec2 scale;
};

mat2 rot(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, s, -s, c);
}

float discreteHeightAt(vec2 pos) {
    vec2 sp = 50 * fract(pos/M_PI);
    return 2 * fract(sp.x * sp.y * (sp.x + sp.y)) - 1;
}

// Returns heights of the cardinal corners in trigonometric order
vec4 coefsOfN() {
    float a = discreteHeightAt(u_Offset - ivec2(0, 0));
    float b = discreteHeightAt(u_Offset - ivec2(1, 0));
    float c = discreteHeightAt(u_Offset - ivec2(0, 1));
    float d = discreteHeightAt(u_Offset - ivec2(1, 1));

    return vec4(a, b - a, c - a, a - b - c + d);
}

float N(vec2 pos, vec4 coefs) {
    const vec2 sPos = smoothstep(0.0, 1.0, pos);
    return  coefs.x + coefs.y * sPos.x +
                      coefs.z * sPos.y +
                      coefs.w * sPos.x * sPos.y;
}

vec2 dN(vec2 pos, vec4 coefs) {
    const vec2 dS = 6 * pos * (1 - pos);
    return dS * (coefs.yz + coefs.w * smoothstep(0, 1, pos).yx);
}

float F(vec2 pos) {
    const vec4 coefs = coefsOfN();
    const mat2 Rot = rot(angle);

    float h = 0;
    float powI = 1;
    mat2 R = mat2(1, 0, 0, 1);

    for (int i = 0; i < octaves; ++i) {
        h += N(powI * R * pos, coefs) / powI;
        R = R * Rot;
        powI *= 2;
    }
    return h;
}

vec2 dF(vec2 pos) {
    const vec4 coefs = coefsOfN();
    const mat2 Rot = rot(angle);

    float powI = 1;
    vec2 grad = vec2(0);
    mat2 R = mat2(1, 0, 0, 1);

    for (int i = 0; i < octaves; ++i) {
        grad += R * dN(powI * R * pos, coefs);
        R = R * Rot;
        powI *= 2;
    }

    return grad;
}

void main()
{

    // Check validity of shader instance
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileSize = imageSize(u_NHMap);
    if (vertexId.x >= tileSize.x || vertexId.y >= tileSize.y)
        return;

    vec2 vertexPosInPlane = vec2(vertexId) / vec2(tileSize);
    float height = F(vertexPosInPlane);
    vec2 grad = dF(vertexPosInPlane);

    vec3 normal = normalize(vec3(-grad.x, 1, -grad.y));
    imageStore(u_NHMap, vertexId, vec4(normal, height));

}