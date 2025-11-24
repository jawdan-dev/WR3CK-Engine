#pragma once
#include <WR3CK/core/core.hpp>

namespace WR3CK
{
class Input {
public:
	typedef int Key; // TODO: Custom key mapping.

	Input();
	Input(const Input& other) = delete;
	~Input() = default;

	const bool isKeyDown(const Key key) const;
	const bool isKeyPressed(const Key key) const;
	const bool isKeyUp(const Key key) const;
	WR3CK_GETTER_CONST(Vector2&, mousePosition, m_mousePosition);

	void registerKeyEvent(const Key key, const int action);
	void setMousePosition(const Vector2& mousePosition);
	void setMousePersistentPosition(const Vector2& mousePosition);

	void processInput();

private:
	std::set<Key> m_keyDown, m_keyPressed, m_keyUp;
	Vector2 m_mousePersistentPosition, m_mousePosition;
};
}