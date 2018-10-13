//
// Created by snorri on 12.10.2018.
//

#include "../../engine/Renderer.h"
#include "BreakOutApp.h"

int main()
{
    BreakOutApp app;
    Renderer renderer;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}