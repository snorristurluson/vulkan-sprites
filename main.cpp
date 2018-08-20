#include <iostream>
#include "app.h"
#include "Renderer.h"

int main()
{
    App app;
    Renderer renderer;

    renderer.createInstance(true);
    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}