#include "GuiLayer.h"
#include "Application.h"
#include "Utitls.h"


namespace RayTracing {

	GuiLayer* GuiLayer::ImGuiInstance = nullptr;

	GuiLayer* GuiLayer::GetInstance() {
		if (ImGuiInstance == nullptr) {
			ImGuiInstance = new GuiLayer();
		}
		return ImGuiInstance;
	}

	void GuiLayer::InitGuiSyetem() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		/*ImGui::StyleColorsDark();*/
		ImGui::StyleColorsLight();
		/*ImGui::StyleColorsClassic();*/

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(Application::appInstance->GetGLFWWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");

		ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\calibri.ttf", 22.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		IM_ASSERT(font != NULL);
	}

	void GuiLayer::CloseGuiSystem() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiLayer::BeginGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GuiLayer::ShowImage(GLuint TexId, ImVec2 WindowSize) {
		if (TexId == 0) {
			TexId =  Utils::LoadTexture("Images/demo.png");
		}
		ImGui::Begin("RayTracer");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(WindowSize);
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)TexId, viewportSize);
		ImGui::End();
	}

}