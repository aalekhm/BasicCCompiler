#include <iostream>
#include "VirtualMachine.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: VirtualMachineInterpreter.exe filename.o" << std::endl;
		exit(EXIT_FAILURE);
	}

	VirtualMachine* pVM = VirtualMachine::create();
	pVM->run(argv[1]);

	exit(EXIT_SUCCESS);
}