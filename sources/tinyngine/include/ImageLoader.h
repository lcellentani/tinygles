#pragma once

#include "System.h"
#include "GraphicsTypes.h"

namespace tinyngine
{

class ImageLoader final : public System {
public:
	bool Load(const char * filename, ImageData& imageData);
};

} // namespace tinyngine