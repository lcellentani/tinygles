#pragma once

#include "GraphicsTypes.h"
#include "GLApi.h"
#include "ShaderGL.h"

namespace tinyngine
{

class ProgramGL {
public:
	ProgramGL() : mId(0) {}

	void Create(const ShaderGL& vs, const ShaderGL& fs);
	void Initialize();

	void BindAttributes();
	void UnbindAttributes();

	const Uniforms& GetUniform(Uniforms::Enum uniform) const;

	inline GLint GetId() const { return mId; }
	inline bool IsValid() const { return mId > 0; }

private:
	GLint mId;
	GLint mAttributes[Attributes::Count];
	uint16_t mUsedAttributesCount;
	uint8_t mUsedAttributes[Attributes::Count];
	Uniforms mPredefinedUniforms[Uniforms::Count];
};

} // namespace tinyngine