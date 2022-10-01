#include "SphereSample.h"
#include "Utitls.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "material.h"
#include <thread>

namespace RayTracing {

	void SphereSample::Init() {
		curIndex = 0;
		isGenerate = false;
		Sample_pixels_num = 1;
		mImageParam = ImageParam(400, int(400 /  aspect_ratio), "sphere.png");

		Camera.Init();

		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0, 1.0, 1.0);
		shared_ptr<lambertian> lamber = make_shared<lambertian>(color(0, 0, 0));
		sphere1 = make_shared<sphere>(vec3(0, 0, -1), 0.5, lamber);
		sphere2 = make_shared<sphere>(vec3(0, -100.5, -1),100, lamber);
		world.add(sphere1);
		world.add(sphere2);
	}

	void SphereSample::Render() {
		Camera.Init();
		int width = mImageParam.mWidth;
		int height = mImageParam.mHeight;
		unsigned char* pixels = new unsigned char[width * height * 3];
		
		int ind = 0;
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				color col;
				for (int k = 0; k < Sample_pixels_num; k++) {
					double u = double(i + random_float()) / (width - 1);
					double v = double(j + random_float()) / (height - 1);
					ray r = Camera.get_ray(u, v);
					col += RayColor(r);
				}
				col /= Sample_pixels_num;
				pixels[ind++] = static_cast<int>(255.999 * col.r());
				pixels[ind++] = static_cast<int>(255.999 * col.g());
				pixels[ind++] = static_cast<int>(255.999 * col.b());
				curIndex += 1;
			}
		}
		Utils::SaveImage("Images/" + mImageParam.mName, width, height, 3, pixels);
		isGenerate = true;
	}

	void SphereSample::ImGui() {
		ImGui::Text("Sphere");
		ImGui::Separator();
		ImGui::ColorEdit3("High", &upColor[0]);
		ImGui::ColorEdit3("Low", &downColor[0]);
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Sphere 1")) {
			ImGui::InputFloat3("Sphere1 Center", &sphere1->center[0]);
			ImGui::SliderFloat("Sphere1 Radius", &sphere1->radius, 0, 1);
		}
		if (ImGui::CollapsingHeader("Sphere 2")) {
			ImGui::InputFloat3("Sphere2 Center", &sphere2->center[0]);
			ImGui::SliderFloat("Sphere2 Radius", &sphere2->radius, 0, 150);
		}
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::InputFloat3("LookFrom", &Camera.lookfrom[0]);
			ImGui::InputFloat3("LookAt", &Camera.lookat[0]);
			ImGui::SliderFloat("Fov", &Camera.vfov,0,90);
		}
		ImGui::Separator();
		ImGui::SliderInt("MSAA SAMPLES", &Sample_pixels_num, 1, 150);
		ImGui::ProgressBar(float(curIndex) / (mImageParam.mWidth * mImageParam.mHeight));
		if (ImGui::Button("Generate")) {
			std::thread t = std::thread([=]() { Render(); });
			t.detach();
		}
		if (isGenerate) {
			GLuint TexHandle = Utils::LoadTexture(("Images/" + mImageParam.mName).c_str());
			Application::GetInstance()->setTexHandle(TexHandle);
			curIndex = 0;
			isGenerate = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}

	void SphereSample::ShutDown() {
		Application::GetInstance()->setTexHandle(0);
	}

	void SphereSample::Reset() {
		isGenerate = false;
		curIndex = 0;

		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1, 1, 1);
		Application::GetInstance()->setTexHandle(0);
	}

	vec3 SphereSample::RayColor(ray& r) {
		hit_record hit;
		if (world.hit(r, 0, infinity, hit)) {
			vec3 col = (hit.normal + vec3(1.0, 1.0, 1.0)) * 0.5;
			return col;
		}
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (1 + unit_direction.y());
		return upColor * t + (1 - t) * downColor;
	}

}