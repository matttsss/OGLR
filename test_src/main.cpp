#include <iostream>

#include "OGLR.h"
#include <stb_image.h>

int main()
{
    std::cout << "Hello world!" << std::endl;
    int w, h, bpp;
    stbi_load("res/texture/dices.png", &w, &h, &bpp, 4);
}