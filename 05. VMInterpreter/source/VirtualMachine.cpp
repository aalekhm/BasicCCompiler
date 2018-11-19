#include "VirtualMachine.h"
#include "RandomAccessFile.h"
#include <assert.h>

VirtualMachine*	VirtualMachine::m_pVMInstance = nullptr;

enum class PRIMIIVETYPE
{
	INT_8,
	INT_16,
	INT_32,
	INT_64
};

struct CodeMap
{
	const char*		sOpCode;
	OPCODE			eOpCode;
	int				iOpcodeOperandCount;
	PRIMIIVETYPE	ePRIMIIVETYPE;
} opCodeMap[] =
{
	{ "NOP",		OPCODE::NOP,		1,  PRIMIIVETYPE::INT_8 },
	{ "FETCH",		OPCODE::FETCH,		2,  PRIMIIVETYPE::INT_32 },
	{ "STORE",		OPCODE::STORE,		2,  PRIMIIVETYPE::INT_32 },
	{ "PUSH",		OPCODE::PUSH,		2,  PRIMIIVETYPE::INT_32 },
	{ "POP",		OPCODE::POP,		2,  PRIMIIVETYPE::INT_32 },
	{ "MUL",		OPCODE::MUL,		1,  PRIMIIVETYPE::INT_8 },
	{ "DIV",		OPCODE::DIV,		1,  PRIMIIVETYPE::INT_8 },
	{ "MOD",		OPCODE::MOD,		1,  PRIMIIVETYPE::INT_8 },
	{ "ADD",		OPCODE::ADD,		1,  PRIMIIVETYPE::INT_8 },
	{ "SUB",		OPCODE::SUB,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_LT",		OPCODE::JMP_LT,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_LTEQ",	OPCODE::JMP_LTEQ,	1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_GT",		OPCODE::JMP_GT,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_GTEQ",	OPCODE::JMP_GTEQ,	1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_EQ",		OPCODE::JMP_EQ,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_NEQ",	OPCODE::JMP_NEQ,	1,  PRIMIIVETYPE::INT_8 },
	{ "LOGICALOR",	OPCODE::LOGICALOR,	1,  PRIMIIVETYPE::INT_8 },
	{ "LOGICALAND",	OPCODE::LOGICALAND,	1,  PRIMIIVETYPE::INT_8 },
	{ "NOT",		OPCODE::_NOT,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP",		OPCODE::JMP,		2,  PRIMIIVETYPE::INT_32 },
	{ "JZ",			OPCODE::JZ,			2,  PRIMIIVETYPE::INT_32 },
	{ "JNZ",		OPCODE::JNZ,		2,  PRIMIIVETYPE::INT_32 },
	{ "PRTS",		OPCODE::PRTS,		1,  PRIMIIVETYPE::INT_8 },
	{ "PRTC",		OPCODE::PRTC,		1,  PRIMIIVETYPE::INT_8 },
	{ "PRTI",		OPCODE::PRTI,		1,  PRIMIIVETYPE::INT_8 },
	{ "CALL",		OPCODE::CALL,		2,  PRIMIIVETYPE::INT_32 },
	{ "RET",		OPCODE::RET,		1,  PRIMIIVETYPE::INT_8 },
	{ "SUB_REG",	OPCODE::SUB_REG,	3,  PRIMIIVETYPE::INT_8 },

	{ "PUSHI",		OPCODE::PUSHI,		2,  PRIMIIVETYPE::INT_32 },
	{ "PUSHR",		OPCODE::PUSHR,		2,  PRIMIIVETYPE::INT_8 },
	{ "POPI",		OPCODE::POPI,		2,  PRIMIIVETYPE::INT_8 },
	{ "POPR",		OPCODE::POPR,		2,  PRIMIIVETYPE::INT_8 },
	{ "NEGATE",		OPCODE::NEGATE,		1,  PRIMIIVETYPE::INT_8 },

	{ "HLT",		OPCODE::HLT,		1,  PRIMIIVETYPE::INT_8 },
};

VirtualMachine::VirtualMachine()
{ }

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
				load(sBuff, iLength);
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
	memset(&RAM, 0, sizeof(char) * MAX_RAM_SIZE);

	REGS.EIP = 0;
	REGS.RSP = 0;
	REGS.RBP = 0;

	REGS.CS = CS_START_OFFSET;
	REGS.SS = SS_START_OFFSET;
	REGS.DS = DS_START_OFFSET;

	m_bRunning = false;
}

int VirtualMachine::loadBSS(const char* iByteCode, int startOffset, int iBuffLength)
{
	int iOffset = startOffset;
	int iStringCount = iByteCode[iOffset++];

	int iStringStartOffset = DS_START_OFFSET + (iStringCount * sizeof(int32_t));
	int32_t* pStringLocOffset = (int32_t*)&RAM[DS_START_OFFSET];
	for (int i = 0; i < iStringCount; i++)
	{
		int iStringSize = iByteCode[iOffset++];

		*pStringLocOffset++ = iStringStartOffset;

		memcpy(&RAM[iStringStartOffset], iByteCode + iOffset, sizeof(char) * iStringSize);
		RAM[iStringStartOffset + sizeof(char) * iStringSize + 1] = 0;

		iStringStartOffset += iStringSize + 1;
		iOffset += iStringSize;
	}

	return iOffset;
}

