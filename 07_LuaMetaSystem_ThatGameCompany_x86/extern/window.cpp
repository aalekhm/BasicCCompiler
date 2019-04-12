#include "extern/window.h"
#include <iostream>
#include <vector>


	Window::Window(const char* _name, int32_t _width, int32_t _height, int32_t _xpos, int32_t _ypos, uint32_t _options)
		: m_inputHandler(new InputHandler())
		, m_initialized(false)
		, m_active(false){
		TCHAR* windowName = 0;
		m_windowPlacement.length = sizeof(WINDOWPLACEMENT);

		windowName = (TCHAR*)_strdup(_name);
		WNDCLASSEX windowClass = {
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			(WNDPROC)WndProc,
			0, 0,
			GetModuleHandle(nullptr),
			LoadIcon(NULL, IDI_WINLOGO),
			LoadCursor(0, IDC_ARROW),
			nullptr,
			nullptr,
			windowName
		};

		if (!RegisterClassEx(&windowClass)){
			std::cerr << "Failed to register Window class. RegisterClassEx returned FALSE\n";
			return;
		}

		auto btst = [](uint32_t _reg, uint32_t _mask) { return (_reg & _mask) != 0; };

		int32_t multisample = (btst(_options, MULTISAMPLE2) ? 2 :
			(btst(_options, MULTISAMPLE4) ? 4 :
			(btst(_options, MULTISAMPLE8) ? 8 :
			(btst(_options, MULTISAMPLE16) ? 16 :
			(btst(_options, MULTISAMPLE32) ? 32 : 0)))));

		const int32_t rgbbits = (btst(_options, RGB16) ? 16 :
			(btst(_options, RGB24) ? 24 :
			(btst(_options, RGB32) ? 32 :
			(btst(_options, RGB64) ? 64 : 32))));

		const int32_t alphabits = (btst(_options, ALPHA4) ? 4 :
			(btst(_options, ALPHA8) ? 8 :
			(btst(_options, ALPHA12) ? 12 :
			(btst(_options, ALPHA16) ? 16 : 0))));

		const int32_t depthbits = (btst(_options, DEPTH8) ? 8 :
			(btst(_options, DEPTH16) ? 16 :
			(btst(_options, DEPTH24) ? 24 :
			(btst(_options, DEPTH32) ? 32 : 24))));

		const int32_t stencilbits = (btst(_options, STENCIL4) ? 4 :
			(btst(_options, STENCIL8) ? 8 :
			(btst(_options, STENCIL16) ? 16 : 8)));

		if (!createWindow(windowName, windowName, WS_OVERLAPPEDWINDOW, _xpos, _ypos, _width, _height)){
			std::cerr << "Failed to create Window CreateWindowEx returned FALSE\n";
			return;
		}
		m_active = true;

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
			1,                     // version number  
			PFD_DRAW_TO_WINDOW |   // support window  
			PFD_SUPPORT_OPENGL |   // support OpenGL  
			(btst(_options, DOUBLE_BUFFER) ? PFD_DOUBLEBUFFER : 0) |   // double buffered
			PFD_GENERIC_ACCELERATED,
			PFD_TYPE_RGBA,         // RGBA type  
			rgbbits,               // 24-bit color depth  
			0, 0, 0, 0, 0, 0,      // color bits ignored  
			alphabits,             // no alpha buffer  
			0,                     // shift bit ignored  
			0,                     // no accumulation buffer  
			0, 0, 0, 0,            // accum bits ignored  
			depthbits,             // 32-bit z-buffer      
			stencilbits,           // no stencil buffer  
			0,                     // no auxiliary buffer  
			PFD_MAIN_PLANE,        // main layer  
			0,                     // reserved  
			0, 0, 0                // layer masks ignored  
		};

		int32_t nPixelFormat = ChoosePixelFormat(m_hdc, &pfd);
		if (nPixelFormat == 0 || SetPixelFormat(m_hdc, nPixelFormat, &pfd) <= 0){
			GetLastError();
			std::cerr << "Error " << GetLastError() << ": Cannot find hardware accelerated OpenGL pixel format \n";
			return;
		}

		int32_t majorVersion = -1, minorVersion = -1;

		HGLRC dummyContext = wglCreateContext(m_hdc);
		if (dummyContext == 0){
			std::cerr << "Cannot create basic OpenGL context\n";
			return;
		}

		wglMakeCurrent(m_hdc, dummyContext);

		const GLubyte*(GL_FUNCPTR*_tempglGetString)(GLenum) = (const GLubyte*(GL_FUNCPTR*)(GLenum))GetGLProcAddress("glGetString");
		const GLubyte* str = _tempglGetString(GL_VERSION);
		sscanf_s((char*)str, "%d.%d", &majorVersion, &minorVersion);

		int32_t fails = LoadOpenGLFunctions(btst(_options, CORE_PROFILE));


		//DeleteDC(m_hdc);
		//DestroyWindow(m_hwnd);
		if (!createWindow(windowName, windowName, WS_OVERLAPPEDWINDOW, _xpos, _ypos, _width, _height)){
			std::cerr << "Cannot re-create the Window.\n";
			return;
		}

		//Check Extensions

		typedef BOOL(WINAPI *FPCPFARB)(HDC, const int32_t*, const float*, uint32_t, int32_t*, uint32_t*);
		typedef BOOL(WINAPI *FPGPFARB)(HDC, int32_t, int32_t, uint32_t, const int32_t*, int32_t*);

		FPCPFARB wglChoosePixelFormatARB = (FPCPFARB)wglGetProcAddress("wglChoosePixelFormatARB");
		FPGPFARB wglGetPixelFormatAttribivARB = (FPGPFARB)wglGetProcAddress("wglGetPixelFormatAttribivARB");

		int32_t pixelFormatAttribList[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_DOUBLE_BUFFER_ARB, btst(_options, DOUBLE_BUFFER),
			WGL_STEREO_ARB, btst(_options, STEREO),
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, rgbbits,
			WGL_DEPTH_BITS_ARB, depthbits,
			WGL_STENCIL_BITS_ARB, stencilbits,
			WGL_SAMPLE_BUFFERS_ARB, ((multisample > 0) ? 1 : 0),
			WGL_SAMPLES_ARB, multisample,
			0
		};

		const int32_t pixelFormatAttribListSize = sizeof(pixelFormatAttribList) / sizeof(int32_t);
		int32_t pixelFormats[255];
		UINT numFormats = 0;

		wglChoosePixelFormatARB(m_hdc, pixelFormatAttribList, nullptr, 255, &pixelFormats[0], &numFormats);
		while (numFormats <= 0 && multisample > 0){
			multisample >>= 1;
			pixelFormatAttribList[pixelFormatAttribListSize - 2] = multisample;
			pixelFormatAttribList[pixelFormatAttribListSize - 4] = (multisample > 0 ? 1 : 0);
			std::cout << "Unable to set pixel format. Trying: MSAAx" << multisample << "...";
			wglChoosePixelFormatARB(m_hdc, pixelFormatAttribList, nullptr, 255, &pixelFormats[0], &numFormats);
			std::cout << (numFormats <= 0 ? "failed\n" : "ok\n");
		}

		if (numFormats <= 0){
			std::cout << "Cannot find pixel format \n";
			return;
		}


		PIXELFORMATDESCRIPTOR dummy;
		memset(&dummy, 0, sizeof(PIXELFORMATDESCRIPTOR));
		dummy.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		if (SetPixelFormat(m_hdc, pixelFormats[0], &dummy) <= 0){
			std::cout << "Cannot set the desired pixel format \n " << GetLastError() << std::endl;
			return;
		}


		if (multisample){
			glEnable(GL_MULTISAMPLE);
		}


		ShowWindow(m_hwnd, SW_SHOW);

		/*if (!CheckWGLExtension("WGL_ARB_create_context")){

		}*/

		typedef HGLRC(WINAPI * FPCCAARB)(HDC, HGLRC, const int32_t*);
		FPCCAARB wglCreateContextAttribsARB = (FPCCAARB)wglGetProcAddress("wglCreateContextAttribsARB");


		std::vector<int32_t> attribs;
		attribs.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
		attribs.push_back(majorVersion);
		attribs.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
		attribs.push_back(minorVersion);
		if (btst(_options, DEBUG_CONTEXT)){
			attribs.push_back(WGL_CONTEXT_FLAGS_ARB);
			attribs.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
		}
		if (btst(_options, CORE_PROFILE)){
			//if (CheckWGLExtension("WGL_ARB_create_context_profile")){
			    attribs.push_back(WGL_CONTEXT_FLAGS_ARB);
			    attribs.push_back(WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);
			    attribs.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
			    attribs.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
			//}
		}
		else{
			attribs.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
			attribs.push_back(WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
		}
		attribs.push_back(0);

		HGLRC hrc = wglCreateContextAttribsARB(m_hdc, nullptr, &attribs[0]);
		if (hrc <= 0){
			std::cerr << "Cannot create OpenGL context.. \n";
			return;
		}

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(dummyContext);
		wglMakeCurrent(m_hdc, hrc);

		if (fails == 0){
			std::cout << "OpenGL Context successfully initialized. Version " << majorVersion << "." << minorVersion << std::endl;
		}
		else{
			std::cout << "OpenGL Context Created, but failed to load " << fails << " functions. Version " << majorVersion << "." << minorVersion << std::endl;
		}
		//list of OpenGL Extensions
		static const char * (WINAPI *wglGetExtensionsStringARB)(HDC) = NULL;
		wglGetExtensionsStringARB = (const char * (WINAPI *)(HDC))GetGLProcAddress("wglGetExtensionsStringARB");
		std::cout << wglGetExtensionsStringARB(GetDC(GetDesktopWindow())) << std::endl;

		typedef BOOL(WINAPI* FPWGLSIEXT)(int);
		static FPWGLSIEXT wglSwapIntervalEXT = (FPWGLSIEXT)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT){
			if (btst(_options, FRAME_SYNC))
				wglSwapIntervalEXT(1);
			else if (btst(_options, ADAPTIVE_FRAME_SYNC))
				wglSwapIntervalEXT(-1);
			else
				wglSwapIntervalEXT(0);
		}
		else{
			std::cout << "WARNING: Cannot set the FrameSync option. Using default Driver settings. \n";
		}


		SetWindowLong(m_hwnd, GWLP_USERDATA, (long)this);
		m_initialized = true;

