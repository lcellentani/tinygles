#include "ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <memory>

namespace tinyngine
{

bool ImageLoader::Load(const char * filename, ImageData& imageData) {
	bool result = false;
	if (filename) {
		std::memset(&imageData, 0, sizeof(ImageData));

		int x, y, n;
		imageData.mData = static_cast<void*>(stbi_load(filename, &x, &y, &n, 0));
		imageData.mWidth = x;
		imageData.mHeight = y;
		imageData.mSize = x * y * n;
		imageData.mBitsPerPixel = static_cast<uint8_t>(n * 8);
		result = true;
	}
	return result;
}

} // namespace tinyngine