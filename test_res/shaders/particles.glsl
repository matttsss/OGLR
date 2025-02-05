#version 450
#define M_PI 3.14159265358979323846

layout (local_size_x = 1) in;

struct Vertex {
    vec4 pos;
    vec4 speed;
    vec4 color;
};

uniform float u_dt;
uniform uint u_nb_particles;
uniform float u_radius;


layout (std430, binding = 0) buffer Vertices_In {
    Vertex vertices_in[];
};

layout (std430, binding = 1) buffer Vertices_Out {
    Vertex vertices_out[];
};

vec4 kernel_and_grad(vec3 ref_pos, vec3 pos) {
    vec3 v_diff = ref_pos - pos;
    float dist = length(v_diff);

    if (dist >= u_radius)
        return vec4(0.0);

    const float volume = M_PI * pow(u_radius, 6) / 15.f;

    // Compute kernel value
    float x = u_radius - dist;
    float val = x * x * x;

    // Compute kernel gradient
    vec3 grad = 3.f * v_diff * x * x / dist;

    return 1 * vec4(grad, val) / volume;
}

vec4 density_and_grad(uint idx) {
    const vec3 pos = vertices_in[idx].pos.xyz;

    vec4 res = vec4(0.0);
    for (uint i = 0; i < u_nb_particles; i++) {
        if (i == idx)
            continue;

        res += kernel_and_grad(pos, vertices_in[i].pos.xyz);
    }

    return res;
}


void main() {
    // Check validity of shader instance
    uint vertexId = gl_GlobalInvocationID.x;
    if (vertexId > u_nb_particles)
        return;

    // =========== Vertex Positon and Speed ================

    Vertex vertex = vertices_in[vertexId];

    vertex.speed.y += -9.81f * u_dt;
    vertex.pos.xyz += vertex.speed.xyz * u_dt;

    if (vertex.pos.x < -1.0 || vertex.pos.x > 1.0) {
        vertex.speed.x *= -1.0;
        vertex.pos.x = clamp(vertex.pos.x, -1.0, 1.0);
    }
    if (vertex.pos.y < -1.0 || vertex.pos.y > 1.0) {
        vertex.speed.y *= -1.0;
        vertex.pos.y = clamp(vertex.pos.y, -1.0, 1.0);
    }
    if (vertex.pos.z < -1.0 || vertex.pos.z > 1.0) {
        vertex.speed.z *= -1.0;
        vertex.pos.z = clamp(vertex.pos.z, -1.0, 1.0);
    }

    vertices_out[vertexId] = vertex;
}