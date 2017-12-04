#include "ImageManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

namespace
{
static const int cMaxImageHandle = 256;
}

namespace tinyngine
{

struct Image {
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mSize;
	uint8_t mBitsPerPixel;
	uint32_t mNumMipmaps;
	std::vector<uint8_t*> mData;
};

struct ImageManager::Impl {
	uint32_t mImagesCount = 0;
	std::array<Image, cMaxImageHandle> mImages;
};

//=====================================================================================================================

ImageManager::ImageManager() : mImpl(new Impl()) {

}

ImageManager::~ImageManager() {}

bool ImageManager::ReleaseImage(ImageHandle& imageHandle) {
	if (imageHandle.IsValid()) {
		auto& image = mImpl->mImages[imageHandle.mHandle];
		for (auto data : image.mData) {
			stbi_image_free(data);
		}
		image.mData.clear();

		//@note: build list of released handles that can be recycled

		return true;
	}
	return false;
}

ImageHandle ImageManager::LoadImageFromFile(const char * filename) {
	if (filename) {
		ImageHandle handle = ImageHandle(mImpl->mImagesCount++);
		auto& image = mImpl->mImages[handle.mHandle];

		int x, y, n;
		uint8_t* data = stbi_load(filename, &x, &y, &n, 0);
		if (data) {
			image.mData.push_back(data);
			image.mWidth = x;
			image.mHeight = y;
			image.mSize = x * y * n;
			image.mBitsPerPixel = static_cast<uint8_t>(n * 8);

			uint32_t depth = 1;
			const uint32_t max = std::max(std::max(image.mWidth, image.mHeight), depth);
			image.mNumMipmaps = 1;// +uint32_t(std::floor(std::log2(max)));

			return handle;
		}
	}
	return ImageHandle(cInvalidHandle);
}

bool ImageManager::ImageHasMipmaps(const ImageHandle& imageHandle) const {
	uint32_t mips = ImageGetMipmapsCount(imageHandle);
	return mips > 1;
}

uint32_t ImageManager::ImageGetMipmapsCount(const ImageHandle& imageHandle) const {
	if (imageHandle.IsValid()) {
		auto& image = mImpl->mImages[imageHandle.mHandle];
		return image.mNumMipmaps;
	}
	return 0;
}

bool ImageManager::GetImageData(const ImageHandle& imageHandle, uint32_t level, ImageData& imageData) {
	if (imageHandle.IsValid()) {
		auto& image = mImpl->mImages[imageHandle.mHandle];
		if (image.mNumMipmaps > level) {
			imageData.mData = image.mData[level];
			imageData.mWidth = image.mWidth;
			imageData.mHeight = image.mHeight;
			imageData.mBitsPerPixel = image.mBitsPerPixel;
			return true;
		}
	}
	return false;
}

} // namespace tinyngine