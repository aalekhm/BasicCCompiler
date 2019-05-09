#include "Dream3DTest.h"
#include "VirtualMachine.h"
#include "meta/MetaFunction.h"
#include "Engine/EngineManager.h"
#include <cstdlib>

void	createGLRotationMatrix(float _x, float _y, float _angle);
float	osClock_ElapsedTimeinSeconds();
float	osClock_DeltaTimeInMs();
void	randomSeed(uint32_t iSeed);
float	random();
float	sine(float fAngleInRadians);
int32_t	getScreenWidth();
int32_t	getScreenHeight();
int32_t getMouseX();
int32_t getMouseY();
int32_t	isKeyPressed(int32_t iKeyCode);

META_REGISTER_FUN(glMatrixMode);
META_REGISTER_FUN(glLoadIdentity);
META_REGISTER_FUN(glClearColor);
META_REGISTER_FUN(glClear);
META_REGISTER_FUN(glBegin);
META_REGISTER_FUN(glColor3f);
META_REGISTER_FUN(glVertex2f);
META_REGISTER_FUN(glVertex2i);
META_REGISTER_FUN(glEnd);
META_REGISTER_FUN(osClock_ElapsedTimeinSeconds);
META_REGISTER_FUN(osClock_DeltaTimeInMs);
META_REGISTER_FUN(randomSeed);
META_REGISTER_FUN(random);
META_REGISTER_FUN(sine);
META_REGISTER_FUN(getScreenWidth);
META_REGISTER_FUN(getScreenHeight);
META_REGISTER_FUN(getMouseX);
META_REGISTER_FUN(getMouseY);
META_REGISTER_FUN(isKeyPressed);
META_REGISTER_FUN_(createGLRotationMatrix, "glRotationzf");

static std::function<void(const char* sSysFuncName, int16_t iArgCount)> fSysFuncCallback = nullptr;

Dream3DTest engine;

Dream3DTest::Dream3DTest()
{
}

Dream3DTest::~Dream3DTest()
{
}

void Dream3DTest::initialize()
{
	fSysFuncCallback = [this](const char* sSysFuncName, int16_t iArgCount)
	{
		this->onScriptCallback(sSysFuncName, iArgCount);
	};

	m_pVM = VirtualMachine::create(&fSysFuncCallback);
	const char* sFileName = "TestCases/main.o";
	m_pVM->loadFile(sFileName);

	LoadOpenGLFunctions(false);
}

void Dream3DTest::update(float elapsedTime)
{

}

void Dream3DTest::render(float elapsedTime)
{
	m_pVM->start();
}
	 
void Dream3DTest::keyPressedEx(unsigned int iVirtualKeycode, unsigned short ch)
{

}

void Dream3DTest::keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch)
{

}
	 
void Dream3DTest::onMouseDownEx(int mCode, int x, int y)
{
	m_iMouseX = x;
	m_iMouseY = y;
}

void Dream3DTest::onMouseMoveEx(int mCode, int x, int y)
{
	m_iMouseX = x;
	m_iMouseY = y;
}

void Dream3DTest::onMouseUpEx(int mCode, int x, int y)
{
	m_iMouseX = x;
	m_iMouseY = y;
}

void Dream3DTest::onMouseWheelEx(WPARAM wParam, LPARAM lParam)
{

}

void Dream3DTest::onScriptCallback(const char* sSysFuncName, int16_t iArgCount)
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
				pArgs[i].m_MetaType->setValueFrom(pArgs[i].m_Var, (void*)m_pVM->getStackPointerFromTOS(i));
			}
		}

		// Function Call
		pMetaFunction->call(ret, pArgs, iArgCount);

		// Check Return type & push it on the 'STACK'.
		size_t iRetVarSize = ret.m_MetaType->sizeOf();
		if (iRetVarSize > 0)
		{
			REGISTERS* pVMRegisters = m_pVM->getVMRegisters();
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

void createGLRotationMatrix(float _x, float _y, float _angle)
{
	float m[] =
	{
		std::cosf(_angle), -std::sinf(_angle), 0, 0,
		std::sinf(_angle), std::cosf(_angle) , 0, 0,
		0                , 0                 , 1, 0,
		_x               , _y                , 0, 1
	};

	glLoadMatrixf(m);
}

float osClock_ElapsedTimeinSeconds()
{
	return (float)engine.getTimer()->getElapsedTimeInSec();
}

float osClock_DeltaTimeInMs()
{
	return (float)engine.getTimer()->getDeltaTimeMs();
}

void randomSeed(uint32_t iSeed)
{
	std::srand(iSeed);
}

float random()
{
	return ((float)std::rand() / RAND_MAX);
}

float sine(float fAngleInRadians)
{
	return std::sin(fAngleInRadians);
}

int32_t getScreenWidth()
{
	return engine.getWidth();
}

int32_t	getScreenHeight()
{
	return engine.getHeight();
}

int32_t getMouseX()
{
	return engine.m_iMouseX;
}

int32_t getMouseY()
{
	return engine.m_iMouseY;
}

int32_t	isKeyPressed(int32_t iKeyCode)
{
	return engine.isKeyPressed(iKeyCode);
}