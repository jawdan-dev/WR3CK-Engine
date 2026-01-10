#include <WR3CK/renderer/staticRenderObject.hpp>

namespace WR3CK
{
StaticRenderObjectData::StaticRenderObjectData(const Internal::RenderGroup& renderGroup, const Internal::RenderGroupData::statickey_t staticKey) :
	m_renderGroup(renderGroup), m_staticKey(staticKey) {}
StaticRenderObjectData::~StaticRenderObjectData() {
	m_renderGroup.get().removeStaticInstance(m_staticKey);
}
}