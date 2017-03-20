#pragma once

#include <cstdint>
#include <memory>

namespace tinygles
{
class PlatformBridge {
public:
	PlatformBridge();
	~PlatformBridge();

	PlatformBridge(PlatformBridge& rhs);
	PlatformBridge& operator=(PlatformBridge& rhs);

	PlatformBridge(PlatformBridge&& rhs) noexcept = default;
	PlatformBridge& operator= (PlatformBridge&& rhs) noexcept = default;

public:
	int16_t Run();

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};
} // namespace tinygles