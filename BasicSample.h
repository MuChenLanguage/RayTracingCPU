#ifndef BASICSAMPLE_H
#define BASICSAMPLE_H

#include "Sample.h"

namespace RayTracing {

	class BasicSample : public Sample {
	public:
		BasicSample() {
			Init();
		}
		void Init();
		void Render();
		void ImGui();
		void ShutDown();
		void Reset();
	private:
		vec3 RayColor(const ray& ray);
	/*	void Reset();*/
	private:
		int curIndex;
		bool isGenerate;
		ImageParam mImageParam; // output image para
		// camera
		const double aspect_ratio = 16.0 / 9.0;
		double viewport_height;
		double viewport_width;
		double focal_length;

		// viewport
		vec3 origin = vec3(0, 0, 0);
		vec3 horizontal;
		vec3 vertical;
		vec3 lower_left_corner;

		// sky color
		vec3 upColor;
		vec3 downColor;

	};


}

#endif // !BASICSAMPLE_H