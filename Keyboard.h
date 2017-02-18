#pragma once
#include <vector>
class Keyboard
{
public:
	static void setKeyDown(short keycode);
	static void setKeyUp(short keycode);
	static bool checkKeyDown(short keycode);
private:
	static std::vector<short> m_keysDown;
};

