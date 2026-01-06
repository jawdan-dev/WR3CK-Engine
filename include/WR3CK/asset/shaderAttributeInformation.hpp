#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK::Internal
{
struct ShaderAttributeInformation {
	size_t m_dataSize, m_dataOffset;
	GLint m_location;
	GLenum m_glType;
	bool m_isStatic;
};
}
