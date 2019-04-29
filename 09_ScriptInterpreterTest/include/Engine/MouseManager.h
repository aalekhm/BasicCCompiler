#ifndef MOUSE_MANAGER_H
#define MOUSE_MANAGER_H

class MouseManager {

	public:
		MouseManager();
		~MouseManager();

		void onMouseDown(int mCode, int x, int y);
		void onMouseHover(int mCode, int x, int y);
		void onMouseMove(int mCode, int x, int y);
		void onMouseUp(int mCode, int x, int y);
		//void onMouseWheel(WPARAM wParam, LPARAM lParam);

		void	setLMouseStatus(bool bPressed, int mouseX, int mouseY);
		void	setMMouseStatus(bool bPressed, int mouseX, int mouseY);
		void	setRMouseStatus(bool bPressed, int mouseX, int mouseY);
		void	setMouseMove(unsigned int keyState, int mouseX, int mouseY);

		int		getMousePosX();
		int		getMousePosY();
	private:
		bool	m_bLMouseDown;
		bool	m_bMMouseDown;
		bool	m_bRMouseDown;

		int		m_iCode;
		int		m_iMouseX;
		int		m_iMouseY;
		int		m_iLastMouseX;
		int		m_iLastMouseY;

		unsigned int	m_iVirtualKeyCode;
};

#endif MOUSE_MANAGER_H