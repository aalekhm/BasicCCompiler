#include <assert.h>
#include <iostream>
#include <functional>
#include "VirtualMachine.h"
#include "meta/MetaFunction.h"

// Dummy System Functions.
void glLoadIdentity()
{
	std::cout << "In glLoadIdentity();" << std::endl;
}

void glClearColor(int32_t iRed, int32_t iGreen, int32_t iBlue, int32_t iAlpha)
{
	std::cout << "In glClearColor(" << iRed << ", " << iGreen << ", " << iBlue << ", " << iAlpha << ");" << std::endl;
}

void glColor3f(float fRed, float fGreen, float fBlue)
{
	std::cout << "In glColor3f(" << fRed << ", " << fGreen << ", " << fBlue << ");" << std::endl;
}

int32_t retSysFunc(int32_t iValue)
{
	std::cout << "In retSysFunc(" << iValue << ", retValue = " << (iValue * 3) << ");" << std::endl;
	return iValue * 3;
}

float retFloatFunc(float fValue)
{
	std::cout << "In retFloatFunc(" << fValue << ", retValue = " << (fValue * 3) << ");" << std::endl;
	return fValue * 3;
}

float glColor3fMul(float fRed, float fGreen, float fBlue)
{
	std::cout << "In glColor3fMul(" << fRed << ", " << fGreen << ", " << fBlue << "); = " << (fRed * fGreen * fBlue) << std::endl;
	return fRed * fGreen * fBlue;
}

META_REGISTER_FUN(glLoadIdentity);
META_REGISTER_FUN(glClearColor);
META_REGISTER_FUN(glColor3f);
META_REGISTER_FUN(retSysFunc);
META_REGISTER_FUN(retFloatFunc);
META_REGISTER_FUN(glColor3fMul);

void scriptTest()
{
	// Script Test
	{
		GetFunctionByName("glLoadIdentity")->call(nullptr, nullptr, 0);

		int32_t iRed = 128, iGreen = 64, iBlue = 32, iAlpha = 255;
		Variable iArgs[] = { iRed, iGreen, iBlue, iAlpha };
		GetFunctionByName("glClearColor")->call(nullptr, iArgs, 4);

		float fRed = 128.1f, fGreen = 64.2f, fBlue = 32.3f;
		Variable fArgs[] = { fRed, fGreen, fBlue };
		GetFunctionByName("glColor3f")->call(nullptr, fArgs, 4);

		int32_t iRetValue = 0;
		Variable iRetVar(iRetValue);
		Variable iArg(iRed);
		GetFunctionByName("retSysFunc")->call(iRetVar, &iArg, 1);
	}
}

VirtualMachine* pVM = nullptr;
void onScriptCallback(const char* sSysFuncName, int16_t iArgCount);
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: VirtualMachineInterpreter.exe filename.o" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::function<void(const char* sSysFuncName, int16_t iArgCount)> fSysFuncCallback = onScriptCallback;

	pVM = VirtualMachine::create(&fSysFuncCallback);
	pVM->run(argv[1]);
	pVM->restart();

	exit(EXIT_SUCCESS);
}

void onScriptCallback(const char* sSysFuncName, int16_t iArgCount)
{
	MetaFunction* pMetaFunction = GetFunctionByName(sSysFuncName);
	assert(pMetaFunction != nullptr);
	if (pMetaFunction != nullptr)
	{
		// Return Value
		Variable ret;
		{
			ret.m_MetaType = pMetaFunction->getRetType();
			if (ret.m_MetaType->sizeOf() > 0)
				ret.m_Var = ret.m_MetaType->newAlloc();
		}

		// Function Arguments
		Variable* pArgs = new Variable[iArgCount];
		{
			for (int32_t i = 0; i < pMetaFunction->getArgCount(); i++)
			{
				const MetaType* pArgMetaType = pMetaFunction->getArgType(i);
				pArgs[i].m_MetaType = pArgMetaType;
				pArgs[i].m_Var = pArgs[i].m_MetaType->newAlloc();
				pArgs[i].m_MetaType->setValueFrom(pArgs[i].m_Var, (void*)pVM->getStackPointerFromTOS(i));
			}
		}

		// Function Call
		pMetaFunction->call(ret, pArgs, iArgCount);

		// Check Return type & push it on the 'STACK'.
		size_t iRetVarSize = ret.m_MetaType->sizeOf();
		if (iRetVarSize > 0)
		{
			REGISTERS* pVMRegisters = pVM->getVMRegisters();
			memcpy_s(&pVMRegisters->RAX, iRetVarSize, ret.m_Var, iRetVarSize);
		}

		// Function Cleanup
		{
			for (int32_t i = 0; i < pMetaFunction->getArgCount(); i++)
			{
				pArgs[i].m_MetaType->deleteAlloc(pArgs[i].m_Var);
			}
			delete[] pArgs;
			if (ret.m_MetaType->sizeOf() > 0)
				ret.m_MetaType->deleteAlloc(ret.m_Var);
		}
	}
}