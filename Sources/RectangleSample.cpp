#include "RectangleSample.h"
#include "Application.h"
#include "Utitls.h"
#include "material.h"
#include <thread>
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"
#include<omp.h>

hittable_list simple_light() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(1, 1, 1));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}


hittable_list cornell_box() {
	hittable_list objects;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.add(box2);
	return objects;
}


hittable_list final_scene() {
	hittable_list boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_float(1, 101);
			auto z1 = z0 + w;

			boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}

	hittable_list objects;

	objects.add(make_shared<bvh_node>(boxes1));

	auto light = make_shared<diffuse_light>(color(7, 7, 7));
	objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto center1 = point3(400, 400, 200);
	auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	objects.add(make_shared<sphere>(center1,50, moving_sphere_material));

	objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.add(make_shared<sphere>(
		point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
		));

	auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	objects.add(boundary);
	objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("Images/earthmap.jpg"));
	objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.1);
	objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittable_list boxes2;
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(random(0, 165), 10, white));
	}

	objects.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(boxes2), 15),
		vec3(-100, 270, 395)
		)
	);

	return objects;
}

namespace RayTracing {

	void RectangleSample::Init() {
		curIndex = 0;
		isGenerate = false;
		Sample_pixels_num = 50;
		mImageParam = ImageParam(800, double(800) / aspect_ratio, "material.png");
		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0, 1.0, 1.0);
		maxdepth = 20;

		Camera.lookat = vec3(0, 0, 0);
		Camera.lookfrom = vec3(13, 2, 3);

		Camera.Init();
	}

	void RectangleSample::Render() {
		Camera.Init();
		if (rect) {
			world = simple_light();
			Camera.lookat = vec3(0, 0, 0);
			Camera.lookfrom = vec3(26, 3, 6);
			Camera.Init();
		}
		if (cornell) {
			world = cornell_box();
			Camera.lookat = vec3(278, 278, 0);
			Camera.lookfrom = vec3(278, 278, -800);
			Camera.Init();
		}
		if (earth) {
			world = final_scene();
			Camera.lookat = vec3(278, 278, 0);
			Camera.lookfrom = vec3(478, 478, -600);
			Camera.Init();
		}

		int width = mImageParam.mWidth;
		int height = mImageParam.mHeight;
		unsigned char* pixels = new unsigned char[width * height * 3];
		int ind = 0;
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				color col;
				#pragma omp parallel for num_threads(10)  

				for (int k = 0; k < Sample_pixels_num; k++) {
					float u = double(i + random_float()) / (width - 1);
					float v = double(j + random_float()) / (height - 1);
					ray r = Camera.get_ray(u, v);
					col += RayColor(r, vec3(0,0,0), 0);
				}
				float scale = 1.0 / Sample_pixels_num;
				col *= scale;

				float r = sqrt(col.r());
				float g = sqrt(col.g());
				float b = sqrt(col.b());

				pixels[ind++] = static_cast<int>(256 * clamp(r, 0.0, 0.999));
				pixels[ind++] = static_cast<int>(256 * clamp(g, 0.0, 0.999));
				pixels[ind++] = static_cast<int>(256 * clamp(b, 0.0, 0.999));
				curIndex += 1;
			}
		}
		Utils::SaveImage("Images/" + mImageParam.mName, width, height, 3, pixels);
		isGenerate = true;
	}

	void RectangleSample::ImGui() {
		ImGui::Text("Texture");
		ImGui::Separator();
		ImGui::ColorEdit3("High", &upColor[0]);
		ImGui::ColorEdit3("Low", &downColor[0]);
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::InputFloat3("LookFrom", &Camera.lookfrom[0]);
			ImGui::InputFloat3("LookAt", &Camera.lookat[0]);
			ImGui::SliderFloat("Fov", &Camera.vfov, 0, 90);
		}
		ImGui::SliderInt("MSAA SAMPLES", &Sample_pixels_num, 1, 500);
		ImGui::Separator();
		ImGui::SliderInt("Max Depth", &maxdepth, 1, 100);
		if (ImGui::Checkbox("Rect", &rect)) {
			cornell = false;
			earth = false;
		}
		if (ImGui::Checkbox("Cornell Box", &cornell)) {
			rect = false;
			earth = false;
		}

		if (ImGui::Checkbox("Fillal", &earth)) {
			cornell = false;
			rect = false;
		}

		ImGui::ProgressBar(float(curIndex) / (mImageParam.mWidth * mImageParam.mHeight));
		if (ImGui::Button("Generate")) {
			;
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

	void RectangleSample::ShutDown() {
		Application::GetInstance()->setTexHandle(0);
	}

	void RectangleSample::Reset() {
		curIndex = 0;
		isGenerate = false;

		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0, 1.0, 1.0);

		world.clear();
		world = simple_light();
		ShutDown();
	}

	vec3 RectangleSample::RayColor(ray r, color background, int depth) {

		if (depth > maxdepth) {
			return vec3(0, 0, 0);
		}
		hit_record hit;
		if (!world.hit(r, 0.001, infinity, hit))
			return background;
		
		ray scattered;
		vec3 attenuation;
		color emitted = hit.mat_ptr->emitted(hit.u, hit.v, hit.p);
		if (!hit.mat_ptr->scatter(r, hit, attenuation, scattered)) {
			return emitted;
		}
		return emitted + attenuation * RayColor(scattered, background, depth + 1);
	}

}