#pragma once

#include "System.h"
#include "GraphicsTypes.h"

namespace tinyngine
{

class ImageManager final : public System {
public:
	ImageManager();
	virtual ~ImageManager();

	bool ReleaseImage(ImageHandle& handle);

	ImageHandle LoadImageFromFile(const char * filename);

	bool ImageHasMipmaps(const ImageHandle& imageHandle) const;
	uint32_t ImageGetMipmapsCount(const ImageHandle& imageHandle) const;
	bool GetImageData(const ImageHandle& imageHandle, uint32_t level, ImageData& imageData);

private:
	struct Impl;
	Impl* mImpl;
};

} // namespace tinyngine