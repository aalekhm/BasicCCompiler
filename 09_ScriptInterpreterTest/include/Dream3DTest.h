#pragma once

#include "Engine/EngineManager.h"
#include "VirtualMachine.h"

class Dream3DTest : public EngineManager
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

		int32_t					m_iMouseX;
		int32_t					m_iMouseY;
protected:
	private:
		VirtualMachine*			m_pVM;
		void					onScriptCallback(const char* sSysFuncName, int16_t iArgCount);
};