//        setFullscreen(btst(_options, FULLSCREEN));
		delete[] windowName;
	}

	Window::~Window(){
		DeleteDC(m_hdc);
		DestroyWindow(m_hwnd);

		HGLRC hglrc = wglGetCurrentContext();
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(hglrc);
	}

	bool Window::processAllMessages() const{
		Sleep(0);

		if (m_active){
			MSG msg;
			PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return active();
	}

	void Window::windowResolution(int& _width, int& _height) const{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		_width = rect.right;
		_height = rect.bottom;
	}

	void Window::desktopResolution(int& _width, int& _height) const{
		RECT desk;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desk);
		_width = desk.right;
		_height = desk.bottom;
	}

	bool Window::isFullscreen() const{
		return (GetWindowLong(m_hwnd, GWL_STYLE) & WS_OVERLAPPEDWINDOW);
	}

	bool Window::setFullscreen(bool _fullscreen){
		if (isFullscreen() == _fullscreen)
			return 1;

		DWORD style = GetWindowLong(m_hwnd, GWL_STYLE);
		if (_fullscreen){
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(m_hwnd, &m_windowPlacement) && GetMonitorInfo(MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)){
				SetWindowLong(m_hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(m_hwnd, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				return 1;
			}
			return 0;
		}
		else{
			SetWindowLong(m_hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(m_hwnd, &m_windowPlacement);
			SetWindowPos(m_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		return 1;
	}

	bool Window::createWindow(TCHAR* _caption, TCHAR* _classname, ULONG _style, int32_t _x, int32_t _y, int32_t _width, int32_t _height){
		const DWORD style = _style;
		const DWORD styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		RECT cr;
		SetRect(&cr, _x, _y, _x + _width, _y + _height);
		AdjustWindowRectEx(&cr, style, FALSE, styleEx);
		m_hwnd = CreateWindowEx(styleEx, _classname, _caption, style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

		if (m_hwnd <= 0){
			return 0;
		}

		m_hdc = GetDC(m_hwnd);
		ShowWindow(m_hwnd, SW_HIDE);
		UpdateWindow(m_hwnd);
		return 1;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
		BYTE ks[256];
		WORD characters[3];
		Window* w = (Window*)GetWindowLong(hWnd, GWLP_USERDATA);
		switch (message){
		case WM_KILLFOCUS:
		{
		/*	if (w->isFullscreen()){
				w->setMinimized(true);
			}*/
		}break;
		/*case WM_SYSCOMMAND:
			switch (wParam){
			case SC_SIZE:
			break;
			}break;*/
		case WM_SIZING:
		{
		}break;
		case WM_SIZE:
			break;
		case WM_WINDOWPOSCHANGING:
		{
		}break;
		case WM_CLOSE:
		{
			w->deactivate();
		}break;
		case WM_DESTROY:
		{	w->deactivate();
		PostQuitMessage(0);
		}break;
		case WM_KEYDOWN:
		{
			GetKeyboardState(ks);
			memset(characters, 0, sizeof(characters));
			ToAscii((UINT)wParam, MapVirtualKey((UINT)wParam, 0), ks, characters, 0);
			char c = (char)characters[0];
			w->inputHandler()->injectKeyboardButton(c, InputHandler::PRESSED);
		}break;
		case WM_KEYUP:
		{
			GetKeyboardState(ks);
			memset(characters, 0, sizeof(characters));
			ToAscii((UINT)wParam, MapVirtualKey((UINT)wParam, 0), ks, characters, 0);
			char c = (char)characters[0];
			w->inputHandler()->injectKeyboardButton(c, InputHandler::RELEASED);
			break;
		}
		case WM_LBUTTONDOWN:
		{	w->inputHandler()->injectMouseButton(InputHandler::LEFT_MOUSE_BUTTON, InputHandler::PRESSED);
		}break;
		case WM_LBUTTONUP:
		{	w->inputHandler()->injectMouseButton(InputHandler::LEFT_MOUSE_BUTTON, InputHandler::RELEASED);
		}break;
		case WM_RBUTTONDOWN:
		{	w->inputHandler()->injectMouseButton(InputHandler::RIGHT_MOUSE_BUTTON, InputHandler::PRESSED);
		}break;
		case WM_RBUTTONUP:
		{	w->inputHandler()->injectMouseButton(InputHandler::RIGHT_MOUSE_BUTTON, InputHandler::RELEASED);
		}break;
		case WM_MBUTTONDOWN:
		{	w->inputHandler()->injectMouseButton(InputHandler::MIDDLE_MOUSE_BUTTON, InputHandler::PRESSED);
		}break;
		case WM_MBUTTONUP:
		{	w->inputHandler()->injectMouseButton(InputHandler::MIDDLE_MOUSE_BUTTON, InputHandler::RELEASED);
		}break;
		case WM_MOUSEMOVE:
		{	w->inputHandler()->injectMousePosition(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));
		}break;
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}break;
		}
		return 0;
	}
