#version 450
#define M_PI 3.14159265358979323846

layout (local_size_x = 8, local_size_y = 8) in;

struct Vertex {
    vec4 pos;
    vec4 normal;
    vec4 color;
};

uniform ivec2 u_ChunkOffset;

layout (std430, binding = 0) buffer Vertices {
    Vertex vertices[];
};

layout (std430, binding = 1) buffer Indices {
    uint indices[];
};

layout (std140, binding = 2) uniform u_TerrainSettings {
    uint octaves;
    float angle;
    float frequ;
    float zScale;
};

layout (std140, binding = 3) uniform u_ChunkSettings {
    vec2 worldOffset;
    ivec2 scale;
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
    float a = discreteHeightAt(tileID + ivec2(0, 0));
    float b = discreteHeightAt(tileID + ivec2(1, 0));
    float c = discreteHeightAt(tileID + ivec2(0, 1));
    float d = discreteHeightAt(tileID + ivec2(1, 1));

    return vec4(a, b - a, c - a, a - b - c + d);
}


vec3 N_and_grad(vec2 pos) {
    const vec2 w = fract(pos);
    const vec4 coefs = coefsOfN(ivec2(floor(pos)));

    const vec2 dS = 6 * w * (1 - w);
    const vec2 S = w * w * (3.0 - 2.0 * w);

    return vec3(dS * (coefs.yz + coefs.w * S.yx),
                    coefs.x + coefs.y * S.x +
                    coefs.z * S.y +
                    coefs.w * S.x * S.y);
}

vec3 F_and_grad(vec2 pos, uint octaves, float rotAngle) {
    const mat2 Rot = rot(rotAngle);

    float h = 0;
    vec2 grad = vec2(0);

    float powI = 1;
    mat2 R = mat2(1, 0, 0, 1);

    for (int i = 0; i < octaves; ++i) {
        vec2 new_noise_pos = powI * R * pos;
        float attenuation_factor = 1 / (1 + length(grad));

        vec3 grad_heigth_N = N_and_grad(new_noise_pos);

        h += attenuation_factor * grad_heigth_N.z / powI;
        grad += attenuation_factor * R * grad_heigth_N.xy;

        R = R * Rot;
        powI *= 2;
    }

    return vec3(grad, h);
}



void main() {
    // Check validity of shader instance
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    if (vertexId.x >= resolution + 1 || vertexId.y >= resolution + 1)
        return;

    // =========== Vertex Positon and Normal ================
    vec2 dUV = vec2(scale) / resolution;
    vec2 localPlanePos = vertexId * dUV - 0.5 * scale;
    vec2 worldPlanePos = localPlanePos + worldOffset + u_ChunkOffset;

    vec3 grad_height = zScale * F_and_grad(worldPlanePos / frequ, octaves, angle);
    vec2 grad = grad_height.xy / frequ;
    vec3 normal = normalize(vec3(-grad.x, 1, -grad.y));

    uint vertIdx = vertexId.y * (resolution + 1) + vertexId.x;
    vertices[vertIdx].pos = vec4(localPlanePos.x, grad_height.z, localPlanePos.y, 1.0);
    vertices[vertIdx].normal = vec4(normal, 0.0);
    vertices[vertIdx].color = vec4(0.7, 0.7, 0.7, 1.0);

    // =============== Indices ==================
    if (vertexId.x < resolution && vertexId.y < resolution) {
        uint indexIdx = 6 * (vertexId.y * resolution + vertexId.x);
        indices[indexIdx + 0] = vertIdx;
        indices[indexIdx + 1] = vertIdx + (resolution + 1) + 1;
        indices[indexIdx + 2] = vertIdx + (resolution + 1);

        indices[indexIdx + 3] = vertIdx;
        indices[indexIdx + 4] = vertIdx + 1;
        indices[indexIdx + 5] = vertIdx + (resolution + 1) + 1;
    }
}