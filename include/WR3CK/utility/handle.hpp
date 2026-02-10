#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK
{
template<typename T>
class Handle {
public:
	template<typename... Args, typename _T = T, typename = std::enable_if_t<(!std::is_abstract_v<_T>)>>
	Handle(Args&&... args);
	template<typename _T = T, typename = std::enable_if_t<(std::is_abstract_v<_T>)>>
	Handle() = delete;
	Handle(Handle<T>& other);
	Handle(const Handle<T>& other);
	Handle(Handle<T>& other, const bool weak);
	Handle(const Handle<T>& other, const bool weak);
	~Handle();

	T& get();
	const T& get() const;
	WR3CK_GETTER_CONST(size_t, referenceCount, *m_referenceCount);
	WR3CK_GETTER_CONST(bool, hasData, *m_referenceCount > 0);
	WR3CK_GETTER_CONST(bool, isStrong, m_referenced);
	WR3CK_GETTER_CONST(bool, isWeak, !isStrong());

	void makeStrong();
	void makeWeak();
	template<typename _T = T, typename = std::enable_if_t<(std::is_copy_constructible_v<_T>)>>
	void makeUnique();

	template<typename C, typename = std::enable_if_t<std::is_base_of_v<C, T>>>
	operator Handle<C>();
	template<typename C, typename = std::enable_if_t<std::is_base_of_v<C, T>>>
	operator Handle<C>() const;
	operator bool() const;
	Handle<T>& operator=(const Handle<T>& other);
	const bool operator==(const Handle<T>& other) const;
	const bool operator<(const Handle<T>& other) const;

private:
	size_t* m_referenceCount;
	T* m_data;
	bool m_referenced;
};
}

#include "handle.ipp"