#include "TextureSample.h"
#include "Application.h"
#include "Utitls.h"
#include "material.h"
#include <thread>
namespace RayTracing {

	hittable_list random_scene() {
		hittable_list world;

		auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
		world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

		int i = 1;
		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = random_float();
				vec3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());
				if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
					if (choose_mat < 0.8) {
						// diffuse
						auto albedo = random() * random();
						world.add(
							make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
					}
					else if (choose_mat < 0.95) {
						// metal
						auto albedo = random(.5, 1);
						auto fuzz = random_float(0, .5);
						world.add(
							make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
					}
					else {
						// glass
						world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
					}
				}
			}
		}

		world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

		world.add(
			make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

		world.add(
			make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

		return static_cast<hittable_list>(make_shared<bvh_node>(world));
	}

	hittable_list two_perlin_spheres() {
		hittable_list objects;

		auto pertext = make_shared<noise_texture>(4);
		objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
		objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

		return objects;
	}

	hittable_list earth_sphere() {
		auto earth_texture = make_shared<image_texture>("Images/earthmap.jpg");
		auto earth_surface = make_shared<lambertian>(earth_texture);
		auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

		return hittable_list(globe);
	}

	void TextureSample::Init() {
		curIndex = 0;
		isGenerate = false;
		Sample_pixels_num = 50;
		mImageParam = ImageParam(600, double(600) / aspect_ratio, "material.png");
		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0, 1.0, 1.0);
		maxdepth = 20;

		Camera.lookat = vec3(0, 0, 0);
		Camera.lookfrom = vec3(13, 2, 3);

		Camera.Init();

		shared_ptr<lambertian> lamber = make_shared<lambertian>(color(0.5, 0.5, 0.5));
		sphere1 = make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.8, 0.8, 0.0)));
		sphere2 = make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0)));
		sphere3 = make_shared<sphere>(vec3(1, 0, -1), 0.5, lamber);
		sphere4 = make_shared<sphere>(vec3(-1, 0, -1), 0.5, lamber);
		world = random_scene();
	}

	void TextureSample::Render() {
		Camera.Init();
		if (checker) {
			world.clear();
			world = random_scene();
		}
		if (perlin) {
			world.clear();
			world = two_perlin_spheres();
		}
		if (earth) {
			world.clear();
			world = earth_sphere();
		}
		int width = mImageParam.mWidth;
		int height = mImageParam.mHeight;
		unsigned char* pixels = new unsigned char[width * height * 3];
		int ind = 0;
		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				color col;
				for (int k = 0; k < Sample_pixels_num; k++) {
					float u = double(i + random_float()) / (width - 1);
					float v = double(j + random_float()) / (height - 1);
					ray r = Camera.get_ray(u, v);
					col += RayColor(r, 0);
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

	void TextureSample::ImGui() {
		ImGui::Text("Texture");
		ImGui::Separator();
		ImGui::ColorEdit3("High", &upColor[0]);
		ImGui::ColorEdit3("Low", &downColor[0]);
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
			ImGui::SliderFloat("Fov", &Camera.vfov, 0, 90);
		}
		ImGui::SliderInt("MSAA SAMPLES", &Sample_pixels_num, 1, 150);
		ImGui::Separator();
		ImGui::SliderInt("Max Depth", &maxdepth, 1, 100);
		if (ImGui::Checkbox("CHECKOR", &checker)) {
			perlin = false;
			earth = false;
		}
		if (ImGui::Checkbox("PERLIN", &perlin)) {
			checker = false;
			earth = false;
		}
		if (ImGui::Checkbox("EARTH", &earth)) {
			checker = false;
			perlin = false;
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

	void TextureSample::ShutDown() {
		Application::GetInstance()->setTexHandle(0);
	}

	void TextureSample::Reset() {
		curIndex = 0;
		isGenerate = false;

		upColor = vec3(0.5, 0.7, 1.0);
		downColor = vec3(1.0, 1.0, 1.0);

		world.clear();
		world = random_scene();
		ShutDown();
	}

	vec3 TextureSample::RayColor(ray r, int depth) {

		if (depth > maxdepth) {
			return vec3(0, 0, 0);
		}
		hit_record hit;
		if (world.hit(r, 0.001, infinity, hit)) {
			ray scattered;
			vec3 attenuation;
			if (hit.mat_ptr->scatter(r, hit, attenuation, scattered)) {
				return attenuation * RayColor(scattered, depth + 1);
			}
			else {
				return color(0, 0, 0);
			}
		}
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1);
		return upColor * t + (1 - t) * downColor;
	}

}