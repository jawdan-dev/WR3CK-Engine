#include <WR3CK/renderer/renderData.hpp>

#define RENDERDATA_MAP(x)                               \
	x(int, GL_INT, Int)                                 \
	x(unsigned int, GL_UNSIGNED_INT, UnsignedInt)       \
	x(float, GL_FLOAT, Float)                           \
	x(Vector2, GL_FLOAT_VEC2, Vector2)                  \
	x(Vector3, GL_FLOAT_VEC3, Vector3)                  \
	x(Vector4, GL_FLOAT_VEC4, Vector4)                  \
	x(ARG(Math::Matrix<2, 2>), GL_FLOAT_MAT2, Mat2)     \
	x(ARG(Math::Matrix<2, 3>), GL_FLOAT_MAT2x3, Mat2x3) \
	x(ARG(Math::Matrix<2, 4>), GL_FLOAT_MAT2x4, Mat2x4) \
	x(ARG(Math::Matrix<3, 3>), GL_FLOAT_MAT3, Mat3)     \
	x(ARG(Math::Matrix<3, 2>), GL_FLOAT_MAT3x2, Mat3x2) \
	x(ARG(Math::Matrix<3, 4>), GL_FLOAT_MAT3x4, Mat3x4) \
	x(ARG(Math::Matrix<4, 4>), GL_FLOAT_MAT4, Mat4)     \
	x(ARG(Math::Matrix<4, 2>), GL_FLOAT_MAT4x2, Mat4x2) \
	x(double, GL_DOUBLE, Double)                        \
	x(TextureHandle, GL_SAMPLER_2D, Texture)            \
	x(RenderTextureHandle, GL_SAMPLER_2D, RenderTexture)

