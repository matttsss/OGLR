#version 450
#include "test_res/shaders/particle_shaders/kernels.glsl"

layout (local_size_x = 1) in;

struct Vertex {
    vec4 pos;
    vec4 speed;
};

uniform float u_radius;
uniform uint u_nb_particles;

layout (std430, binding = 0) buffer Vertices {
    Vertex vertices[];
};

layout (std430, binding = 1) buffer Densities {
    writeonly float densities[];
};

float density(vec3 pos) {

    float density = 0.0;
    for (uint i = 0; i < u_nb_particles; i++)
        density += spicky_kernel_3(distance(pos, vertices[i].pos.xyz), u_radius);

    return density;
}

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= u_nb_particles)
        return;

    densities[idx] = density(vertices[idx].pos.xyz);
}