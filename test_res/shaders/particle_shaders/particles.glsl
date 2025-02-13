#version 450
#define M_PI 3.14159265358979323846

layout (local_size_x = 1) in;

struct Vertex {
    vec4 pos;
    vec4 speed;
};

uniform float u_dt;
uniform uint u_nb_particles;
uniform float u_pressure_multiplier;
uniform float u_pressure_target;
uniform float u_radius;


layout (std430, binding = 0) buffer Vertices_In {
    Vertex vertices_in[];
};

layout (std430, binding = 1) buffer Vertices_Out {
    Vertex vertices_out[];
};

layout (std430, binding = 2) buffer Densities {
    readonly float densities[];
};

float compute_pressure(uint idx) {
    return u_pressure_multiplier * (densities[idx] - u_pressure_target);
}

float compute_shared_pressure(uint idx1, uint idx2) {
    float pressure1 = compute_pressure(idx1);
    float pressure2 = compute_pressure(idx2);
    return 0.5f * (pressure1 + pressure2);
}

vec3 spicky_kernel_grad(vec3 ref_pos, vec3 pos) {
    vec3 v_diff = ref_pos - pos;
    float dist = length(v_diff);

    if (dist >= u_radius || dist < 1e-6)
        return vec3(0.0);

    const float volume = M_PI * pow(u_radius, 6) / 15.f;

    float x = u_radius - dist;
    vec3 grad = 3.f * v_diff * x * x / dist;

    return grad / volume;
}

vec3 pressure_force(uint idx) {
    const vec3 pos = vertices_in[idx].pos.xyz;

    vec3 res = vec3(0.0);
    for (uint i = 0; i < u_nb_particles; i++) {
        if (i == idx)
            continue;

        const float avg_pressure = compute_shared_pressure(idx, i);
        res += avg_pressure * spicky_kernel_grad(pos, vertices_in[i].pos.xyz) / densities[i];
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
    vec3 pressure_force = pressure_force(vertexId);
    vec3 gravity_force = vec3(0.f, 0.f, 0.f);
    vec3 acceleration = (pressure_force + gravity_force) / densities[vertexId];

    vertex.speed.xyz += acceleration * u_dt;
    vertex.pos.xyz += vertex.speed.xyz * u_dt;

    if (vertex.pos.x < -1.0 || vertex.pos.x > 1.0) {
        vertex.speed.x *= -1.0;
        vertex.pos.x = clamp(vertex.pos.x, -1.0, 1.0);
    }
    if (vertex.pos.y < -1.0 || vertex.pos.y > 1.0) {
        vertex.speed.y *= -1.0;
        vertex.pos.y = clamp(vertex.pos.y, -1.0, 1.0);
        //vertex.pos.y = max(vertex.pos.y, -1.0);
    }
    if (vertex.pos.z < -1.0 || vertex.pos.z > 1.0) {
        vertex.speed.z *= -1.0;
        vertex.pos.z = clamp(vertex.pos.z, -1.0, 1.0);
    }

    vertices_out[vertexId] = vertex;
}