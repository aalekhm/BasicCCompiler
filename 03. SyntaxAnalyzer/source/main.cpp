
#include <iostream>
#include <fstream>
#include "TinyCReader.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: SyntaxAnalyzer.exe filename.c" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string sFilename = argv[1];
	TinyCReader* pTinyCReader = new TinyCReader();
	pTinyCReader->read(sFilename.c_str());

	exit(EXIT_SUCCESS);
}