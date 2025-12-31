#pragma once
#include "handle.hpp"

namespace WR3CK
{
template<class T>
template<typename... Args>
Handle<T>::Handle(const Args&&... args) :
	m_referenceCount(new size_t(0)), m_data(new T(args...)) {
	// TODO: Replace new allocator with some form of Arena perhaps?
	referenceIncrement();
}
template<class T>
Handle<T>::Handle(const Handle<T>& other) :
	m_referenceCount(other.m_referenceCount), m_data(other.m_data) {
	referenceIncrement();
}
template<class T>
Handle<T>::~Handle() {
	referenceDecrement();
}

template<class T>
template<typename C, typename>
Handle<T>::operator Handle<C>() {
	return *reinterpret_cast<Handle<C>*>(this);
}
template<class T>
template<typename C, typename>
Handle<T>::operator Handle<C>() const {
	return *reinterpret_cast<const Handle<C>*>(this);
}

template<class T>
void Handle<T>::referenceIncrement() {
	WR3CK_ASSERT(m_referenceCount != nullptr);
	WR3CK_ASSERT(m_data != nullptr);

	(*m_referenceCount)++;
}
template<class T>
void Handle<T>::referenceDecrement() {
	WR3CK_ASSERT(m_referenceCount != nullptr);
	WR3CK_ASSERT(m_data != nullptr);

	(*m_referenceCount)--;

	if (*m_referenceCount > 0)
		return;
	delete m_referenceCount;
	delete m_data;
}
}