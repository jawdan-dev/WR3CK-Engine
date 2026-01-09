#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/texture.hpp>

#define RENDERDATA_MAP(x)                        \
	x(int, GL_INT);                              \
	x(unsigned int, GL_UNSIGNED_INT);            \
	x(float, GL_FLOAT);                          \
	x(Vector2, GL_FLOAT_VEC2);                   \
	x(Vector3, GL_FLOAT_VEC3);                   \
	x(Vector4, GL_FLOAT_VEC4);                   \
	x(ARG(Math::Matrix<2, 2>), GL_FLOAT_MAT2);   \
	x(ARG(Math::Matrix<2, 3>), GL_FLOAT_MAT2x3); \
	x(ARG(Math::Matrix<2, 4>), GL_FLOAT_MAT2x4); \
	x(ARG(Math::Matrix<3, 3>), GL_FLOAT_MAT3);   \
	x(ARG(Math::Matrix<3, 2>), GL_FLOAT_MAT3x2); \
	x(ARG(Math::Matrix<3, 4>), GL_FLOAT_MAT3x4); \
	x(ARG(Math::Matrix<4, 4>), GL_FLOAT_MAT4);   \
	x(ARG(Math::Matrix<4, 2>), GL_FLOAT_MAT4x2); \
	x(double, GL_DOUBLE);                        \
	x(Texture, GL_SAMPLER_2D);

namespace WR3CK::Internal
{
class RenderData {
public:
#define RENDERDATA_CONSTRUCTOR(type, glType) \
	RenderData(const type& value);           \
	RenderData(const std::vector<type>& value);
	RENDERDATA_MAP(RENDERDATA_CONSTRUCTOR);
#undef RENDERDATA_CONSTRUCTOR
	RenderData(const RenderData& other);
	~RenderData();

	WR3CK_GETTER_CONST(size_t, dataCount, m_dataCount);
	WR3CK_GETTER_CONST(GLenum, glType, m_glType);

	const void* data() const;
	const size_t dataSize() const;

	RenderData& operator=(const RenderData& other);
#define RENDERDATA_ASSIGN(type, glType)       \
	RenderData& operator=(const type& value); \
	RenderData& operator=(const std::vector<type>& value);
	RENDERDATA_MAP(RENDERDATA_ASSIGN);
#undef RENDERDATA_ASSIGN
	const bool operator==(const RenderData& other) const;
	const bool operator<(const RenderData& other) const;

private:
	void* getData();
	void freeData();

	size_t m_dataCount;
	union {
		void* m_allocatedData;
		uint8_t m_stackData[sizeof(Vector4)];
	};
	GLenum m_glType;
};
}

#undef RENDERDATA_MAP