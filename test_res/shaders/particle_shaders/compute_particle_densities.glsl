#version 450

#define M_PI 3.14159265358979323846

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
    float densities[];
};

float kernel(float dist) {
    if (dist >= u_radius)
        return 0.0;

    const float volume = M_PI * pow(u_radius, 6) / 15.f;

    float x = u_radius - dist;
    return x * x * x / volume;
}

float density(vec3 pos) {

    float density = 0.0;
    for (uint i = 0; i < u_nb_particles; i++)
        density += kernel(distance(pos, vertices[i].pos.xyz));

    return density;
}

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= u_nb_particles)
        return;

    densities[idx] = density(vertices[idx].pos.xyz);
}