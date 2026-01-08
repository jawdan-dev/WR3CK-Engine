#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/asset/shader.hpp>
#include <WR3CK/renderer/renderInstance.hpp>

namespace WR3CK
{
class RenderUniformBuffer : public RenderInstance {
public:
	using RenderInstance::RenderInstance;

	void bindAll(const ShaderData& shader) const;
};
}