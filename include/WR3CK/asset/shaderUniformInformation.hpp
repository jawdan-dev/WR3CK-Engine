#pragma once
#include <WR3CK/core/core.hpp>
#include <WR3CK/core/opengl.hpp>

namespace WR3CK::Internal
{
struct ShaderUniformInformation {
	GLint m_location;
	GLenum m_glType;
};
}