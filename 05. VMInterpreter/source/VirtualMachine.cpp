#include "VirtualMachine.h"
#include "RandomAccessFile.h"

VirtualMachine*	VirtualMachine::m_pVMInstance = nullptr;

struct CodeMap
{
	const char*		sOpCode;
	OPCODE			eOpCode;
	int				iOpcodeOperandCount;
} opCodeMap[] =
{
	{ "NOP",		OPCODE::NOP,		1 },
	{ "FETCH",		OPCODE::FETCH,		2 },
	{ "STORE",		OPCODE::STORE,		2 },
	{ "PUSH",		OPCODE::PUSH,		2 },
	{ "POP",		OPCODE::POP,		2 },
	{ "MUL",		OPCODE::MUL,		1 },
	{ "DIV",		OPCODE::DIV,		1 },
	{ "MOD",		OPCODE::MOD,		1 },
	{ "ADD",		OPCODE::ADD,		1 },
	{ "SUB",		OPCODE::SUB,		1 },
	{ "JMP_LT",		OPCODE::JMP_LT,		1 },
	{ "JMP_LTEQ",	OPCODE::JMP_LTEQ,	1 },
	{ "JMP_GT",		OPCODE::JMP_GT,		1 },
	{ "JMP_GTEQ",	OPCODE::JMP_GTEQ,	1 },
	{ "JMP_EQ",		OPCODE::JMP_EQ,		1 },
	{ "JMP_NEQ",	OPCODE::JMP_NEQ,	1 },
	{ "LOGICALOR",	OPCODE::LOGICALOR,	1 },
	{ "LOGICALAND",	OPCODE::LOGICALAND,	1 },
	{ "NOT",		OPCODE::_NOT,		1 },
	{ "JMP",		OPCODE::JMP,		2 },
	{ "JZ",			OPCODE::JZ,			2 },
	{ "JNZ",		OPCODE::JNZ,		2 },
	{ "PRTS",		OPCODE::PRTS,		1 },
	{ "PRTC",		OPCODE::PRTC,		1 },
	{ "PRTI",		OPCODE::PRTI,		1 },
	{ "CALL",		OPCODE::CALL,		2 },
	{ "RET",		OPCODE::RET,		1 },
	{ "SUB_REG",	OPCODE::SUB_REG,	3 },
	{ "PUSHI",		OPCODE::PUSHI,		2 },
	{ "PUSHR",		OPCODE::PUSHR,		2 },
	{ "POPI",		OPCODE::POPI,		2 },
	{ "POPR",		OPCODE::POPR,		2 },
	{ "NEGATE",		OPCODE::NEGATE,		1 },

	{ "HLT",		OPCODE::HLT,		1 },
};

VirtualMachine::VirtualMachine()
{

}

VirtualMachine::~VirtualMachine()
{

}

VirtualMachine*	VirtualMachine::create()
{
	if (m_pVMInstance == nullptr)
	{
		m_pVMInstance = new VirtualMachine();
	}

	return m_pVMInstance;
}

void VirtualMachine::run(const char* sMachineCodeFile)
{
	if (sMachineCodeFile != nullptr && strlen(sMachineCodeFile) > 0)
	{
		RandomAccessFile* pRaf = new RandomAccessFile();
		bool bCanRead = pRaf->openForRead(sMachineCodeFile);
		if (bCanRead)
		{
			unsigned long iLength = pRaf->length();
			char* sBuff = new char[iLength + 1];
			memset(sBuff, 0, iLength);

			unsigned long iBytesRead = pRaf->read(sBuff);
			if (iBytesRead > 0)
			{
				reset();
				execute(sBuff);
			}

			delete[] sBuff;
			sBuff = nullptr;
		}
		pRaf->close();
	}
}

void VirtualMachine::reset()
{
	memset(&REGS, 0, sizeof(REGS));
	memset(&STACK, 0, sizeof(int) * MAX_STACK_SIZE);

	m_pByteCode = nullptr;
	m_bRunning = false;

	if (m_pVariables != nullptr)
	{
		delete[] m_pVariables;
	}
	m_vStrings.clear();
}

