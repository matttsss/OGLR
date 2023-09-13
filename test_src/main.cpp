#include "OGLR.h"

#include "TestLayer.h"

int main()
{
    OGLR::Application app("OGLR test", 1080, 720);

    app.pushLayer(new TestLayer());
    app.run();

}