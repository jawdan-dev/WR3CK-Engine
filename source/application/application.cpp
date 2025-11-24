#include <WR3CK/application/application.hpp>

namespace WR3CK
{
Application::Application() : m_windows() {}
Application::~Application() {
	for (size_t i = 0; i < m_windows.size(); i++) {
		WR3CK_CLEANUP(m_windows[i], delete m_windows[i]);
	}
	m_windows.clear();
}

void Application::start() {
	while (m_windows.size() > 0) {
		// Window events.
		for (size_t i = 0; i < m_windows.size(); i++) {
			Window& window = *m_windows[i];
			window.input().processInput();
			window.time().updateTime(glfwGetTime());
		}
		glfwPollEvents();

		// Window close handling.
		for (size_t i = 0; i < m_windows.size(); i++) {
			if (m_windows[i]->isOpen())
				continue;

			WR3CK_CLEANUP(m_windows[i], delete m_windows[i]);
			m_windows.erase(m_windows.begin() + i);

			if (i == 0 && m_windows.size() > 0) {
				m_windows[0]->setSwapInterval(1);
			}

			i--;
		}

		// Process.
		for (size_t i = 0; i < m_windows.size(); i++) {
			Window& window = *m_windows[i];

			glfwMakeContextCurrent(window.context());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			window.process();
		}

		// Render.
		for (size_t i = m_windows.size() - 1; i < m_windows.size(); i--) {
			Window& window = *m_windows[i];
			glfwSwapBuffers(window.context());
		}
	}
}
}