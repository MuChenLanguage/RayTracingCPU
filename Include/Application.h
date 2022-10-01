#ifndef APPLICATION_H
#define APPLICATION_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"GuiLayer.h"
#include"Sample.h"
#include<string>


namespace RayTracing {

    struct ApplicationWindow {
        ApplicationWindow(std::string name = "RayTracer", int width = 1280, int height = 960, bool isFullScreen = true)
            : mWindowName(name), mWidth(width), mHeight(height), mIsFullScreen(isFullScreen) {
        }
        std::string mWindowName;
        int mWidth;
        int mHeight;
        bool mIsFullScreen;
    };

    static void glfw_error_callback(int error, const char* description) {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    class Application {

    public:
        static Application* appInstance;
        Sample* mSample;
        void SetWindow(int Width, int Height, std::string WindowName, bool isFullScreen);
        static Application* CreateAppInstance();
        static Application* GetInstance();
        void InitApplication();
        bool CreateWindow();
        void setTexHandle(GLuint TexID);
        GLFWwindow* GetGLFWWindow();
        void run();

        ~Application() {
            mImGuiLayer->CloseGuiSystem();
            glfwDestroyWindow(GLFWwindow);
            glfwTerminate();
        }


    private:
        GLFWwindow* GLFWwindow;
        ApplicationWindow windowParm;
        GuiLayer* mImGuiLayer;
        GLuint mTexHandle;
        
    };

}

#endif // !APPLICATION_H

