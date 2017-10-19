#pragma once

#include "System.h"

#include <typeinfo>
#include <typeindex>
#include <unordered_map>

namespace tinyngine
{

class Engine final {
public:
	template<typename S>
	S& GetSystem() {
		return *(reinterpret_cast<S*>(mSystems[std::type_index(typeid(S))]));
	}
	template<typename S>
	const S& GetSystem() const {
		return *(reinterpret_cast<S*>(mSystems[std::type_index(typeid(S))]));
	}
public:
	Engine();
	~Engine();

private:
	std::unordered_map<std::type_index, System*> mSystems;
};

} // namespace tinyngine