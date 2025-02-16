#define M_PI 3.14159265358979323846

float spicky_kernel_3(float dist, float h) {
    if (dist >= h)
        return 0.f;

    const float volume = M_PI * pow(h, 6) / 15.f;

    float x = h - dist;
    return x * x * x / volume;
}

float spicky_kernel_grad_3(float dist, float h) {
    if (dist >= h)
        return 0.f;

    const float volume = M_PI * pow(h, 6) / 15.f;

    float x = h - dist;
    return -3.f * x * x / volume;
}

float viscocity_kernel(float dist, float h) {
    if (dist >= h)
        return 0.f;

    const float volume = 2.f * M_PI * pow(h, 3) / 15.f;

    return (-0.5f * dist * dist * dist / (h * h * h) +
             dist * dist / (h * h) +
             0.5f * h / dist
            -1.f) / volume;
}

float viscocity_kernel_laplacian(float dist, float h) {
    if (dist >= h)
        return 0.f;

    return 45.f * (h - dist) / (M_PI *  pow(h, 6));
}
