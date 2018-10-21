#include <iostream>
#include "BlendModesApp.h"
#include "Renderer.h"

int main()
{
    BlendModesApp app;
    Renderer renderer;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}