#include"Application.h"

#include<iostream>

using namespace RayTracing;



int main() {
    Application* app = Application::CreateAppInstance();
    app->InitApplication();
    app->run();


    return 0;
}
