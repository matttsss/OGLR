#define M_PI 3.14159265358979323846

float spicky_kernel_3(float dist, float h) {
    if (dist >= h)
    return 0.0;

    const float volume = M_PI * pow(h, 6) / 15.f;

    float x = h - dist;
    return x * x * x / volume;
}

vec3 spicky_kernel_grad_3(vec3 ref_pos, vec3 pos, float h) {
    vec3 v_diff = ref_pos - pos;
    float dist = length(v_diff);

    if (dist >= h || dist < 1e-6)
        return vec3(0.0);

    const float volume = M_PI * pow(h, 6) / 15.f;

    float x = h - dist;
    vec3 grad = 3.f * v_diff * x * x / dist;

    return grad / volume;
}
