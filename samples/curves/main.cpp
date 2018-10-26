//
// Created by snorri on 26.10.2018.
//

#include "CurvesApp.h"

#include "Renderer.h"
#include "Logger.h"
#include "telemetry.h"

static auto logger = GetLogger("main");

int main(int argc, char* argv[])
{
    ConnectTelemetry();

    logger->debug("Starting CurvesApp");

    CurvesApp app;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}