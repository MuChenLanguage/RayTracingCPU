#include "basicsample.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "Utitls.h"
#include <thread>
#include <fstream>
#include "color.h"

namespace RayTracing {
	void BasicSample::Init() {
		curIndex = 0;
		isGenerate = false;
		mImageParam = ImageParam(400, int(400/aspect_ratio), "bg.png");
		viewport_height = 2.0;
		viewport_width = viewport_height * aspect_ratio;
		focal_length = 1.0;
		horizontal = vec3(viewport_width, 0, 0);
		vertical = vec3(0, viewport_height, 0);
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0,0,focal_length);

		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0, 1.0, 1.0);
	}

	void BasicSample::Render() {
		int width = mImageParam.mWidth;
		int height = mImageParam.mHeight;
		unsigned char* pixels = new  unsigned char[width * height * 3];
		int ind = 0;
		std::ofstream ppm("demo.ppm");
		ppm << "P3\n" << width << " " << height << "\n255\n";
		for (int j = height-1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				double u = double(i) / (width-1);
				double v = double(j) / (height-1);
				ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
				color col =  RayColor(r);
				write_color(ppm, col);
				pixels[ind++] = static_cast<int>(255.99f * col.r());
				pixels[ind++] = static_cast<int>(255.99f * col.g());
				pixels[ind++] = static_cast<int>(255.99f * col.b());
				curIndex++;
			}
		}
		Utils::SaveImage("Images/" + mImageParam.mName , width, height, 3, pixels);
		isGenerate = true;
	}

	void BasicSample::ImGui() {
		ImGui::Text(u8"BackGround");
		ImGui::Separator();
		ImGui::ColorEdit3(u8"High", &upColor[0]);
		ImGui::ColorEdit3(u8"Low", &downColor[0]);
		ImGui::Separator();
		auto w = (float)mImageParam.mWidth;
		auto h = (float)mImageParam.mHeight;
		auto progress = ((float)curIndex / (w * h));
		ImGui::ProgressBar(progress);
		if (ImGui::Button(u8"Generate")) {
			std::thread t = std::thread([=]() { Render(); });
			t.detach();
		}
		if (isGenerate) {
			
			auto mTexHandle = Utils::LoadTexture(("Images/" + mImageParam.mName).c_str());
			Application::GetInstance()->setTexHandle(mTexHandle);
			isGenerate = false;
			curIndex = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) { Reset(); }
		
	}

	void BasicSample::ShutDown() {
		Application::GetInstance()->setTexHandle(0);
	}

	void BasicSample::Reset() {
		curIndex = 0;
		isGenerate = false;

		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0,1.0,1.0);
		Application::GetInstance()->setTexHandle(0);
	}

	vec3 BasicSample::RayColor(const ray& ray) {
		vec3 unit_direction = unit_vector(ray.direction());
		double t = 0.5 * (1 + unit_direction.y());
		return t * upColor + (1 - t) * downColor;
	}


}