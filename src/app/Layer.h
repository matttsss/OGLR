#pragma once

namespace OGLR
{

    class Layer {

    public:
        virtual void onAttach();
        virtual void onDettach();

        virtual void onUpdate(float deltaMs);
        virtual void onRender();

    };

}
