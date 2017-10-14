#include "DynLibLoader.h"
#include <string>

namespace {
#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
	typedef HINSTANCE LIBTYPE;

	LIBTYPE OpenLibrary(const char* libraryPath) {
#if defined(_UNICODE) // UNDER_CE
		if (libraryPath == nullptr) {
			return NULL;
		}

		wchar_t libraryPathW[_MAX_PATH]{ 0 };
		DWORD i = 0;
		for (i = 0; i <= strlen(libraryPath); ++i) {
			libraryPathW[i] = static_cast<wchar_t>(libraryPath[i]);
		}

		libraryPathW[i] = '\0';
		return LoadLibraryW(libraryPathW);
#else
		return LoadLibraryA(libraryPath);
#endif
	}

	void CloseLibrary(LIBTYPE libraryHandle) {
		if (libraryHandle != nullptr) {
			FreeLibrary(libraryHandle);
		}
	}

	void* GetLibraryFunction(LIBTYPE libraryHandle, const char* functionName) {
		if (libraryHandle != nullptr) {
#if defined(UNDER_CE)
			return GetProcAddressA(libraryHandle, functionName);
#else
			return GetProcAddress(libraryHandle, functionName);
#endif
		}
		return NULL;
	}
#elif defined( __linux__ ) || defined(__QNXNTO__) || defined(__APPLE__)
#else
#endif
}

namespace tinyngine
{
struct DynLibLoader::Impl {
	void* mLibraryHandle = nullptr;
	bool mLastError = false;
};

DynLibLoader::DynLibLoader(const std::string& libraryPath) : mImpl(std::unique_ptr<Impl>(new Impl())) {
	size_t start = 0;
	std::string tmp;

	while (mImpl->mLibraryHandle == nullptr)
	{
		size_t end = libraryPath.find_first_of(';', start);

		if (end == std::string::npos) {
			tmp = libraryPath.substr(start, libraryPath.length() - start);
		} else {
			tmp = libraryPath.substr(start, end - start);
		}

		if (!tmp.empty()) {
			mImpl->mLibraryHandle = ::OpenLibrary(tmp.c_str());
			if (mImpl->mLibraryHandle == nullptr)
			{
				// Remove the last character in case a new line character snuck in
				tmp = tmp.substr(0, tmp.size() - 1);
				mImpl->mLibraryHandle = ::OpenLibrary(tmp.c_str());
			}
		}
		if (end == std::string::npos) {
			break;
		}
		start = end + 1;
	}

	mImpl->mLastError = false;
	if (mImpl->mLibraryHandle == nullptr) {
		//Log(Logger.Critical, "Could not load host library '%s'", LibPath.c_str());
		mImpl->mLastError = true;
	} else {
		//Log(Logger.Debug, "Host library '%s' loaded", LibPath.c_str());
	}
}

DynLibLoader::~DynLibLoader() {
	if (!mImpl->mLibraryHandle)
	{
		::CloseLibrary((LIBTYPE)mImpl->mLibraryHandle);
	}
	mImpl->mLibraryHandle = 0;
}

DynLibLoader::DynLibLoader(DynLibLoader& rhs) : mImpl(std::unique_ptr<Impl>(new Impl(*rhs.mImpl))) {

}

DynLibLoader& DynLibLoader::operator=(DynLibLoader& rhs) {
	*mImpl = *rhs.mImpl;
	return (*this);
}

DynLibLoader::DynLibLoader(DynLibLoader&& rhs) = default;
DynLibLoader& DynLibLoader::operator= (DynLibLoader&& rhs) = default;

bool DynLibLoader::HasBeenLoaded() const {
	return !mImpl->mLastError;
}

void* DynLibLoader::GetFunction(const char* functionName) {
	void* funcPtr = ::GetLibraryFunction((LIBTYPE)mImpl->mLibraryHandle, functionName);

	mImpl->mLastError = false;
	if (!funcPtr) {
		mImpl->mLastError = true;
		//Log("Could not get function %s", pszName);
	}

	return funcPtr;
}
} // namespace tinyngine