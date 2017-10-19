#include "Engine.h"
#include "InputWin32.h"
#include "Time.h"

namespace tinyngine
{

Engine::Engine() {
	mSystems[std::type_index(typeid(Input))] = new InputWin32();
	mSystems[std::type_index(typeid(Time))] = new Time();
	//mSystems[std::type_index(typeid(RendererSystem))] = new RendererSystem();
}

Engine::~Engine() {
	for (auto e : mSystems) {
		delete e.second;
		e.second = nullptr;
	}
	mSystems.clear();
}

} // tinyngine