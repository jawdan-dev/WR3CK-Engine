#pragma once
#include <WR3CK/asset/assetHandle.hpp>

namespace WR3CK
{
class FrameBufferData : public AssetBaseData {
public:
	FrameBufferData();
	FrameBufferData(const Color& clearColor);
	FrameBufferData(const FrameBufferData& other) = delete;
	virtual ~FrameBufferData() = default;

	virtual const GLuint frameBuffer() const = 0;
	WR3CK_GETTER_CONST(Color&, clearColor, m_clearColor);

	void setClearColor(const Color& clearColor);

	virtual void clear();

private:
	Color m_clearColor;
};
typedef AssetHandle<FrameBufferData> FrameBufferHandle;
}