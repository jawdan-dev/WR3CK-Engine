#pragma once
#include "handle.hpp"

namespace WR3CK
{
template<class T>
template<typename... Args, typename _T, typename>
Handle<T>::Handle(Args&&... args) :
	m_referenceCount(new size_t(0)), m_data(new T(args...)), m_referenced(false) {
	// TODO: Replace new allocator with some form of Arena perhaps?
	makeStrong();
}
template<class T>
Handle<T>::Handle(Handle<T>& other) : Handle(other, other.isWeak()) {}
template<class T>
Handle<T>::Handle(const Handle<T>& other) : Handle(other, other.isWeak()) {}
template<class T>
Handle<T>::Handle(Handle<T>& other, const bool weak) :
	m_referenceCount(other.m_referenceCount), m_data(other.m_data), m_referenced(false) {
	if (!weak) {
		makeStrong();
	}
}
template<class T>
Handle<T>::Handle(const Handle<T>& other, const bool weak) :
	m_referenceCount(other.m_referenceCount), m_data(other.m_data), m_referenced(false) {
	if (!weak) {
		makeStrong();
	}
}
template<class T>
Handle<T>::~Handle() {
	makeWeak();
}

template<class T>
T& Handle<T>::get() {
	WR3CK_ASSERT(m_referenced, "Cannot get() from a weak handle.");
	return *m_data;
}
template<class T>
const T& Handle<T>::get() const {
	WR3CK_ASSERT(m_referenced, "Cannot get() from a weak handle.");
	return *m_data;
}

template<class T>
void Handle<T>::makeStrong() {
	if (m_referenced)
		return;
	m_referenced = true;

	WR3CK_ASSERT(m_referenceCount != nullptr);
	WR3CK_ASSERT(m_data != nullptr);

	(*m_referenceCount)++;
}
template<class T>
void Handle<T>::makeWeak() {
	if (!m_referenced)
		return;
	m_referenced = false;

	WR3CK_ASSERT(m_referenceCount != nullptr);
	WR3CK_ASSERT(m_data != nullptr);

	(*m_referenceCount)--;

	if (*m_referenceCount > 0)
		return;

	WR3CK_CLEANUP(m_referenceCount, delete m_referenceCount);
	WR3CK_CLEANUP(m_data, delete m_data);
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
Handle<T>::operator bool() const {
	return hasData();
}
template<class T>
Handle<T>& Handle<T>::operator=(const Handle<T>& other) {
	makeWeak();
	m_referenceCount = other.m_referenceCount;
	m_data = other.m_data;
	if (!other.isWeak()) {
		makeStrong();
	}
	return *this;
}
template<class T>
const bool Handle<T>::operator==(const Handle<T>& other) const {
	return m_data == other.m_data;
}
template<class T>
const bool Handle<T>::operator<(const Handle<T>& other) const {
	return reinterpret_cast<intptr_t>(m_data) < reinterpret_cast<intptr_t>(other.m_data);
}
}