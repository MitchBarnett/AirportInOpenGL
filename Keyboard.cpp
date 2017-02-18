#include "stdafx.h"
#include "Keyboard.h"
#include <algorithm>

std::vector<short> Keyboard::m_keysDown;
void Keyboard::setKeyDown(short keycode)
{
	if (std::find(m_keysDown.begin(), m_keysDown.end(), keycode) == m_keysDown.end())
	{
		m_keysDown.push_back(keycode);
	}
}

void Keyboard::setKeyUp(short keycode)
{
	m_keysDown.erase(std::remove(m_keysDown.begin(), m_keysDown.end(), keycode), m_keysDown.end());
}

bool Keyboard::checkKeyDown(short keycode)
{
	if (std::find(m_keysDown.begin(), m_keysDown.end(), keycode) != m_keysDown.end())
	{
		return true;
	}
	return false;
}
