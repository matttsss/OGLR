#pragma once

namespace OGLR
{

    class Layer {

    public:

        virtual void onAttach() = 0;
        virtual void onDettach() = 0;

        virtual void onUpdate(float deltaMs) = 0;
        virtual void onRender() = 0;

    };

}
