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


layout (std430, binding = 0) buffer Vertices {
    Vertex vertices[];
};

float kernel(float dist) {
    if (dist >= u_radius)
        return 0.0;

    float x = u_radius - dist;
    const float volume = M_PI * pow(u_radius, 6) / 15.f;
    return x * x * x / volume;
}

float densityAt(vec3 pos) {
    float density = 0.0;
    for (uint i = 0; i < u_nb_particles; i++) {
        float dist = distance(vertices[i].pos.xyz, pos);
        density += kernel(dist);
    }
    return density;
}


void main() {
    // Check validity of shader instance
    uint vertexId = gl_GlobalInvocationID.x;
    if (vertexId > u_nb_particles)
        return;

    // =========== Vertex Positon and Speed ================

    Vertex vertex = vertices[vertexId];
    vertex.speed = vec4(0.f, 0.f, 0.f, 0.f);
    vertex.color = vec4(densityAt(vertex.pos.xyz), 0.0, 0.0, 1.0);

    vertices[vertexId] = vertex;
}