#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace WR3CK::Internal::GL
{
const size_t getTypeBase(const GLenum glType);
const size_t getTypeBaseByteSize(const GLenum glType);
const size_t getTypeElementCount(const GLenum glType);
const size_t getTypeVertexSplitCount(const GLenum glType);
const size_t getTypeByteSize(const GLenum glType);
}