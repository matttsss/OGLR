#include "OGLR.h"

#include "TerrainLayer.h"
#include "ParticleLayer.h"

int main()
{
    OGLR::Application app("OGLR test", 1080, 720);

    app.pushLayer(new ParticleLayer());
    //app.pushLayer(new TerrainLayer());
    app.run();

}