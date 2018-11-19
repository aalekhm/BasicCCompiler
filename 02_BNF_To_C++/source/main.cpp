#include <iostream>
#include "BNFToCpp.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: BNFToCpp.exe BNFGrammerInoutFile.gr ClassName" << std::endl;
	}

	BNFToCpp* pBNFToCpp = new BNFToCpp();
	pBNFToCpp->process(argv[1], argv[2]);

	system("copyTinyCReader.bat");

	exit(EXIT_SUCCESS);
}