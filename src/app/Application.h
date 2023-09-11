#pragma once

#include <vector>

#include "Layer.h"


namespace OGLR
{

    class Application
    {
    public:

        void run();


    private:
        std::vector<Layer> layers;

    };

} // OGLR
