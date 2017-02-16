#pragma once
#include <Windows.h>
class Mouse
{
public:
	Mouse();
	~Mouse();
	static POINT getPosition();
	static POINT getPosition(HWND& window);
	static void setPosition(const POINT& position);
	static void setPosition(const POINT& position, const HWND& window);


};

