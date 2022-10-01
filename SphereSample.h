#ifndef SPHERESAMPLE_H
#define SPHERESAMPLE_H

#include"Sample.h"
#include"rtweekend.h"
#include"sphere.h"
#include"hittable_list.h"
#include"camera.h"

namespace RayTracing {

	class SphereSample : public Sample {
	public:
		SphereSample() {
			Init();
		};
		void Init();
		void Render();
		void ImGui();
		void ShutDown();
		void Reset();
	private:
		vec3 RayColor(ray& r);
		/*	void Reset();*/
	private:
		int curIndex;
		bool isGenerate;
		int Sample_pixels_num;
		ImageParam mImageParam; // output image para
		// camera
		const double aspect_ratio = 16.0 / 9.0;
		camera Camera;
		

		// sky color
		vec3 upColor;
		vec3 downColor;


		// sphere
		shared_ptr<sphere> sphere1;
		shared_ptr<sphere> sphere2;
		hittable_list world;
	};
}

#endif // !SPHERESAMPLE_H