void VirtualMachine::execute(const char* iByteCode)
{
	m_bRunning = true;

	int iOffset = 0;

	int iStringCount = iByteCode[iOffset++];
	for (int i = 0; i < iStringCount; i++)
	{
		int iStringSize = iByteCode[iOffset++];
		char sString[255] = { 0 };
		memcpy(sString, iByteCode + iOffset, sizeof(char) * iStringSize);
		m_vStrings.push_back(sString);

		iOffset += iStringSize;
	}

	m_pByteCode = (int*)(iByteCode + iOffset);
	REGS.EIP = 0;
	REGS.RSP = -1;
	REGS.RBP = -1;

	while (m_bRunning)
	{
		eval(fetch());
	}
}

OPCODE VirtualMachine::fetch()
{
	return (OPCODE)m_pByteCode[REGS.EIP++];
}

void VirtualMachine::eval(OPCODE eOpCode)
{
	int iOperand = 0, iOperand2 = 0, iTemp1 = 0, iTemp2 = 0;
	switch (eOpCode)
	{
		case OPCODE::FETCH:
			iOperand = m_pByteCode[REGS.EIP++];
			STACK[++REGS.RSP] = STACK[REGS.RBP + iOperand];
		break;
		case OPCODE::STORE:
			iOperand = m_pByteCode[REGS.EIP++];
			STACK[REGS.RBP + iOperand] = STACK[REGS.RSP--];
		break;
		case OPCODE::PUSH:
		case OPCODE::PUSHI:
			iOperand = m_pByteCode[REGS.EIP++];
			STACK[++REGS.RSP] = iOperand;
		break;
		case OPCODE::PUSHR:
			iOperand = m_pByteCode[REGS.EIP++];
			switch (iOperand)
			{
				case (int)EREGISTERS::RAX:	// Accumulator
					STACK[++REGS.RSP] = REGS.RAX;
				break;
				case (int)EREGISTERS::RCX:	// Counter
					STACK[++REGS.RSP] = REGS.RCX;
				break;
				case (int)EREGISTERS::RDX:	// Data
					STACK[++REGS.RSP] = REGS.RDX;
				break;
				case (int)EREGISTERS::RBX:	// Base
					STACK[++REGS.RSP] = REGS.RBX;
				break;
				case (int)EREGISTERS::RSP:	// Stack Pointer
					STACK[++REGS.RSP] = REGS.RSP;
				break;
				case (int)EREGISTERS::RBP:	// Stack Base Pointer
					STACK[++REGS.RSP] = REGS.RBP;
				break;
				case (int)EREGISTERS::RSI:	// Source
					STACK[++REGS.RSP] = REGS.RSI;
				break;
				case (int)EREGISTERS::RDI:	// Destination
					STACK[++REGS.RSP] = REGS.RDI;
				break;
			}
		break;
		case OPCODE::POP:
		case OPCODE::POPI:
		break;
		case OPCODE::POPR:
			iOperand = m_pByteCode[REGS.EIP++];
			switch (iOperand)
			{
				case (int)EREGISTERS::RAX:	// Accumulator
					REGS.RAX = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RCX:	// Counter
					REGS.RCX = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RDX:	// Data
					REGS.RDX = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RBX:	// Base
					REGS.RBX = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RSP:	// Stack Pointer
					REGS.RSP = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RBP:	// Stack Base Pointer
					REGS.RBP = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RSI:	// Source
					REGS.RSI = STACK[REGS.RSP--];
				break;
				case (int)EREGISTERS::RDI:	// Destination
					REGS.RDI = STACK[REGS.RSP--];
				break;
			}
		break;
		case OPCODE::CALL:
			iOperand = m_pByteCode[REGS.EIP++];
			REGS.RBP = REGS.RSP;					// ESP is now the new EBP.
			REGS.EIP = iOperand;					// Jump to the call address.
		break;
		case OPCODE::RET:
			REGS.EIP = STACK[REGS.RSP--];			// Pop the Return address off the stack.

			//STACK[++REGS.RSP] = REGS.RAX;			// PUSH EAX onto the stack.
		break;
		case OPCODE::SUB_REG:
			iOperand = m_pByteCode[REGS.EIP++];
			iOperand2 = m_pByteCode[REGS.EIP++];

			switch (iOperand)
			{
				case (int)EREGISTERS::RAX:	// Accumulator
					REGS.RAX -= iOperand2;
				break;
				case (int)EREGISTERS::RCX:	// Counter
					REGS.RCX -= iOperand2;
				break;
				case (int)EREGISTERS::RDX:	// Data
					REGS.RDX -= iOperand2;
				break;
				case (int)EREGISTERS::RBX:	// Base
					REGS.RBX -= iOperand2;
				break;
				case (int)EREGISTERS::RSP:	// Stack Pointer
					REGS.RSP -= iOperand2;
				break;
				case (int)EREGISTERS::RBP:	// Stack Base Pointer
					REGS.RBP -= iOperand2;
				break;
				case (int)EREGISTERS::RSI:	// Source
					REGS.RSI -= iOperand2;
				break;
				case (int)EREGISTERS::RDI:	// Destination
					REGS.RDI -= iOperand2;
				break;
			}
		break;
		case OPCODE::MUL:
			iTemp1 = STACK[REGS.RSP--];
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 *= iTemp2;

			STACK[++REGS.RSP] = iTemp1;
		break;
		case OPCODE::DIV:
			iTemp1 = STACK[REGS.RSP--];
			iTemp2 = STACK[REGS.RSP--];
			iTemp2 /= iTemp1;

			STACK[++REGS.RSP] = iTemp2;
		break;
		case OPCODE::MOD:
			iTemp1 = STACK[REGS.RSP--];
			iTemp2 = STACK[REGS.RSP--];
			iTemp2 %= iTemp1;

			STACK[++REGS.RSP] = iTemp2;
		break;
		case OPCODE::ADD:
			iTemp1 = STACK[REGS.RSP--];
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 += iTemp2;

			STACK[++REGS.RSP] = iTemp1;
		break;
		case OPCODE::SUB:
			iTemp1 = STACK[REGS.RSP--];
			iTemp2 = STACK[REGS.RSP--];
			iTemp2 -= iTemp1;

			STACK[++REGS.RSP] = iTemp2;
		break;
		case OPCODE::JMP_LT:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 < iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::JMP_LTEQ:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 <= iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::JMP_GT:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 > iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::JMP_GTEQ:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 >= iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::JMP_EQ:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 == iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::JMP_NEQ:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 != iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::LOGICALOR:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 || iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::LOGICALAND:
			iTemp2 = STACK[REGS.RSP--];
			iTemp1 = STACK[REGS.RSP--];

			if (iTemp1 && iTemp2)
				STACK[++REGS.RSP] = 1;
			else
				STACK[++REGS.RSP] = 0;
		break;
		case OPCODE::_NOT:
			iTemp1 = STACK[REGS.RSP--];
			if(iTemp1 > 0)
				STACK[++REGS.RSP] = 0;
			else
				STACK[++REGS.RSP] = 1;
		break;
		case OPCODE::NEGATE:
			iTemp1 = STACK[REGS.RSP--];			
			STACK[++REGS.RSP] = -iTemp1;
		break;
		case OPCODE::JMP:
			iOperand = m_pByteCode[REGS.EIP++];
			REGS.EIP = iOperand;
		break;
		case OPCODE::JZ:
			iOperand = m_pByteCode[REGS.EIP++];
			iTemp1 = STACK[REGS.RSP--];
			if(iTemp1 == 0)
				REGS.EIP = iOperand;
		break;
		case OPCODE::JNZ:
			iOperand = m_pByteCode[REGS.EIP++];
			iTemp1 = STACK[REGS.RSP--];
			if(iTemp1 > 0)
				REGS.EIP = iOperand;
		break;
		case OPCODE::PRTS:
			iTemp1 = STACK[REGS.RSP--];
			printf("%s", m_vStrings[iTemp1].c_str());
		break;
		case OPCODE::PRTC:
			iTemp1 = STACK[REGS.RSP--];
			printf("%c", iTemp1);
		break;
		case OPCODE::PRTI:
			iTemp1 = STACK[REGS.RSP--];
			printf("%d", iTemp1);
		break;
		case OPCODE::HLT:
			m_bRunning = false;
		break;
	}
}