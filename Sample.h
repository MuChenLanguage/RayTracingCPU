#ifndef SAMPLE_H
#define SAMPLE_H

#include"vec3.h"
#include"ray.h"
#include "ImageParam.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace RayTracing {

	class Sample {
	public:
		Sample();
		virtual ~Sample();

		virtual void Init();
		virtual void Update(float time);
		virtual void Render();
		virtual void ImGui();
		virtual void ShutDown();
	private:
		Sample(const Sample&);
	};

} // namespace


#endif // !SAMPLE_H
