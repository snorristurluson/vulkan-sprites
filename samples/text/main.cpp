//
// Created by snorri on 10.10.2018.
//

#include <iostream>
#include "TextApp.h"

#include "../../engine/Renderer.h"
#include "../../engine/Logger.h"

static auto logger = GetLogger("main");

int main(int argc, char* argv[])
{
    logger->debug("Starting TextureAtlasApp");

    TextApp app;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}