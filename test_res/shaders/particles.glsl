#version 450

layout (local_size_x = 1) in;

struct Vertex {
    vec4 pos;
    vec4 speed;
    vec4 color;
};

uniform float u_time;
uniform uint u_nb_particles;
uniform float u_radius;


layout (std430, binding = 0) buffer Vertices {
    Vertex vertices[];
};

float kernel(float dist) {
    dist = min(dist, u_radius);
    return 4 * pow(u_radius - dist, 3.f) / pow(u_radius, 4);
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
    vertex.speed = vec4(0.f, -vertex.pos.x * sin(u_time), 0.f, 0.0);
    vertex.color = vec4(densityAt(vertex.pos.xyz), 0.0, 0.0, 1.0);

    vertices[vertexId] = vertex;
}