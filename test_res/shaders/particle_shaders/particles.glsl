#version 450
#include "test_res/shaders/particle_shaders/kernels.glsl"

layout (local_size_x = 1) in;

struct Vertex {
    vec4 pos;
    vec4 speed;
};

uniform float u_dt;

layout (std140, binding = 3) uniform u_ParticleSettings {
    uint u_nb_particles;
    float u_viscosity_mul;
    float u_pressure_mul;
    float u_pressure_target;
    float u_radius;
};

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
    return u_pressure_mul * (densities[idx] - u_pressure_target);
}

float compute_shared_pressure(uint idx1, uint idx2) {
    float pressure1 = compute_pressure(idx1);
    float pressure2 = compute_pressure(idx2);
    return 0.5f * (pressure1 + pressure2);
}

vec3 pressure_force(uint idx) {
    const vec3 pos = vertices_in[idx].pos.xyz;

    vec3 res = vec3(0.f);
    for (uint i = 0; i < u_nb_particles; i++) {
        const vec3 v_diff = vertices_in[i].pos.xyz - pos;
        const float dist = length(v_diff);

        if (dist < 1e-7f)
            continue;

        const float avg_pressure = compute_shared_pressure(idx, i);
        const float scale = avg_pressure * spicky_kernel_grad_3(dist, u_radius) / densities[i];
        res += scale * normalize(v_diff);
    }

    return res;
}

vec3 viscosity_force(uint idx) {
    const vec3 pos = vertices_in[idx].pos.xyz;
    const vec3 speed = vertices_in[idx].speed.xyz;


    vec3 res = vec3(0.f);
    for (uint i = 0; i < u_nb_particles; i++) {
        const float dist = distance(pos, vertices_in[i].pos.xyz);
        if (dist < 1e-7f) continue;

        const vec3 speed_diff = vertices_in[i].speed.xyz - speed;
        res += speed_diff * viscocity_kernel_laplacian(dist, u_radius) / densities[i];
    }

    return u_viscosity_mul * res;
}


void main() {
    // Check validity of shader instance
    uint vertexId = gl_GlobalInvocationID.x;
    if (vertexId > u_nb_particles)
        return;

    // =========== Vertex Positon and Speed ================
    Vertex vertex = vertices_in[vertexId];
    vec3 pressure_force = pressure_force(vertexId);
    vec3 viscosity_force = viscosity_force(vertexId);
    vec3 gravity_force = vec3(0.f, 0.f, 0.f);
    vec3 acceleration = (pressure_force + gravity_force + viscosity_force) / densities[vertexId];

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