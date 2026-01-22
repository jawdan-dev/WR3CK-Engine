#include <WR3CK/renderer/renderUniformBuffer.hpp>

namespace WR3CK
{
void RenderUniformBuffer::bindAll(const ShaderData& shader) const {
	for (auto it : data()) {
		const auto uniformIt = shader.uniforms().find(it.first);
		if (uniformIt == shader.uniforms().end())
			continue;

		const Internal::ShaderUniformInformation& uniformData = uniformIt->second;
		if (it.second.glType() != uniformData.m_glType)
			continue;

		const void* const data = it.second.data();
		switch (it.second.dataType()) {
			case Internal::RenderData::DataType::Int:
				glUniform1i(uniformData.m_location, *(int*)data);
				break;
			case Internal::RenderData::DataType::UnsignedInt:
				glUniform1i(uniformData.m_location, *(unsigned int*)data);
				break;
			case Internal::RenderData::DataType::Float:
				glUniform1f(uniformData.m_location, *(float*)data);
				break;
			case Internal::RenderData::DataType::Vector2:
				glUniform2f(uniformData.m_location, ((Vector2*)data)->x(), ((Vector2*)data)->y());
				break;
			case Internal::RenderData::DataType::Vector3:
				glUniform3f(uniformData.m_location, ((Vector3*)data)->x(), ((Vector3*)data)->y(), ((Vector3*)data)->z());
				break;
			case Internal::RenderData::DataType::Mat4:
				glUniformMatrix4fv(uniformData.m_location, 1, GL_FALSE, ((Matrix4*)data)->data());
				break;

			case Internal::RenderData::DataType::Texture: {
				const TextureHandle& textureHandle = *reinterpret_cast<const TextureHandle*>(data);
				const GLuint64 glTextureHandle = textureHandle.get().handle();
				glUniformHandleui64ARB(uniformData.m_location, glTextureHandle);
			} break;
			case Internal::RenderData::DataType::RenderTexture: {
				const RenderTextureHandle& renderTextureHandle = *reinterpret_cast<const RenderTextureHandle*>(data);
				const GLuint64 glTextureHandle = renderTextureHandle.get().handle();
				glUniformHandleui64ARB(uniformData.m_location, glTextureHandle);
			} break;

			default:
				WR3CK_ERROR("Unsupported RenderData uniform data type \"0x%x\".", it.second.dataType());
				break;
		}
	}
}
}