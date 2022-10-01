#include "Application.h"
#include "BasicSample.h"
#include "SphereSample.h"
#include "MaterialSample.h"
#include "TextureSample.h"
#include "RectangleSample.h"

namespace RayTracing {

	Application* Application::appInstance = nullptr;


	void Application::SetWindow(int Width, int Height, std::string WindowName, bool isFullScreen) {
		windowParm.mHeight = Height;
		windowParm.mWidth = Width;
		windowParm.mIsFullScreen = isFullScreen;
		windowParm.mWindowName = WindowName;
	}

	Application* Application::CreateAppInstance() {
		if (appInstance == nullptr) {
			appInstance = new Application();
		}
		return appInstance;
	}

	Application* Application::GetInstance() {
		return appInstance;
	}

	void Application::InitApplication() {
		if (!CreateWindow()) {
			std::cout << "Create window success!" << std::endl;
		}
		else {
			std::cout << "Create window failed!" << std::endl;
			return;
		}
		mImGuiLayer = GuiLayer::GetInstance();
		mImGuiLayer->InitGuiSyetem();
	}

	bool Application::CreateWindow() {
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) {
			return 1;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		appInstance->GLFWwindow = glfwCreateWindow(windowParm.mWidth, windowParm.mHeight, windowParm.mWindowName.c_str(), NULL, NULL);
		if (appInstance->GLFWwindow == nullptr) {
			return 1;
		}
		if (windowParm.mIsFullScreen) {
			glfwMaximizeWindow(appInstance->GLFWwindow);
		}
		glfwMakeContextCurrent(appInstance->GLFWwindow);
		glfwSetKeyCallback(appInstance->GLFWwindow, key_callback);

		bool err = gladLoadGL() == 0;
		if (err) {
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			return 1;
		}

	}

	void Application::setTexHandle(GLuint TexID) {
		mTexHandle = TexID;
	}

	void Application::run() {
		bool p_open = true;
		while (!glfwWindowShouldClose(GLFWwindow)) {

			int dispaly_w, display_h;
			glfwGetFramebufferSize(GLFWwindow, &dispaly_w, &display_h);
			glViewport(0, 0, dispaly_w, display_h);

			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.1, 0.2, 0.2, 0);

			mImGuiLayer->BeginGui();

			{
				ImGui::Begin("Hello, world!");
				ImGui::SetWindowPos(ImVec2(dispaly_w - dispaly_w / 5, 0));
				ImGui::SetWindowSize(ImVec2(dispaly_w/5, display_h));

				/*BasicSample* sample = new BasicSample();*/
				if (mSample) {
					mSample->ImGui();
					if (ImGui::Button("Close")) {
						p_open = true;
						mSample->ShutDown();
						delete mSample;
						mSample = nullptr;
					}
				}
				if (p_open) {
					if (ImGui::CollapsingHeader("BasicSample")) {
						if (ImGui::Button("Generate Sky")) {
							mSample = new BasicSample();
							p_open = false;
						}
					}
					if (ImGui::CollapsingHeader("Sphere")) {
						if (ImGui::Button("Generate Sphere")) {
							mSample = new SphereSample();
							p_open = false;
						}
					}
					if (ImGui::CollapsingHeader("Material")) {
						if (ImGui::Button("Generate")) {
							mSample = new MaterialSample();
							p_open = false;
						}
					}
					if (ImGui::CollapsingHeader("Texture")) {
						if (ImGui::Button("Generate")) {
							mSample = new TextureSample();
							p_open = false;
						}
					}
					if (ImGui::CollapsingHeader("Rectangle")) {
						if (ImGui::Button("Generate")) {
							mSample = new RectangleSample();
							p_open = false;
						}
					}
				}
				mImGuiLayer->ShowImage(appInstance->mTexHandle, ImVec2(dispaly_w * 4 / 5, display_h));
				ImGui::End();;
			}

			

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwPollEvents();
			glfwSwapBuffers(GLFWwindow);
		}
	}
	GLFWwindow* Application::GetGLFWWindow() {
		return GLFWwindow;
	}
}
