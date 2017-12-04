#include "Engine.h"
#include "InputWin32.h"
#include "Time.h"
#include "gl/GraphicsDeviceGL.h"
#include "ImGUIWrapper.h"
#include "MeshLoader.h"
#include "ImageManager.h"

namespace tinyngine
{

Engine::Engine() {
	mSystems[std::type_index(typeid(Input))] = new InputWin32();
	mSystems[std::type_index(typeid(GraphicsDevice))] = new GraphicsDeviceGL();
	mSystems[std::type_index(typeid(ImGUIWrapper))] = CreateImGUIWrapper();
	mSystems[std::type_index(typeid(Time))] = new Time();
	mSystems[std::type_index(typeid(MeshLoader))] = new MeshLoader();
	mSystems[std::type_index(typeid(ImageManager))] = new ImageManager();
}

Engine::~Engine() {
	for (auto e : mSystems) {
		delete e.second;
		e.second = nullptr;
	}
	mSystems.clear();
}

} // tinyngine