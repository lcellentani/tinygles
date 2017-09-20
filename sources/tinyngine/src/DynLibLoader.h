#pragma once

#include <memory>
#include <string>

namespace tinyngine
{
class DynLibLoader final {
public:
	DynLibLoader() = delete;
	DynLibLoader(DynLibLoader& rhs);
	DynLibLoader& operator=(DynLibLoader& rhs);

	DynLibLoader(DynLibLoader&& rhs);
	DynLibLoader& operator= (DynLibLoader&& rhs);

	DynLibLoader(const std::string& libraryPath);
	~DynLibLoader();

	bool HasBeenLoaded() const;

	void* GetFunction(const char* functionName);

	template<typename T>
	T GetFunction(const char* functionName) {
		return reinterpret_cast<T>(GetFunction(functionName));
	}

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};
} // namespace tinyngine