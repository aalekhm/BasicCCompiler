#pragma once
#include <windows.h>
#include <ostream>

struct color
{
    color(WORD wAttribute)
	: m_colorAttribute(wAttribute)
	{};

    WORD m_colorAttribute;
};

color red(FOREGROUND_RED | FOREGROUND_INTENSITY);
color green(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
color blue(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
color white(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
color yellow(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

std::ostream& operator<<(std::ostream& os, color c)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
							c.m_colorAttribute);
	
	return os;
}