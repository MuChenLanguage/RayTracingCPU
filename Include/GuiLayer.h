#ifndef GUILAYER_H
#define GUILAYER_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>

namespace RayTracing {
	class GuiLayer {
	public:
		static GuiLayer* ImGuiInstance;
		static GuiLayer* GetInstance();

		void InitGuiSyetem();
		void CloseGuiSystem();
		void BeginGui();
		void ShowImage(GLuint TexId,ImVec2 WindowsSize);
	};
}

#endif // !GUILAYER_H

