#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
	int j = 0;
	++j;

	int m = 0;
	m = ++j + ++j + ++j;

	std::cout << "m = " << m << std::endl;
	int iCount = 1;
	for (int i = 5, j = 1; i < 6; ++i, ++iCount)
	{
		std::cout << "ForLoop i = " << i << " ,val of j = " << j << " ,iCount = " << iCount << std::endl;
	}

	exit(EXIT_SUCCESS);
}