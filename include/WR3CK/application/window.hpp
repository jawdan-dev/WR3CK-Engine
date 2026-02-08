#pragma once
#include <WR3CK/core/core.hpp>
#include <WR3CK/core/opengl.hpp>

#include <WR3CK/application/time.hpp>
#include <WR3CK/application/input.hpp>
#include <WR3CK/asset/frameBuffer.hpp>

namespace WR3CK
{
class WindowData : public FrameBufferData {
public:
	struct CreationConfig {
		std::string m_title = "WR3CK Engine";
		int m_width = 640,
			m_height = 360;
	};

	WindowData();
	WindowData(const CreationConfig& config);
	WindowData(const WindowData& other) = delete;
	virtual ~WindowData();

	WR3CK_GETTER_CONST(int, width, m_width);
	WR3CK_GETTER_CONST(int, height, m_height);
	WR3CK_GETTER_CONST_MUT(GLFWwindow*, context, m_windowContext);
	virtual const GLuint frameBuffer() const override;
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
typedef AssetHandle<WindowData> WindowHandle;
}