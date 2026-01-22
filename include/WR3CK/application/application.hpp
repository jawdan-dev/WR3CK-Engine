#pragma once
#include <WR3CK/application/window.hpp>

namespace WR3CK
{
class Application {
public:
	Application();
	Application(const Application& other) = delete;
	~Application() = default;

	template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<WindowData, T>>>
	WindowHandle addWindow(Args&&... args);
	WindowHandle addWindow(WindowHandle& windowHandle);

	void start();

	static void initializeApplication(Application& app);

private:
	std::vector<WindowHandle> m_windowHandles;
};
}

#include "application.ipp"