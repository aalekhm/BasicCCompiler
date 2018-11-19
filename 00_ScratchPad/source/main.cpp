#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
	int i = 0;
	if (i < 10)
	{
		i = 11;
		int i = 12;
	}

	i = 2;
	int j = ++i + (2 + i);

	std::vector<int> v(3, 100);
	
	v.insert(v.begin() + 1, 10);

	exit(EXIT_SUCCESS);
}