int VirtualMachine::loadCode(const char* iByteCode, int startOffset, int iBuffLength)
{
	int iOffset = startOffset;
	int iCodeSize = iBuffLength - iOffset;

	memcpy(CODE, iByteCode + iOffset, sizeof(char) * iCodeSize);

	iOffset += iCodeSize;

	return iOffset;
}

void VirtualMachine::load(const char* iByteCode, int iBuffLength)
{
	CODE = (int8_t*)&RAM[CS_START_OFFSET];
	STACK = (int32_t*)&RAM[SS_START_OFFSET];

	int iEndOffset = 0;
	iEndOffset = loadBSS(iByteCode, 0, iBuffLength);
	iEndOffset = loadCode(iByteCode, iEndOffset, iBuffLength);
}

void VirtualMachine::execute(const char* iByteCode)
{
	m_bRunning = true;
	while (m_bRunning)
	{
		eval(fetch());
	}
}

OPCODE VirtualMachine::fetch()
{
	return (OPCODE)CODE[REGS.EIP++];
}

void VirtualMachine::eval(OPCODE eOpCode)
{
	int iOperand = 0, iOperand2 = 0, iTemp1 = 0, iTemp2 = 0;
	switch (eOpCode)
	{
		case OPCODE::FETCH:
			iOperand = READ_OPERAND(eOpCode);
			STACK[--REGS.RSP] = STACK[REGS.RBP - iOperand];
		break;
		case OPCODE::STORE:
			iOperand = READ_OPERAND(eOpCode);
			STACK[REGS.RBP - iOperand] = STACK[REGS.RSP++];
		break;
		case OPCODE::PUSH:
		case OPCODE::PUSHI:
			iOperand = READ_OPERAND(eOpCode);
			STACK[--REGS.RSP] = iOperand;
		break;
		case OPCODE::PUSHR:
			iOperand = READ_OPERAND(eOpCode);
			switch (iOperand)
			{
				case (int)EREGISTERS::RAX:	// Accumulator
					STACK[--REGS.RSP] = REGS.RAX;
				break;
				case (int)EREGISTERS::RCX:	// Counter
					STACK[--REGS.RSP] = REGS.RCX;
				break;
				case (int)EREGISTERS::RDX:	// Data
					STACK[--REGS.RSP] = REGS.RDX;
				break;
				case (int)EREGISTERS::RBX:	// Base
					STACK[--REGS.RSP] = REGS.RBX;
				break;
				case (int)EREGISTERS::RSP:	// Stack Pointer
					STACK[--REGS.RSP] = REGS.RSP;
				break;
				case (int)EREGISTERS::RBP:	// Stack Base Pointer
					STACK[--REGS.RSP] = REGS.RBP;
				break;
				case (int)EREGISTERS::RSI:	// Source
					STACK[--REGS.RSP] = REGS.RSI;
				break;
				case (int)EREGISTERS::RDI:	// Destination
					STACK[--REGS.RSP] = REGS.RDI;
				break;
			}
		break;
		case OPCODE::POP:
		case OPCODE::POPI:
		break;
		case OPCODE::POPR:
			iOperand = READ_OPERAND(eOpCode);
			switch (iOperand)
			{
				case (int)EREGISTERS::RAX:	// Accumulator
					REGS.RAX = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RCX:	// Counter
					REGS.RCX = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RDX:	// Data
					REGS.RDX = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RBX:	// Base
					REGS.RBX = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RSP:	// Stack Pointer
					REGS.RSP = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RBP:	// Stack Base Pointer
					REGS.RBP = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RSI:	// Source
					REGS.RSI = STACK[REGS.RSP++];
				break;
				case (int)EREGISTERS::RDI:	// Destination
					REGS.RDI = STACK[REGS.RSP++];
				break;
			}
		break;
		case OPCODE::CALL:
			iOperand = READ_OPERAND(eOpCode);
			REGS.RBP = REGS.RSP;					// ESP is now the new EBP.
			REGS.EIP = iOperand;					// Jump to the call address.
		break;
		case OPCODE::RET:
			REGS.EIP = STACK[REGS.RSP++];			// Pop the Return address off the stack.
		break;
		case OPCODE::SUB_REG:
			iOperand = READ_OPERAND(eOpCode);
			iOperand2 = READ_OPERAND(eOpCode);

			switch (iOperand)
			{
				case (int)EREGISTERS::RAX:	// Accumulator
					REGS.RAX += iOperand2;
				break;
				case (int)EREGISTERS::RCX:	// Counter
					REGS.RCX += iOperand2;
				break;
				case (int)EREGISTERS::RDX:	// Data
					REGS.RDX += iOperand2;
				break;
				case (int)EREGISTERS::RBX:	// Base
					REGS.RBX += iOperand2;
				break;
				case (int)EREGISTERS::RSP:	// Stack Pointer
					REGS.RSP += iOperand2;
				break;
				case (int)EREGISTERS::RBP:	// Stack Base Pointer
					REGS.RBP += iOperand2;
				break;
				case (int)EREGISTERS::RSI:	// Source
					REGS.RSI += iOperand2;
				break;
				case (int)EREGISTERS::RDI:	// Destination
					REGS.RDI += iOperand2;
				break;
			}
		break;
		case OPCODE::MUL:
			iTemp1 = STACK[REGS.RSP++];
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 *= iTemp2;

			STACK[--REGS.RSP] = iTemp1;
		break;
		case OPCODE::DIV:
			iTemp1 = STACK[REGS.RSP++];
			iTemp2 = STACK[REGS.RSP++];
			iTemp2 /= iTemp1;

			STACK[--REGS.RSP] = iTemp2;
		break;
		case OPCODE::MOD:
			iTemp1 = STACK[REGS.RSP++];
			iTemp2 = STACK[REGS.RSP++];
			iTemp2 %= iTemp1;

			STACK[--REGS.RSP] = iTemp2;
		break;
		case OPCODE::ADD:
			iTemp1 = STACK[REGS.RSP++];
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 += iTemp2;

			STACK[--REGS.RSP] = iTemp1;
		break;
		case OPCODE::SUB:
			iTemp1 = STACK[REGS.RSP++];
			iTemp2 = STACK[REGS.RSP++];
			iTemp2 -= iTemp1;

			STACK[--REGS.RSP] = iTemp2;
		break;
		case OPCODE::JMP_LT:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 < iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::JMP_LTEQ:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 <= iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::JMP_GT:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 > iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::JMP_GTEQ:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 >= iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::JMP_EQ:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 == iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::JMP_NEQ:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 != iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::LOGICALOR:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 || iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::LOGICALAND:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			if (iTemp1 && iTemp2)
				STACK[--REGS.RSP] = 1;
			else
				STACK[--REGS.RSP] = 0;
		break;
		case OPCODE::_NOT:
			iTemp1 = STACK[REGS.RSP++];
			if(iTemp1 > 0)
				STACK[--REGS.RSP] = 0;
			else
				STACK[--REGS.RSP] = 1;
		break;
		case OPCODE::NEGATE:
			iTemp1 = STACK[REGS.RSP++];
			STACK[--REGS.RSP] = -iTemp1;
		break;
		case OPCODE::JMP:
			iOperand = READ_OPERAND(eOpCode);
			REGS.EIP = iOperand;
		break;
		case OPCODE::JZ:
			iOperand = READ_OPERAND(eOpCode);
			iTemp1 = STACK[REGS.RSP++];
			if(iTemp1 == 0)
				REGS.EIP = iOperand;
		break;
		case OPCODE::JNZ:
			iOperand = READ_OPERAND(eOpCode);
			iTemp1 = STACK[REGS.RSP++];
			if(iTemp1 > 0)
				REGS.EIP = iOperand;
		break;
		case OPCODE::PRTS:
		{
			iTemp1 = STACK[REGS.RSP++];

			int32_t* pDS = (int32_t*)&RAM[DS_START_OFFSET];

			int32_t iStringOffset = *(pDS + iTemp1);
			printf("%s", &RAM[iStringOffset]);
		}
		break;
		case OPCODE::PRTC:
			iTemp1 = STACK[REGS.RSP++];
			printf("%c", iTemp1);
		break;
		case OPCODE::PRTI:
			iTemp1 = STACK[REGS.RSP++];
			printf("%d", iTemp1);
		break;
		case OPCODE::HLT:
			m_bRunning = false;
		break;
	}

	assert(abs(REGS.RSP) < MAX_STACK_SIZE);
	assert(REGS.EIP < MAX_BYTECODE_SIZE);
}

int64_t VirtualMachine::readOperandFor(OPCODE eOpCode)
{
	CodeMap pMachineInstruction = opCodeMap[(int)eOpCode];
	switch (pMachineInstruction.ePRIMIIVETYPE)
	{
		case PRIMIIVETYPE::INT_8:
		{
			int8_t iByte = *((int8_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
			REGS.EIP += sizeof(int8_t);

			return iByte;
		}
		break;
		case PRIMIIVETYPE::INT_16:
		{
			int16_t iShort = *((int16_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
			REGS.EIP += sizeof(int16_t);

			return iShort;
		}
		break;
		case PRIMIIVETYPE::INT_32:
		{
			int32_t iInt = *((int32_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
			REGS.EIP += sizeof(int32_t);

			return iInt;
		}
		break;
		case PRIMIIVETYPE::INT_64:
		{
			int64_t iLong = *((int64_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
			REGS.EIP += sizeof(int64_t);

			return iLong;
		}
		break;
	}
}