#pragma once
#include "application.hpp"

namespace WR3CK
{
template<typename T, typename... Args, typename>
WindowHandle Application::addWindow(Args&&... args) {
	WindowHandle windowHandle = AssetHandle<T>(args...);
	return addWindow(windowHandle);
}
}