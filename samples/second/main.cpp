#include <iostream>
#include "SecondApp.h"
#include "../../engine/Renderer.h"

int main()
{
    SecondApp app;
    Renderer renderer;

    app.CreateWindow(800, 600);
    app.Run();

    return 0;
}