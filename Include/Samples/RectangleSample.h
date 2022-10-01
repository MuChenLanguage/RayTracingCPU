#ifndef RECTANGLESAMPLE_H
#define RECTANGLESAMPLE_H

#include"Sample.h"
#include"rtweekend.h"
#include"sphere.h"
#include"hittable_list.h"
#include"camera.h"
#include"bvh.h"

namespace RayTracing {

	class RectangleSample : public Sample {
	public:
		RectangleSample() {
			Init();
		};
		void Init();
		void Render();
		void ImGui();
		void ShutDown();
		void Reset();
	private:
		vec3 RayColor(ray r, color background, int depth);
		/*	void Reset();*/
	private:
		int curIndex;
		bool isGenerate;
		int Sample_pixels_num;
		ImageParam mImageParam; // output image para
		// camera
		const double aspect_ratio = 1.0;
		camera Camera;
		vec3 lookfrom, lookat, vup;
		double vfov; // top to bottom, in degrees

		// sky color
		vec3 upColor;
		vec3 downColor;

		// sphere
		shared_ptr<sphere> sphere1;
		shared_ptr<sphere> sphere2;
		shared_ptr<sphere> sphere3;
		shared_ptr<sphere> sphere4;
		hittable_list world;

		int maxdepth;

		// material item

		bool Diffuse = true;
		bool Metal = false;
		bool Dielectric = false;
		bool MoreSphere = false;


		//selector
		bool rect = false;
		bool cornell = true;
		bool earth = false;
	};
}
#endif // !MATERIALSAMPLE_H
