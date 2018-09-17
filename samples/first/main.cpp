#include <iostream>
#include "FirstApp.h"
#include "../../engine/Renderer.h"

int main()
{
    FirstApp app;
    Renderer renderer;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}