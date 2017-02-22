#pragma once
#include <Windows.h>

/*!
Give access to the real-time state of the mouse.

Provides access to the position of the mouse.
Only contains static functions as there should only be a single mouse (should not be instantiated).
*/
class Mouse
{
public:
	/*!
	Gets the position of the mouse in global coordinates.

	This function returns the global position of the mouse cursor on the desktop.
	\return Current position of the mouse in global coordinates.
	*/
	static POINT getPosition();

	/*!
	Gets the position of the mouse in window coordinates.

	This function returns the position of the mouse relative to the
	top left of the given window.
	\param window The handle of the window to use as a reference.
	\return Current position of the mouse in window coordinates.
	*/
	static POINT getPosition(HWND& window);

	/*!
	Sets the cursor to the given global coordinates.

	This function sets the global position of the  mouse cursor on the screen.
	\param position The position to set the cursor to.
	*/
	static void setPosition(const POINT& position);

	/*!
	Sets the cursor position relitive to a window.

	Sets the cursor to the given position relative to the top left of the
	given window.
	This function sets the global position of the  mouse cursor on the screen.
	\param position The position to set the cursor to.
	\param window The handle of the window to use as a reference.
	*/
	static void setPosition(const POINT& position, const HWND& window);


};

