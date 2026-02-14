#include <WR3CK/application/application.hpp>

namespace WR3CK
{
Application::Application() : m_windowHandles() {}

WindowHandle Application::addWindow(WindowHandle& windowHandle) {
	WindowData& window = windowHandle.get();

	if (m_windowHandles.size() > 0)
		window.setSwapInterval(0);

	m_windowHandles.push_back(windowHandle);
	return windowHandle;
}

void Application::start() {
	while (m_windowHandles.size() > 0) {
		// Window events.
		for (size_t i = 0; i < m_windowHandles.size(); i++) {
			WindowData& window = m_windowHandles[i].get();
			window.input().processInput();
			window.time().updateTime(glfwGetTime());
		}
		glfwPollEvents();

		// Window close handling.
		for (size_t i = 0; i < m_windowHandles.size(); i++) {
			WindowData& window = m_windowHandles[i].get();

			if (window.isOpen())
				continue;

			m_windowHandles.erase(m_windowHandles.begin() + i);

			if (i == 0 && m_windowHandles.size() > 0) {
				m_windowHandles[0].get().setSwapInterval(1);
			}

			i--;
		}

		// Process.
		for (size_t i = 0; i < m_windowHandles.size(); i++) {
			WindowData& window = m_windowHandles[i].get();
			glFinish();
			glfwMakeContextCurrent(window.context());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			window.process();
		}

		// Render.
		for (size_t i = m_windowHandles.size() - 1; i < m_windowHandles.size(); i--) {
			WindowData& window = m_windowHandles[i].get();
			glfwSwapBuffers(window.context());
		}
	}
}
}