namespace WR3CK::Internal
{
#define RENDERDATA_CONSTRUCTOR(type, glType, enum)                                        \
	RenderData::RenderData(const type& value) :                                           \
		m_allocatedData(nullptr), m_dataCount(1), m_dataType(DataType::enum) {            \
		new (getData()) type(value);                                                      \
	}                                                                                     \
	RenderData::RenderData(const std::vector<type>& value) :                              \
		m_allocatedData(nullptr), m_dataCount(value.size()), m_dataType(DataType::enum) { \
		void* const data = getData();                                                     \
		for (size_t i = 0; i < m_dataCount; i++) {                                        \
			new (&reinterpret_cast<type*>(data)[i]) type(value[i]);                       \
		}                                                                                 \
	}
RENDERDATA_MAP(RENDERDATA_CONSTRUCTOR);
#undef RENDERDATA_CONSTRUCTOR

RenderData::RenderData(const RenderData& other) :
	m_allocatedData(nullptr), m_dataCount(other.m_dataCount), m_dataType(other.m_dataType) {
	switch (m_dataType) {
		default:
			WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
			break;

#define RENDERDATA_COPY(type, glType, enum)                                                               \
	case DataType::enum: {                                                                                \
		void* const data = getData();                                                                     \
		for (size_t i = 0; i < m_dataCount; i++) {                                                        \
			new (&reinterpret_cast<type*>(data)[i]) type(reinterpret_cast<const type*>(other.data())[i]); \
		}                                                                                                 \
	} break;
			RENDERDATA_MAP(RENDERDATA_COPY);
	}
}
RenderData::~RenderData() {
	switch (m_dataType) {
		default:
			WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
			break;

#define RENDERDATA_DESTRUCTOR(type, glType, enum)                        \
	case DataType::enum: {                                               \
		if (std::is_destructible_v<type>) {                              \
			for (size_t i = 0; i < m_dataCount; i++) {                   \
				std::destroy_at(&reinterpret_cast<type*>(getData())[i]); \
			}                                                            \
		}                                                                \
	} break;
			RENDERDATA_MAP(RENDERDATA_DESTRUCTOR);
#undef RENDERDATA_DESTRUCTOR
	}
	freeData();
}

const GLuint RenderData::glType() const {
	switch (m_dataType) {
#define RENDERDATA_TYPE(type, glType, enum) \
	case DataType::enum: return glType;
		RENDERDATA_MAP(RENDERDATA_TYPE);
#undef RENDERDATA_TYPE
	}
	WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
}
const void* RenderData::data() const {
	if (dataSize() <= sizeof(m_stackData)) {
		return reinterpret_cast<const void*>(m_stackData);
	}
	return m_allocatedData;
}
const size_t RenderData::dataSize() const {
	switch (m_dataType) {
		default:
			WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
			break;
#define RENDERDATA_SIZEOF(type, glType, enum) \
	case DataType::enum: return sizeof(type) * m_dataCount;
			RENDERDATA_MAP(RENDERDATA_SIZEOF)
#undef RENDERDATA_SIZEOF
	}
	return 0;
}

RenderData& RenderData::operator=(const RenderData& other) {
	RenderData::~RenderData();
	m_dataCount = other.m_dataCount;
	m_dataType = other.m_dataType;
	switch (m_dataType) {
		default:
			WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
			break;

			RENDERDATA_MAP(RENDERDATA_COPY);
	}
#undef RENDERDATA_COPY
	return *this;
}
#define RENDERDATA_ASSIGN(type, glType, enum)                           \
	RenderData& RenderData::operator=(const type& value) {              \
		RenderData::~RenderData();                                      \
		m_dataCount = 1;                                                \
		m_dataType = DataType::enum;                                    \
		new (getData()) type(value);                                    \
		return *this;                                                   \
	}                                                                   \
	RenderData& RenderData::operator=(const std::vector<type>& value) { \
		RenderData::~RenderData();                                      \
		m_dataCount = value.size();                                     \
		m_dataType = DataType::enum;                                    \
		void* const data = getData();                                   \
		for (size_t i = 0; i < m_dataCount; i++) {                      \
			new (&reinterpret_cast<type*>(data)[i]) type(value[i]);     \
		}                                                               \
		return *this;                                                   \
	}
RENDERDATA_MAP(RENDERDATA_ASSIGN);
#undef RENDERDATA_ASSIGN
const bool RenderData::operator==(const RenderData& other) const {
	if (m_dataType != other.m_dataType)
		return false;

	// TODO: Array check....
	switch (m_dataType) {
#define RENDERDATA_COMPARE(type, glType, enum) \
	case DataType::enum: return *reinterpret_cast<const type*>(data()) == *reinterpret_cast<const type*>(other.data());
		RENDERDATA_MAP(RENDERDATA_COMPARE);
#undef RENDERDATA_COMPARE
	}

	WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
	return true;
}
const bool RenderData::operator<(const RenderData& other) const {
	if (m_dataType != other.m_dataType)
		return m_dataType < other.m_dataType;

	switch (m_dataType) {
#define RENDERDATA_COMPARE(type, glType, enum) \
	case DataType::enum: return *reinterpret_cast<const type*>(data()) < *reinterpret_cast<const type*>(other.data());
		RENDERDATA_MAP(RENDERDATA_COMPARE);
#undef RENDERDATA_COMPARE
	}

	WR3CK_ERROR("Unsupported RenderData type \"%x\".", m_dataType);
	return false;
}

void* RenderData::getData() {
	const size_t size = dataSize();
	if (size <= sizeof(m_stackData)) {
		return reinterpret_cast<void*>(&m_stackData[0]);
	}
	if (m_allocatedData == nullptr) {
		m_allocatedData = realloc(m_allocatedData, size);
		WR3CK_ASSERT(m_allocatedData != nullptr, "Failed to allocate RenderData.");
	}
	return m_allocatedData;
}
void RenderData::freeData() {
	if (dataSize() <= sizeof(m_stackData)) {
		return;
	}
	WR3CK_CLEANUP(m_allocatedData, free(m_allocatedData));
	m_dataCount = 0;
}
}

#undef RENDERDATA_MAP