#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK
{
template<typename T>
class Handle {
public:
	template<typename... Args>
	Handle(Args&&... args);
	Handle(Handle<T>& other);
	Handle(const Handle<T>& other);
	~Handle();

	WR3CK_GETTER_MUT(T&, get, *m_data);
	WR3CK_GETTER_CONST(size_t, referenceCount, *m_referenceCount);

	template<typename C, typename = std::enable_if_t<std::is_base_of_v<C, T>>>
	operator Handle<C>();
	template<typename C, typename = std::enable_if_t<std::is_base_of_v<C, T>>>
	operator Handle<C>() const;
	Handle<T>& operator=(const Handle<T>& other);
	const bool operator==(const Handle<T>& other) const;
	const bool operator<(const Handle<T>& other) const;

private:
	void referenceIncrement();
	void referenceDecrement();

	size_t* m_referenceCount;
	T* m_data;
};
}

#include "handle.ipp"