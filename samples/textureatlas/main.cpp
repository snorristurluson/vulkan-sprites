//
// Created by snorri on 8.10.2018.
//

#include <iostream>
#include "TextureAtlasApp.h"

#include "../../engine/Renderer.h"
#include "../../engine/Logger.h"

static auto logger = GetLogger("main");

int main()
{
    logger->debug("Starting TextureAtlasApp");

    TextureAtlasApp app;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}