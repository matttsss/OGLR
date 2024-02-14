#version 450
#define M_PI 3.14159265358979323846

layout(local_size_x = 8,  local_size_y = 8) in;

uniform ivec2 u_Offset;

layout(r32f, binding = 0) writeonly uniform image2D u_HeightMap;
const int maxIter = 1;

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
    vec2 sPos = smoothstep(0.0, 1.0, pos);
    return  coefs.x + coefs.y * sPos.x +
                      coefs.z * sPos.y +
                      coefs.w * sPos.x * sPos.y;
}

float heightAt(vec2 pos) {
    vec4 coefs = coefsOfN();
    mat2 R = rot(M_PI / 4.0);

    float h = 0;
    float powI = 1;
    vec2 rotatedP = pos;

    for (int i = 0; i < maxIter; ++i) {
        h += N(powI * rotatedP, coefs) / powI;
        powI *= 2;
        rotatedP = R * rotatedP;
    }

    return h;
}

void main()
{

    // Check validity of shader instance
    ivec2 vertexId = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileSize = imageSize(u_HeightMap);
    if (vertexId.x >= tileSize.x || vertexId.y >= tileSize.y)
        return;

    vec2 vertexPosInPlane = vec2(vertexId) / vec2(tileSize);
    imageStore(u_HeightMap, vertexId, vec4(heightAt(vertexPosInPlane), 0, 0, 0));

}