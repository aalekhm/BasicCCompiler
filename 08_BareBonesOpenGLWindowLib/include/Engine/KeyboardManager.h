#ifndef KEYBOARD_MANAGER_H
#define KEYBOARD_MANAGER_H

#define DEF_MAX_KEYS	256

class KeyboardManager {

	public:
		KeyboardManager();
		~KeyboardManager();

		static bool		isKeyPressed(int iKeyID);
		void				keyPressed(unsigned int iVirtualKeycode, unsigned short ch);
		void				keyReleased(unsigned int iVirtualKeycode, unsigned short ch);

	private:
		static bool					m_bKeys[DEF_MAX_KEYS];	// Array Used For The Keyboard Routine
		static unsigned int		m_iVirtualKeycode;
};

#endif