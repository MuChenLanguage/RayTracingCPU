#ifndef IMAGEPARAM_H
#define IMAGEPARAM_H
#include <string>
namespace RayTracing {

	class ImageParam {
	public:
		int mWidth;
		int mHeight;
		int mChannel;
		std::string mName;
	public:
		ImageParam() {}
		ImageParam(uint32_t w, uint32_t h, const std::string& name, uint32_t channel = 3)
			:mWidth(w), mHeight(h), mName(name), mChannel(channel) {
		}
	};

}

#endif // !IMAGEPARAM_H