#pragma once

#include "Engine/EngineManager.h"

class Dream3DTest : EngineManager
{
	public:
								Dream3DTest();
		virtual					~Dream3DTest();

		virtual void			initialize();
		virtual void			update(float elapsedTime);
		virtual void			render(float elapsedTime);

		virtual void			keyPressedEx(unsigned int iVirtualKeycode, unsigned short ch);
		virtual void			keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch);

		virtual void			onMouseDownEx(int mCode, int x, int y);
		virtual void			onMouseMoveEx(int mCode, int x, int y);
		virtual void			onMouseUpEx(int mCode, int x, int y);
		virtual void			onMouseWheelEx(WPARAM wParam, LPARAM lParam);
	protected:
	private:
};

