#pragma once
#include <WR3CK/application/window.hpp>

namespace WR3CK
{
class Application {
public:
	Application();
	Application(const Application& other) = delete;
	~Application();

	template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Window, T>>>
	T* const addWindow(Args&&... args);
	Window* const addWindow(Window* window);

	void start();

	static void initializeApplication(Application& app);

private:
	std::vector<Window*> m_windows;
};
}

#include "application.ipp"