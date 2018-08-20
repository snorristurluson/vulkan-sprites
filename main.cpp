#include <iostream>
#include "app.h"
#include "Renderer.h"

int main()
{
    App app;
    Renderer renderer;

    app.CreateWindow(800, 600);
    renderer.Initialize(app.GetWindow(), Renderer::ENABLE_VALIDATION);
    app.Run();

    return 0;
}