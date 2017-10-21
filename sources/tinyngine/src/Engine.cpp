#include "Engine.h"
#include "InputWin32.h"
#include "Time.h"
#include "RendererGL.h"
#include "ImGUIWrapper.h"

namespace tinyngine
{

Engine::Engine() {
	mSystems[std::type_index(typeid(Input))] = new InputWin32();
	mSystems[std::type_index(typeid(Renderer))] = new RendererGL();
	mSystems[std::type_index(typeid(ImGUIWrapper))] = CreateImGUIWrapper();
	mSystems[std::type_index(typeid(Time))] = new Time();
}

Engine::~Engine() {
	for (auto e : mSystems) {
		delete e.second;
		e.second = nullptr;
	}
	mSystems.clear();
}

} // tinyngine