#pragma once
#include "application.hpp"

namespace WR3CK
{
template<typename T, typename... Args, typename>
T* const Application::addWindow(Args&&... args) {
	T* const window = new T(args...);
	if (m_windows.size() > 0)
		window->setSwapInterval(0);
	m_windows.push_back(window);
	return window;
}
}