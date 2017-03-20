--

solution "tinygles"
	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"x32",
		"x64",
--		"Xbox360",
		"Native", -- for targets where bitness is not specified
	}

	language "C++"
	startproject "tinygles"

MODULE_DIR = path.getabsolute("../")
PRJ_DIR = path.getabsolute("..")

local PRJ_BUILD_DIR = path.join(PRJ_DIR, ".build")
local PRJ_THIRD_PARTY_DIR = path.join(PRJ_DIR, "3rdparty")

dofile (path.join(PRJ_DIR, "scripts/toolchain.lua"))
if not toolchain(PRJ_BUILD_DIR, PRJ_THIRD_PARTY_DIR) then
	return -- no action specified
end

function copyLib()
end

function mainProject(_name)
	project (_name)
		uuid (os.uuid(_name))
		kind "WindowedApp"

	configuration {}

	debugdir (path.join(PRJ_DIR, "media/"))

		defines {
			"_HAS_ITERATOR_DEBUGGING=0"
		}
	
	includedirs {
		path.join(PRJ_DIR, "sources/"),
		path.join(PRJ_BUILD_DIR, "../3rdparty/khronos/include"),
	}

	files {
		path.join(PRJ_DIR, "sources/", "**.c"),
		path.join(PRJ_DIR, "sources/", "**.cpp"),
		path.join(PRJ_DIR, "sources/", "**.h"),
	}

	removefiles {
		path.join(PRJ_DIR, "sources/", "**.bin.h"),
	}
	
	configuration { "x32", "vs*" }
		libdirs {
			path.join(PRJ_BUILD_DIR, "win32_" .. _ACTION, "bin"),
			path.join(PRJ_BUILD_DIR, "../3rdparty/khronos/lib"),
		}

	configuration { "x64", "vs*" }
		libdirs {
			path.join(PRJ_BUILD_DIR, "win64_" .. _ACTION, "bin"),
			path.join(PRJ_BUILD_DIR, "../3rdparty/khronos/lib"),
		}
		
	configuration { "vs*", "x32 or x64" }
		linkoptions {
			"/ignore:4199", -- LNK4199: /DELAYLOAD:*.dll ignored; no imports found from *.dll
		}
		
	configuration { "vs20*", "Debug" }
		links {
			"libEGL",
			"libGLESv2"
		}
		
	configuration { "vs20*", "Release" }
		links {
			"libEGL",
			"libGLESv2"
		}
		
	configuration {}

	strip()
end

mainProject("tinygles")


