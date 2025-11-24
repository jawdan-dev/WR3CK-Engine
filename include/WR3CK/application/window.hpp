#pragma once
#include <WR3CK/core/core.hpp>

#include <WR3CK/application/time.hpp>
#include <WR3CK/application/input.hpp>

namespace WR3CK
{
class Window {
public:
	Window();
	Window(const char* title);
	Window(const char* title, const int width, const int height);
	Window(const Window& other) = delete;
	~Window();

	WR3CK_GETTER_CONST(int, width, m_width);
	WR3CK_GETTER_CONST(int, height, m_height);
	WR3CK_GETTER_CONST_MUT(GLFWwindow*, context, m_windowContext);
	WR3CK_GETTER_MUT(Time&, time, m_time);
	WR3CK_GETTER_MUT(Input&, input, m_input);
	const bool isOpen() const;

	// TODO: More window functions.
	void setSwapInterval(const int interval);

	virtual void process() = 0;

private:
	GLFWwindow* m_windowContext;
	Time m_time;
	Input m_input;

	int m_width, m_height;
};
}