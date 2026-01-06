#include <WR3CK/asset/shader.hpp>

namespace WR3CK
{
ShaderData::ShaderData(const std::string& source) :
	m_uniforms(), m_attributes(),
	m_attributeInstanceTotalSize(0), m_attributeStaticTotalSize(0),
	m_program(GL_NONE) {
	compile(source);
}
ShaderData::~ShaderData() {
	WR3CK_CLEANUP_GL(m_program, glDeleteProgram(m_program));
}

const GLuint ShaderData::compileShader(const std::string& source, const GLenum shaderType) {
	WR3CK_ASSERT(source.size() > 0, "No given shader source.");

	GLuint shader = glCreateShader(shaderType);
	WR3CK_ASSERT(shader != 0, "Failed to create shader.");

	const char* sourceCstr = source.c_str();
	const GLint sourceLength = source.size();
	glShaderSource(shader, 1, &sourceCstr, &sourceLength);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		switch (shaderType) {
			case GL_VERTEX_SHADER: {
				WR3CK_ERROR("Vertex shader compilation failed.\n%s", infoLog);
			} break;
			case GL_GEOMETRY_SHADER: {
				WR3CK_ERROR("Geometry shader compilation failed.\n%s", infoLog);
			} break;
			case GL_FRAGMENT_SHADER: {
				WR3CK_ERROR("Fragment shader compilation failed.\n%s", infoLog);
			} break;
		}
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}
const GLuint ShaderData::compileProgram(const std::string& vertexSource, const std::string& fragmentSource) {
	const GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER),
				 fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		WR3CK_ERROR("Shader program linking failed.\n%s", infoLog);
		glDeleteProgram(program);
		program = 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
void ShaderData::loadAttributes() {
	m_attributes.clear();
	m_attributeInstanceTotalSize = 0;
	m_attributeStaticTotalSize = 0;

	GLint attributeCount, maxNameSize;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
	if (attributeCount <= 0)
		return;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameSize);
	maxNameSize += 1;

	char* nameBuffer = new char[maxNameSize];
	GLint nameLength, attributeSize, attributeLocation;
	GLenum attributeType;

	for (size_t i = 0; i < attributeCount; i++) {
		glGetActiveAttrib(
			m_program, i,
			maxNameSize, &nameLength,
			&attributeSize, &attributeType, nameBuffer
		);
		attributeLocation = glGetAttribLocation(m_program, nameBuffer);

		bool staticAttribute =
			nameLength >= 2 &&
			nameBuffer[0] == 'v' &&
			nameBuffer[1] == '_';

		m_attributes.emplace(
			std::string(nameBuffer),
			(Internal::ShaderAttributeInformation) {
				.m_dataSize = Internal::GL::getTypeByteSize(attributeType),
				.m_dataOffset = staticAttribute ? m_attributeStaticTotalSize : m_attributeInstanceTotalSize,
				.m_location = attributeLocation,
				.m_glType = attributeType,
				.m_isStatic = staticAttribute,
			}
		);

		const size_t dataSize = Internal::GL::getTypeByteSize(attributeType);
		if (staticAttribute) {
			m_attributeStaticTotalSize += dataSize;
		} else {
			m_attributeInstanceTotalSize += dataSize;
		}
	}

	delete[] nameBuffer;
}
void ShaderData::loadUniforms() {
	m_uniforms.clear();

	GLint uniformCount, maxNameSize;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);
	if (uniformCount <= 0)
		return;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
	maxNameSize += 1;

	char* nameBuffer = new char[maxNameSize];
	GLint nameLength, uniformSize, uniformLocation;
	GLenum uniformType;

	for (size_t i = 0; i < uniformCount; i++) {
		glGetActiveUniform(
			m_program, i,
			maxNameSize, &nameLength,
			&uniformSize, &uniformType, nameBuffer
		);
		uniformLocation = glGetUniformLocation(m_program, nameBuffer);

		m_uniforms.emplace(
			std::string(nameBuffer),
			(Internal::ShaderUniformInformation) {
				.m_location = uniformLocation,
				.m_glType = uniformType,
			}
		);
	}

	delete[] nameBuffer;
}
void ShaderData::compile(const std::string& source) {
	// TODO: Custom GLSL language support.
	const std::string splitDeliminator = "#pragma fragment";
	const size_t sourceSplitPosition = source.find(splitDeliminator);
	WR3CK_ASSERT(sourceSplitPosition != std::string::npos, "Could not find \"#pragma fragment\" in shader source.");

	m_program = compileProgram(
		source.substr(0, sourceSplitPosition),
		source.substr(sourceSplitPosition + splitDeliminator.length())
	);

	loadAttributes();
	loadUniforms();
}
}