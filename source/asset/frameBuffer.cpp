#include <WR3CK/asset/frameBuffer.hpp>

namespace WR3CK
{
FrameBufferData::FrameBufferData() : FrameBufferData(Color()) {}
FrameBufferData::FrameBufferData(const Color& clearColor) : m_clearColor(clearColor), AssetBaseData() {}

void FrameBufferData::setClearColor(const Color& clearColor) {
	m_clearColor = clearColor;
}

void FrameBufferData::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer());
	glClearColor(m_clearColor.r(), m_clearColor.g(), m_clearColor.b(), m_clearColor.a());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
}