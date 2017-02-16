#include "stdafx.h"
#include "Mouse.h"


POINT Mouse::getPosition()
{
	POINT point;
	GetCursorPos(&point);
	return point;
}

POINT Mouse::getPosition(HWND & window)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window, &point);
	return point;
}

void Mouse::setPosition(const POINT & position)
{
	SetCursorPos(position.x, position.y);
}

void Mouse::setPosition(const POINT & position, const HWND & window)
{
	POINT point = { position.x, position.y };
	ClientToScreen(window, &point);
	SetCursorPos(point.x, point.y);
}
