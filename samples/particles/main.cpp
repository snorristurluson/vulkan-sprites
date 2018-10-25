//
// Created by snorri on 25.10.2018.
//

#include <iostream>
#include "ParticlesApp.h"

#include "Renderer.h"
#include "Logger.h"
#include "telemetry.h"

static auto logger = GetLogger("main");

int main(int argc, char* argv[])
{
    ConnectTelemetry();

    logger->debug("Starting ParticlesApp");

    ParticlesApp app;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}