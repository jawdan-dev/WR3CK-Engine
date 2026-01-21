#pragma once
#include <WR3CK/asset/assetHandle.hpp>

#include "shaderAttributeInformation.hpp"
#include "shaderUniformInformation.hpp"

namespace WR3CK
{
class ShaderData : public AssetBaseData {
public:
	ShaderData(const std::string& source);
	ShaderData(const ShaderData& other) = delete;
	~ShaderData();

	WR3CK_GETTER_CONST(ARG(std::map<std::string, Internal::ShaderUniformInformation>&), uniforms, m_uniforms);
	WR3CK_GETTER_CONST(ARG(std::map<std::string, Internal::ShaderAttributeInformation>&), attributes, m_attributes);
	WR3CK_GETTER_CONST(size_t, attributeInstanceTotalSize, m_attributeInstanceTotalSize);
	WR3CK_GETTER_CONST(size_t, attributeStaticTotalSize, m_attributeStaticTotalSize);
	WR3CK_GETTER_CONST(GLuint, program, m_program);

private:
	const GLuint compileShader(const std::string& source, const GLenum shaderType);
	const GLuint compileProgram(const std::string& vertexSource, const std::string& fragmentSource);
	void loadAttributes();
	void loadUniforms();
	void compile(const std::string& source);

	std::map<std::string, Internal::ShaderUniformInformation> m_uniforms;
	std::map<std::string, Internal::ShaderAttributeInformation> m_attributes;
	size_t m_attributeInstanceTotalSize, m_attributeStaticTotalSize;
	GLuint m_program;
};
typedef AssetHandle<ShaderData> ShaderHandle;
}