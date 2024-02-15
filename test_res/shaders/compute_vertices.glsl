#version 450
#define M_PI 3.14159265358979323846

layout (local_size_x = 8, local_size_y = 8) in;

struct Vertex {
    vec4 pos;
    vec4 normal;
    vec4 color;
};

layout (std430, binding = 0) buffer Vertices {
    Vertex vertices[];
};

layout (std430, binding = 1) buffer Indices {
    uint indices[][3];
};

layout (std140, binding = 2) uniform u_TerrainSettings {
    uint octaves;
    float angle;
    vec2 pad0;
};

layout (std140, binding = 3) uniform u_ChunkSettings {
    vec2 centerPos;
    vec2 scale;
    uint resolution;
    vec3 pad1;
};


mat2 rot(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, s, -s, c);
}

float discreteHeightAt(ivec2 pos) {
    vec2 sp = 50 * fract(pos/M_PI);
    return 2 * fract(sp.x * sp.y * (sp.x + sp.y)) - 1;
}

vec4 coefsOfN(ivec2 tileID) {
    float a = discreteHeightAt(tileID - ivec2(0, 0));
    float b = discreteHeightAt(tileID - ivec2(1, 0));
    float c = discreteHeightAt(tileID - ivec2(0, 1));
    float d = discreteHeightAt(tileID - ivec2(1, 1));

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

float F(vec2 pos, uint octaves, float rotAngle) {
    const vec4 coefs = coefsOfN(ivec2(floor(pos)));
    const mat2 Rot = rot(rotAngle);

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

vec2 dF(vec2 pos, uint octaves, float rotAngle) {
    const vec4 coefs = coefsOfN(ivec2(floor(pos)));
    const mat2 Rot = rot(rotAngle);

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

uint lineariseCoord(ivec2 ids) {
    return ids.y * resolution + ids.x;
}

void main() {
    // Check validity of shader instance
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    if (vertexId.x >= resolution || vertexId.y >= resolution)
        return;

    // =========== Vertex Positon and Normal ================
    vec2 dUV = scale / resolution;
    vec2 localPlanePos = vertexId * dUV - 0.5 * scale;
    vec2 worldPlanePos = localPlanePos + center;

    float height = F(worldPlanePos, octaves, angle);
    vec2 grad = dF(worldPlanePos, octaves, angle);
    vec3 normal = normalise(vec3(-grad.x, 1, -grad.y));

    uint vertIdx = lineariseCoord(vertexId);
    vertices[vertexId].pos = vec4(localPlanePos.x, height, localPlanePos.y, 1.0);
    vertices[vertexId].normal = vec4(normal, 0.0);
    vertices[vertexId].color = vec4(0.7, 0.7, 0.7, 1.0);

    // =============== Indices ==================
    if (vertexId.x < resolution - 1 && vertexId.y < resolution - 1) {
        uint indexIdx = 6 * vertexIdx;
        indices[indexIdx + 0] = vertIdx;
        indices[indexIdx + 1] = lineariseCoord(vertId + ivec2(0, 1));
        indices[indexIdx + 2] = lineariseCoord(vertId + ivec2(1, 1));

        indices[indexIdx + 3] = vertIdx;
        indices[indexIdx + 4] = lineariseCoord(vertId + ivec2(1, 1));
        indices[indexIdx + 5] = lineariseCoord(vertId + ivec2(1, 0));
    }
}