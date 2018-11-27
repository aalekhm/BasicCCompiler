#include "VirtualMachine.h"
#include "RandomAccessFile.h"
#include <assert.h>
#include <algorithm>

VirtualMachine*	VirtualMachine::m_pVMInstance = nullptr;

enum class PRIMIIVETYPE
{
	INT_8,
	INT_16,
	INT_32,
	INT_64
};

enum class E_VARIABLETYPE
{
	INVALID = -1,
	ARGUMENT,
	LOCAL,
	STATIC,
	MEMBER
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
	{ "BITWISEOR",	OPCODE::BITWISEOR,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISEAND",	OPCODE::BITWISEAND,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISEXOR",	OPCODE::BITWISEXOR,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISENOT",	OPCODE::BITWISENOT,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISELEFTSHIFT",	OPCODE::BITWISELEFTSHIFT,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISERIGHTSHIFT",	OPCODE::BITWISERIGHTSHIFT,	1,  PRIMIIVETYPE::INT_8 },
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

	{ "MALLOC",		OPCODE::MALLOC,		1,  PRIMIIVETYPE::INT_8 },
	{ "FREE",		OPCODE::FREE,		2,  PRIMIIVETYPE::INT_32 },

	{ "LDA",		OPCODE::LDA,		2,  PRIMIIVETYPE::INT_32},
	{ "STA",		OPCODE::STA,		2,  PRIMIIVETYPE::INT_32},

	{ "CLR",		OPCODE::CLR,		7,  PRIMIIVETYPE::INT_32},

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

	m_vUnAllocatedList.push_back(HeapNode(0, MAX_HEAP_SIZE));

	m_bRunning = false;
}

int VirtualMachine::loadBSS(const char* iByteCode, int startOffset, int iBuffLength)
{
	int iOffset = startOffset;
	int iStringCount = iByteCode[iOffset++];

	int iStringSize = 0;
	int iStringStartOffset = DS_START_OFFSET + (iStringCount * sizeof(int32_t));
	int32_t* pStringLocOffset = (int32_t*)DATA;

	// Load Strings in Memory
	for (int i = 0; i < iStringCount; i++)
	{
		iStringSize = iByteCode[iOffset++];

		*pStringLocOffset++ = iStringStartOffset;

		memcpy(&RAM[iStringStartOffset], iByteCode + iOffset, sizeof(char) * iStringSize);
		RAM[iStringStartOffset + sizeof(char) * iStringSize + 1] = 0;

		iStringStartOffset += iStringSize + 1;
		iOffset += iStringSize;
	}

	// Get Static variable count
	{
		int32_t iStaticVariableCount = (*(int32_t*)&iByteCode[iOffset]);

		GLOBALS = (int32_t*)&RAM[iStringStartOffset];
		iOffset += sizeof(int32_t);
		iStringStartOffset += sizeof(int32_t) * iStaticVariableCount;
	}

	HEAP = &RAM[iStringStartOffset];
	REGS.GS = iStringStartOffset;

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
	DATA = (int8_t*)&RAM[DS_START_OFFSET];

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
	int32_t iOperand = 0, iOperand2 = 0, iTemp1 = 0, iTemp2 = 0, iTemp3 = 0;
	switch (eOpCode)
	{
		case OPCODE::FETCH:
		{
			int32_t iVariable = READ_OPERAND(eOpCode);
			{
				int32_t iAddress = getAddressOf(iVariable);
				STACK[--REGS.RSP] = iAddress;
			}
		}
		break;
		case OPCODE::STORE:
		{
			int32_t iVariable = READ_OPERAND(eOpCode);
			{
				int16_t iVariablePos = (iVariable & 0x0000FFFF);
				E_VARIABLETYPE eVariableType = (E_VARIABLETYPE)((int32_t)iVariable >> (sizeof(int16_t) * 8));

				// Local Var or Function Argument ==> saved on the STACK
				if (eVariableType == E_VARIABLETYPE::ARGUMENT
					||
					eVariableType == E_VARIABLETYPE::LOCAL
				) {
					if (eVariableType == E_VARIABLETYPE::ARGUMENT)
						iVariablePos *= -1;

					STACK[REGS.RBP - iVariablePos] = STACK[REGS.RSP++];
				}
				else // STATIC variable saved on the HEAP
				{
					GLOBALS[iVariablePos] = STACK[REGS.RSP++];
				}
			}
		}
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
		case OPCODE::BITWISEOR:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			STACK[--REGS.RSP] = (iTemp1 | iTemp2);
		break;
		case OPCODE::BITWISEAND:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			STACK[--REGS.RSP] = (iTemp1 & iTemp2);
		break;
		case OPCODE::BITWISEXOR:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			STACK[--REGS.RSP] = (iTemp1 ^ iTemp2);
		break;
		case OPCODE::BITWISENOT:
			iTemp1 = STACK[REGS.RSP++];

			STACK[--REGS.RSP] = (~iTemp1);
		break;
		case OPCODE::BITWISELEFTSHIFT:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			STACK[--REGS.RSP] = (iTemp1 << iTemp2);
		break;
		case OPCODE::BITWISERIGHTSHIFT:
			iTemp2 = STACK[REGS.RSP++];
			iTemp1 = STACK[REGS.RSP++];

			STACK[--REGS.RSP] = (iTemp1 >> iTemp2);
		break;
		case OPCODE::_NOT:
			iTemp1 = STACK[REGS.RSP++];
			if(iTemp1 > 0)
				STACK[--REGS.RSP] = 0;
			else
				STACK[--REGS.RSP] = 1;
		break;
		case OPCODE::LDA:
		{
			// LDA - Load Value from memory address in Accumulator(in our case, the STACK)
			iOperand = READ_OPERAND(eOpCode);			// Pointer Variable Type(int8_t = 0xFF, int16_6 = 0xFFFF, int32_t = 0xFFFFFFFF).

			int32_t iVarType = STACK[REGS.RSP++];		// Variable TYPE (int8_t = 1, int16_t = 2, int32_t = 4).
			int32_t iAddress = STACK[REGS.RSP++];
			int32_t iArrayIndex = STACK[REGS.RSP++];	// ArrayIndex.
			
			int8_t* pAddress_8 = (int8_t*)&HEAP[iAddress];
			pAddress_8 += (iArrayIndex * iVarType);
			int32_t* pAddress = (int32_t*)pAddress_8;

			int32_t iValue = ((*pAddress) & iOperand);
			
			STACK[--REGS.RSP] = iValue;
		}
		break;
		case OPCODE::STA:
		{
			// STA - Store Value in Accumulator(in our case, the STACK) to memory address
			int32_t iVariable = READ_OPERAND(eOpCode);		// Pointer Variable.

			int32_t iVarType = STACK[REGS.RSP++];				// Variable TYPE (int8_t = 1, int16_t = 2, int32_t = 4).
			int32_t iArrayIndex = STACK[REGS.RSP++];			// ArrayIndex.
			int32_t iRValue = STACK[REGS.RSP++];				// RValue to be stored, picked up from the STACK.

			int32_t iAddress = getAddressOf(iVariable);
			{
				int8_t* pAddress_8 = (int8_t*)&HEAP[iAddress];
				pAddress_8 += (iArrayIndex*iVarType);
				int32_t* pAddress = (int32_t*)pAddress_8;
				*pAddress = iRValue;

				printf("Stored %d @[%d]\n", iRValue, iArrayIndex);
			}
		}
		break;
		case OPCODE::CLR:
		{
			// arr[5..7] = 0; ------------ - (II)

			int32_t iOperand1_Variable				= READ_OPERAND(eOpCode);		// 1. "arr" position in heap
			int32_t iOperand2_ArrayIndex			= READ_OPERAND(eOpCode);		// 2. '5'	==> ArrayIndex.
			int32_t iOperand3_LastPos				= READ_OPERAND(eOpCode);		// 3. Count (in this case, 3 i.e for 5, 6, 7)
			int32_t iOperand4_RValue				= READ_OPERAND(eOpCode);		// 4. '0'	==> RValue to be stored
			int32_t iOperand5_VarType				= READ_OPERAND(eOpCode);		// 5. Cast Value of Type "arr" to perform relevant 'CAST'
			int32_t iOperand6_CastValue				= READ_OPERAND(eOpCode);		// 6. Variable TYPE(int8_t = 1, int16_t = 2, int32_t = 4).

			// Clear Memory
			{
				int32_t iAddress = getAddressOf(iOperand1_Variable);

				int8_t* pAddress_8 = (int8_t*)&HEAP[iAddress];
				pAddress_8 += (iOperand2_ArrayIndex * iOperand5_VarType);
				int32_t iCount = (iOperand3_LastPos - iOperand2_ArrayIndex);

				for (int32_t i = 1; i <= iCount; i++)
				{
					*pAddress_8 = (iOperand4_RValue & iOperand6_CastValue);
					pAddress_8 += iOperand5_VarType;
				}
			}
		}
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
		case OPCODE::MALLOC:
		{
			iTemp1 = STACK[REGS.RSP++];

			int32_t iAddress = malloc(iTemp1);
			assert(iAddress >= 0);
			printf("Allocated %d bytes @ %d\n", iTemp1, iAddress);

			STACK[--REGS.RSP] = iAddress;
		}
		break;
		case OPCODE::FREE:
		{
			int32_t iVariable = READ_OPERAND(eOpCode);
			{
				int32_t iAddress = getAddressOf(iVariable);
				dealloc(iAddress);
			}
		}
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

int32_t VirtualMachine::malloc(int32_t iSize)
{
	int32_t iReturnAddress = -1;
	std::vector<HeapNode>::iterator itrUnAllocList = m_vUnAllocatedList.begin();
	for (; itrUnAllocList != m_vUnAllocatedList.end(); ++itrUnAllocList)
	{
		HeapNode& pUnAllocHeapNode = *itrUnAllocList;
		if(iSize <= pUnAllocHeapNode.m_iSize)
		{
			iReturnAddress = pUnAllocHeapNode.m_pAddress;
			m_vAllocatedList.push_back(HeapNode(pUnAllocHeapNode.m_pAddress, iSize));
			
			if (iSize == pUnAllocHeapNode.m_iSize)
			{
				m_vUnAllocatedList.erase(itrUnAllocList);
			}
			else
			{
				pUnAllocHeapNode.m_pAddress += iSize;
				pUnAllocHeapNode.m_iSize -= iSize;
			}

			break;
		}
	}

	return iReturnAddress;
}

bool sortList(const HeapNode& first, const HeapNode& second)
{
	return (first.m_pAddress < second.m_pAddress);
}

void VirtualMachine::dealloc(int32_t pAddress)
{
	std::vector<HeapNode>::iterator itrAllocList = m_vAllocatedList.begin();

	for (; itrAllocList != m_vAllocatedList.end();  ++itrAllocList)
	{
		HeapNode& pAllocHeapNode = *itrAllocList;
		if (pAddress == pAllocHeapNode.m_pAddress)
		{
			bool bFoundPrecedingNode = false;
			for (HeapNode& pUnAllocHeapNode : m_vUnAllocatedList)
			{
				if (pUnAllocHeapNode.m_pAddress + pUnAllocHeapNode.m_iSize == pAddress)
				{
					bFoundPrecedingNode = true;
					pUnAllocHeapNode.m_iSize += pAllocHeapNode.m_iSize;
					break;
				}
			}

			{
				std::vector<HeapNode>::iterator itr = std::find_if(	m_vUnAllocatedList.begin(), 
																	m_vUnAllocatedList.end(), 
																	[pAllocHeapNode](HeapNode& pHeapNode) {
																		return (	pAllocHeapNode.m_pAddress >= pHeapNode.m_pAddress
																					&&
																					(pAllocHeapNode.m_pAddress + pAllocHeapNode.m_iSize) <= (pHeapNode.m_pAddress + pHeapNode.m_iSize)
																				);
																	});
				if (itr == m_vUnAllocatedList.end())
				{
					m_vUnAllocatedList.push_back(HeapNode(pAllocHeapNode.m_pAddress, pAllocHeapNode.m_iSize));
					std::sort(m_vUnAllocatedList.begin(), m_vUnAllocatedList.end(), sortList);
				}

				m_vAllocatedList.erase(itrAllocList);
			}

			break;
		}
	}
}

int32_t VirtualMachine::getAddressOf(int32_t iVariable)
{
	int32_t iAddress = 0;
	int16_t iVariablePos = (iVariable & 0x0000FFFF);
	E_VARIABLETYPE eVariableType = (E_VARIABLETYPE)((int32_t)iVariable >> (sizeof(int16_t) * 8));

	// Local Var or Function Argument ==> saved on the STACK
	if (eVariableType == E_VARIABLETYPE::ARGUMENT
		||
		eVariableType == E_VARIABLETYPE::LOCAL
	) {
		if (eVariableType == E_VARIABLETYPE::ARGUMENT)
			iVariablePos *= -1;

		iAddress = STACK[REGS.RBP - iVariablePos];
	}
	else // STATIC variable saved on the HEAP
	{
		iAddress = GLOBALS[iVariablePos];
	}

	return iAddress;
}

