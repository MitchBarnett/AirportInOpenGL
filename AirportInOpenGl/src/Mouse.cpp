#include "stdafx.h"
#include "Mouse.h"


POINT Mouse::getPosition()
{
	POINT point;
	GetCursorPos(&point);	// Windows function.
	return point;
}

POINT Mouse::getPosition(HWND & window)
{
	POINT point;
	GetCursorPos(&point);	// Windows function returns global coords.
	ScreenToClient(window, &point); // Windows function translate gloabl to local coords.
	return point;
}

void Mouse::setPosition(const POINT & position)
{
	SetCursorPos(position.x, position.y);	// Windows function.
}

void Mouse::setPosition(const POINT & position, const HWND & window)
{
	POINT point = { position.x, position.y };
	ClientToScreen(window, &point);	// // Windows function translates a local point to global coords.
	SetCursorPos(point.x, point.y);	// Windows function takes gloabl coords.
}
