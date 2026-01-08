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
		switch (it.second.glType()) {
			case GL_FLOAT:
				glUniform1f(uniformData.m_location, *(float*)data);
				break;
			case GL_FLOAT_VEC2:
				glUniform2f(uniformData.m_location, ((Vector2*)data)->x(), ((Vector2*)data)->y());
				break;
			case GL_FLOAT_VEC3:
				glUniform3f(uniformData.m_location, ((Vector3*)data)->x(), ((Vector3*)data)->y(), ((Vector3*)data)->z());
				break;
			case GL_FLOAT_MAT4:
				glUniformMatrix4fv(uniformData.m_location, 1, GL_FALSE, ((Matrix4*)data)->data());
				break;

			case GL_SAMPLER_2D: {
				const Texture* const textureHandle = reinterpret_cast<const Texture*>(data);
				const GLuint64 handle = textureHandle->get().handle();
				glUniformHandleui64ARB(uniformData.m_location, handle);
			} break;

			default:
				WR3CK_ERROR("Unsupported uniform type");
				break;
		}
	}
}
}