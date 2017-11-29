#pragma once

#include "System.h"
#include "GraphicsTypes.h"

namespace tinyngine
{

class ImagesManager final : public System {
public:
	ImagesManager();
	virtual ~ImagesManager();

	bool ReleaseImage(ImageHandle& handle);

	ImageHandle LoadImageFromFile(const char * filename);

	bool ImageHasMipmaps(const ImageHandle& imaageHandle);
	bool GetImageData(const ImageHandle& imageHandle, uint32_t level, ImageData& imageData);

private:
	struct Impl;
	Impl* mImpl;
};

} // namespace tinyngine