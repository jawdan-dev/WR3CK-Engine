#include <WR3CK/application/window.hpp>

namespace WR3CK
{
WindowData::WindowData() : WindowData("WR3CK Engine") {}
WindowData::WindowData(const char* title) : WindowData(title, 640, 360) {}
WindowData::WindowData(const char* title, const int width, const int height) :
	m_windowContext(nullptr) {
	glfwSetErrorCallback([](int errorCode, const char* description) {
		WR3CK_ERROR("GLFW Error %i: %s", errorCode, description);
	});

	WR3CK_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_windowContext = glfwCreateWindow(width, height, title, NULL, NULL);
	WR3CK_ASSERT(m_windowContext != nullptr, "Failed to initialize GLFW window context.");

	m_width = width;
	m_height = height;
	glfwMakeContextCurrent(m_windowContext);
	glfwSetWindowUserPointer(m_windowContext, this);

	glewExperimental = GL_TRUE;
	WR3CK_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");

	glfwSwapInterval(1);
	glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glfwSetWindowSizeCallback(m_windowContext, [](GLFWwindow* windowHandle, int width, int height) {
		WindowData& window = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
		glViewport(0, 0, width, height);
		window.m_width = width;
		window.m_height = height;
	});
	glfwSetKeyCallback(m_windowContext, [](GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
		WindowData& window = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
		window.m_input.registerKeyEvent(key, action);
	});
	glfwSetMouseButtonCallback(m_windowContext, [](GLFWwindow* windowHandle, int button, int action, int mods) {
		WindowData& window = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
		window.m_input.registerKeyEvent(button, action);
	});
	glfwSetCursorPosCallback(m_windowContext, [](GLFWwindow* windowHandle, double mouseX, double mouseY) {
		WindowData& window = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
		window.m_input.setMousePersistentPosition(Vector2(mouseX, mouseY));
	});

	glfwShowWindow(m_windowContext);
	glfwDefaultWindowHints();
}
WindowData::~WindowData() {
	WR3CK_CLEANUP(m_windowContext, glfwDestroyWindow(m_windowContext))
}

const GLuint WindowData::frameBuffer() const {
	return 0;
}

const bool WindowData::isOpen() const {
	return !glfwWindowShouldClose(m_windowContext);
}

void WindowData::setSwapInterval(const int interval) {
	glfwMakeContextCurrent(m_windowContext);
	glfwSwapInterval(interval);
}
}
