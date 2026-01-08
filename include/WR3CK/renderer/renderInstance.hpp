#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/renderer/renderData.hpp>

namespace WR3CK
{
class RenderInstance {
public:
	RenderInstance();
	RenderInstance(const RenderInstance& other) = default;
	~RenderInstance() = default;

	WR3CK_GETTER_CONST(ARG(std::map<std::string, Internal::RenderData>&), data, m_data);

	template<typename T>
	void setData(const std::string& attributeName, const T& data);

	const bool operator<(const RenderInstance& other) const;

private:
	std::map<std::string, Internal::RenderData> m_data;
};
}

#include "renderInstance.ipp"