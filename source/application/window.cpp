#include <WR3CK/application/window.hpp>

namespace WR3CK
{
std::set<GLFWwindow*> s_contextShareParents;

WindowData::WindowData() : WindowData(CreationConfig()) {}
WindowData::WindowData(const CreationConfig& config) :
	m_windowContext(nullptr),
	m_time(), m_input(),
	m_width(0), m_height(0) {
	glfwSetErrorCallback([](int errorCode, const char* description) {
		WR3CK_ERROR("GLFW Error %i: %s", errorCode, description);
	});

	WR3CK_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* contextShareParent = nullptr;
	if (!s_contextShareParents.empty())
		contextShareParent = *s_contextShareParents.begin();

	m_windowContext = glfwCreateWindow(config.m_width, config.m_height, config.m_title.c_str(), NULL, contextShareParent);
	WR3CK_ASSERT(m_windowContext != nullptr, "Failed to initialize GLFW window context.");
	s_contextShareParents.emplace(m_windowContext);

	m_width = config.m_width;
	m_height = config.m_width;
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
	s_contextShareParents.erase(m_windowContext);

	WR3CK_CLEANUP(m_windowContext, glfwDestroyWindow(m_windowContext));
	m_width = 0;
	m_height = 0;
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
