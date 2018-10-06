#include <iostream>
#include "SecondApp.h"

#include "../../engine/Renderer.h"
#include "../../engine/Logger.h"

static auto logger = GetLogger("main");

int main()
{
    logger->debug("Starting SecondApp");

    SecondApp app;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}