#include <WR3CK/renderer/staticRenderObject.hpp>

namespace WR3CK
{
StaticRenderObjectData::StaticRenderObjectData(const Internal::RenderGroupHandle& renderGroupHandle, const Internal::RenderGroupData::statickey_t staticKey) :
	m_renderGroupHandle(renderGroupHandle), m_staticKey(staticKey) {}
StaticRenderObjectData::~StaticRenderObjectData() {
	m_renderGroupHandle.get().removeStaticInstance(m_staticKey);
}
}