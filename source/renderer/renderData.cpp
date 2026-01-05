#include <WR3CK/renderer/renderData.hpp>

#define RENDERDATA_MAP(x)                        \
	x(int, GL_INT);                              \
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
	x(double, GL_DOUBLE);

namespace WR3CK::Internal
{
#define RENDERDATA_CONSTRUCTOR(type, glType)                             \
	RenderData::RenderData(const type& value) :                          \
		m_data(malloc(sizeof(type))), m_dataCount(1), m_glType(glType) { \
		new (m_data) type(value);                                        \
	}                                                                    \
	RenderData::RenderData(const std::vector<type>& value) :             \
		m_data(nullptr), m_dataCount(value.size()), m_glType(glType) {   \
		if (m_dataCount == 0)                                            \
			return;                                                      \
		m_data = malloc(sizeof(type) * m_dataCount);                     \
		for (size_t i = 0; i < m_dataCount; i++) {                       \
			new (&reinterpret_cast<type*>(m_data)[i]) type(value[i]);    \
		}                                                                \
	}
RENDERDATA_MAP(RENDERDATA_CONSTRUCTOR);
#undef RENDERDATA_CONSTRUCTOR

RenderData::RenderData(const RenderData& other) :
	m_data(nullptr), m_dataCount(other.m_dataCount), m_glType(other.m_glType) {
	switch (m_glType) {
		default: WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_glType);

#define RENDERDATA_COPY(type, glType)                                                                       \
	case glType: {                                                                                          \
		m_data = malloc(sizeof(type) * m_dataCount);                                                        \
		for (size_t i = 0; i < m_dataCount; i++) {                                                          \
			new (&reinterpret_cast<type*>(m_data)[i]) type(reinterpret_cast<const type*>(other.m_data)[i]); \
		}                                                                                                   \
	} break;
			RENDERDATA_MAP(RENDERDATA_COPY);
#undef RENDERDATA_COPY
	}
}
RenderData::~RenderData() {
	switch (m_glType) {
		default: WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_glType);

#define RENDERDATA_DESTRUCTOR(type, glType)                           \
	case glType: {                                                    \
		if (std::is_destructible_v<type>) {                           \
			for (size_t i = 0; i < m_dataCount; i++) {                \
				std::destroy_at(&reinterpret_cast<type*>(m_data)[i]); \
			}                                                         \
		}                                                             \
	} break;
			RENDERDATA_MAP(RENDERDATA_DESTRUCTOR);
#undef RENDERDATA_DESTRUCTOR
	}
	WR3CK_CLEANUP(m_data, free(m_data));
}

const bool RenderData::operator==(const RenderData& other) const {
	if (m_glType != other.m_glType)
		return false;

	switch (m_glType) {
#define RENDERDATA_COMPARE(type, glType) \
	case glType: return *reinterpret_cast<type*>(m_data) == *reinterpret_cast<type*>(m_data);
		RENDERDATA_MAP(RENDERDATA_COMPARE);
#undef RENDERDATA_COMPARE
	}

	WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_glType);
	return true;
}
const bool RenderData::operator<(const RenderData& other) const {
	if (m_glType != other.m_glType)
		return m_glType < other.m_glType;

	switch (m_glType) {
#define RENDERDATA_COMPARE(type, glType) \
	case glType: return *reinterpret_cast<type*>(m_data) < *reinterpret_cast<type*>(m_data);
		RENDERDATA_MAP(RENDERDATA_COMPARE);
#undef RENDERDATA_COMPARE
	}

	WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_glType);
	return false;
}
}

#undef RENDERDATA_MAP