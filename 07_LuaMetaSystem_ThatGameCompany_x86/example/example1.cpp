#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "extern/window.cpp"
#include "extern/gl.cpp"

#include "meta/luaRegister.h"
#include "example/luaGL.h"

void error(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	//lua_close(L);
	//exit(EXIT_FAILURE);
}

//create a rotation matrix around _x and _y and place in on top of the matrix stack
void createGlRotationMatrix(float32 _x, float32 _y, float32 angle){
	float32 m[] = {
		std::cosf(angle), -std::sinf(angle), 0, 0,
		std::sinf(angle), std::cosf(angle), 0, 0,
		0,0,1,0,
		_x,_y,0,1
	};
	glLoadMatrixf(m);
}

void funcStr(const char* sStr)
{
	std::cout << sStr << std::endl;
}

int main(int argc, char* argv[]){
	//Init lua state and bind variables and function defined at global scope
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	MetaBind(L);

	//create window
	Window wnd = Window("Lua extension demo", 1024, 768, 40, 40, DEBUG_CONTEXT | MULTISAMPLE16 | RGB24 | ALPHA8 | DEPTH16 | DOUBLE_BUFFER);
	
	glEnable(GL_DEPTH_TEST);
	float64 startTime = wnd.elapsedTime();
	int32 framecount = 0;

	META_REGISTER_FUN(funcStr, "funcStr");

	do{
		if (luaL_dofile(L, "example/example_loop.lua"))
			error(L, "cannot run configuration file: %s", lua_tostring(L, -1));

		wnd.swapBuffers();

		float64 currentTime = wnd.elapsedTime();
		framecount++;
		if (currentTime - startTime >= 1){
			float64 fps = (float64)framecount / (currentTime - startTime);
			framecount = 0;
			std::cout << "Rendering at ~ " << fps << " FPS or " << 1000.0 / fps << "ms per frame" << std::endl;
			startTime = currentTime;
		}

	} while (wnd.processAllMessages() && wnd.getKey('c') != InputHandler::ButtonState::PRESSED);
	lua_close(L);
	return 0;
}
