#include "GrammerUtils.h"
#include <assert.h>

Token									GrammerUtils::m_pToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);
Token									GrammerUtils::m_pPrevToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);

std::vector<std::string>				GrammerUtils::m_vKeywords;

StringTokenizer*						GrammerUtils::m_pStrTok = NULL;
int										GrammerUtils::iTabCount = -1;
std::vector<std::string>				GrammerUtils::m_vVariables;
std::vector<std::string>				GrammerUtils::m_vStrings;
int										GrammerUtils::m_iByteCode[MAX_BYTECODE_SIZE];
std::map<std::string, FunctionInfo*>	GrammerUtils::m_MapFunctionInfos;
FunctionInfo*							GrammerUtils::m_pCurrentFunction;

#define VERBOSE							1
#define EMIT_1(__ICODE__, __OPERAND__)					emit(__ICODE__, (int)__OPERAND__, m_iByteCode, iOffset);
#define EMIT_2(__ICODE__, __OPERAND1__, __OPERAND2__)	emit(__ICODE__, (int)__OPERAND1__, (int)__OPERAND2__, m_iByteCode, iOffset);

#define EMIT(__ICODE__)					emit((int)__ICODE__, m_iByteCode, iOffset++);
#define EMIT_NOOFFSETINCR(__ICODE__)	emit((int)__ICODE__, m_iByteCode, iOffset);

CodeMap opCodeMap[] =
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

RegisterMap registerMap[]
{
	{ "RAX", EREGISTERS::RAX },	// Accumulator
	{ "RCX", EREGISTERS::RCX },	// Counter
	{ "RDX", EREGISTERS::RDX },	// Data
	{ "RBX", EREGISTERS::RBX },	// Base
	{ "RSP", EREGISTERS::RSP },	// Stack Pointer
	{ "RBP", EREGISTERS::RBP },	// Stack Base Pointer
	{ "RSI", EREGISTERS::RSI },	// Source
	{ "RDI", EREGISTERS::RDI },	// Destination

	{ "RMAX", EREGISTERS::RMAX },
};

void GrammerUtils::init()
{
}

bool GrammerUtils::read(const char* sFile)
{
	bool bSuccess = false;
	RandomAccessFile* pRaf = new RandomAccessFile();
	bSuccess = pRaf->openForRead(sFile);
	if (bSuccess)
	{
		unsigned long iLength = pRaf->length();
		char* sBuff = new char[iLength + strlen("\r\n") + 1];
		memset(sBuff, 0, iLength);

		unsigned long iBytesRead = pRaf->read(sBuff);
		if (iBytesRead > 0)
		{
			m_pStrTok = StringTokenizer::create(sBuff);
			m_pStrTok->tokenize();
		}

		delete[] sBuff;
		sBuff = nullptr;
	}
	pRaf->close();

	return bSuccess;
}

Token GrammerUtils::getNextToken()
{
	m_pToken = m_pStrTok->nextToken();

	if (m_pToken.getType() == TokenType::Type::TK_EOL
		||
		m_pToken.getType() == TokenType::Type::TK_SINGLELINECOMMENT
		||
		m_pToken.getType() == TokenType::Type::TK_MULTILINECOMMENT
		||
		m_pToken.getType() == TokenType::Type::TK_WHITESPACE
	) {
		getNextToken();
	}

	return m_pToken;
}

bool GrammerUtils::match(TokenType::Type eTokenType, int TYPE)
{
	TokenType::Type eCurrentTokenType = m_pToken.getType();

	bool bIsAMatch = (eCurrentTokenType == eTokenType && !isOneOfTheKeywords(m_pToken.getText()));
	if (bIsAMatch)
	{
		m_pPrevToken = m_pToken;
		getNextToken();
	}

	return bIsAMatch;
}

bool GrammerUtils::match(std::string sText, int TYPE)
{
	std::string sCurrentTokenText = m_pToken.getText();

	bool bIsAMatch = (sCurrentTokenText == sText);
	if (bIsAMatch)
	{
		m_pPrevToken = m_pToken;
		getNextToken();
	}

	return bIsAMatch;
}

bool GrammerUtils::match(char ch, int TYPE)
{
	return match(std::string(1, ch), TYPE);
}

bool GrammerUtils::isOneOfTheKeywords(std::string sKeyword)
{
	bool bYes = false;
	for (std::string sInBuiltKeyword : m_vKeywords)
	{
		if (sInBuiltKeyword == sKeyword)
		{
			bYes = true;
			break;
		}
	}

	return bYes;
}

void GrammerUtils::printTabs()
{
	if (iTabCount >= 0)
	{
		for (int i = 0; i <= iTabCount; i++)
			std::cout << "\t";
	}
}

void GrammerUtils::printAST(Tree* pNode)
{
	Tree* pLeftNode = pNode->m_pLeftNode;
	Tree* pRightNode = pNode->m_pRightNode;
	bool bProcessChildren = true;

	printTabs();
	switch (pNode->m_eASTNodeType)
	{
		case ASTNodeType::ASTNode_FUNCTIONDEF:
		{
			Tree* pReturnTypeNode = pNode->m_pLeftNode;
			Tree* pArgListNode = pNode->m_pRightNode;

			std::cout << pReturnTypeNode->m_sText << " " << pNode->m_sText << "(";

			// Arg List
			for (Tree* pArgNode : pArgListNode->m_vStatements)
			{
				if (pArgNode->m_eASTNodeType == ASTNodeType::ASTNode_PRIMITIVETYPEINT)
					std::cout << "int ";
				else
				if (pArgNode->m_eASTNodeType == ASTNodeType::ASTNode_PRIMITIVETYPESTRING)
					std::cout << "string ";

				std::cout << pArgNode->m_sText << ", ";
			}

			std::cout << ") {";
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_FUNCTIONCALL:
		{
			std::cout << pNode->m_sText << "(";

			std::vector<Tree*>* vStatements = &pNode->m_vStatements;
			for (Tree* pChildNode : *vStatements)
			{
				if(pChildNode->m_eASTNodeType != ASTNodeType::ASTNode_FUNCTIONCALLEND)
					std::cout << pChildNode->m_sText << ", ";
			}
			std::cout << ");";

			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_PRIMITIVETYPEINT:
		{
			std::cout << "int " << pNode->m_sText;
			std::cout << " = ";

			Tree* pExpressionNode = pNode->m_pLeftNode;

			if (pExpressionNode->m_vStatements.size() > 0)
			{
				for (Tree* pChild : pExpressionNode->m_vStatements)
				{
					std::cout << pChild->m_sText << " ";
				}
			}
			else
			if(pExpressionNode != nullptr)
				std::cout << pExpressionNode->m_sText;

			std::cout << ";";
		}
		break;
		case ASTNodeType::ASTNode_PRIMITIVETYPESTRING:
		{
			std::cout << "string " << pNode->m_sText;
			std::cout << " = ";

			Tree* pExpressionNode = pNode->m_pLeftNode;
			if (pExpressionNode != nullptr)
				std::cout << pExpressionNode->m_sText;

			std::cout << ";";
		}
		break;
		case ASTNodeType::ASTNode_ASSIGN:
		{
			Tree* pIdentifierNode = pRightNode;
			Tree* pExpressionNode = pLeftNode;

			std::cout << pIdentifierNode->m_sText << " = " << pExpressionNode->m_sText;

			std::cout << ";";
			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_IF:
		{
			std::cout << "if(" << pLeftNode->m_sText << ") {" << std::endl;
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_ELSE:
		{
			std::cout << "else" << "{\n";
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_WHILE:
		{
			std::cout << "while(" << pLeftNode->m_sText << ") {" << std::endl;
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_PRINT:
		{
			std::cout << "print(";
			std::vector<Tree*>* vStatements = &pNode->m_vStatements;
			for (Tree* pChildNode : *vStatements)
			{
				std::cout << pChildNode->m_sText << ", ";
			}
			std::cout << ");";

			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_PUTC:
		{
			std::cout << "putc(";
			std::vector<Tree*>* vStatements = &pNode->m_vStatements;
			for (Tree* pChildNode : *vStatements)
			{
				std::cout << pChildNode->m_sText;
			}
			std::cout << ");";

			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_STRING:
		{
			std::cout << pNode->m_sText << " ";
			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_EXPRESSION:
		{
			std::cout << pNode->m_sText << " ";
			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_RETURNSTMT:
		{
			std::cout << "return" << " ";

			std::vector<Tree*>* vStatements = &pNode->m_vStatements;
			for (Tree* pChildNode : *vStatements)
			{
				std::cout << pChildNode->m_sText << ", ";
			}
			std::cout << ";";

			bProcessChildren = false;
		}
		break;
	}

	if (bProcessChildren)
	{
		std::vector<Tree*>* vStatements = &pNode->m_vStatements;
		for (Tree* pChildNode : *vStatements)
		{
			printAST(pChildNode);
		}
	}

	switch (pNode->m_eASTNodeType)
	{
		case ASTNodeType::ASTNode_FUNCTIONDEF:
		{
			if (iTabCount >= 0)
				iTabCount--;

			printTabs();  std::cout << "}";
		}
		break;
		case ASTNodeType::ASTNode_IF:
		{
			if (iTabCount >= 0)
				iTabCount--;

			printTabs();  std::cout << "}" << std::endl;
			// Else
			if (pRightNode != nullptr)
				printAST(pRightNode);

			std::cout << std::endl;
		}
		break;
		case ASTNodeType::ASTNode_ELSE:
		{
			if (iTabCount >= 0)
				iTabCount--;

			printTabs(); std::cout << "}";
		}
		break;
		case ASTNodeType::ASTNode_WHILE:
		{
			if (iTabCount >= 0)
				iTabCount--;

			printTabs(); std::cout << "}";
		}
		break;
	}

	std::cout << std::endl;
}

void GrammerUtils::generateCode(Tree* pRootNode)
{
	printAST(pRootNode);

	populateStrings(pRootNode, m_vStrings);
	int iStrings = m_vStrings.size();

	int iOffset = 0;
	{
		//////////////////////////////////////////////////////////////////////////////
		// Call to main()
		int iJumpMainOffsetHole = 0;
		int iMainRetAddressHole = 0;
		{
			// RET
			EMIT(OPCODE::PUSHI);
			iMainRetAddressHole = iOffset++;

			// EBP
			EMIT(OPCODE::PUSHR);
			EMIT(EREGISTERS::RBP);

			// Currently main() does not take any arguments.
			// Else push arguments here...

			// Call to main(), offset populated later.
			EMIT(OPCODE::CALL);
#if (VERBOSE == 1)
			std::cout << (iOffset - 1) << ". " << "CALL ";
#endif
			iJumpMainOffsetHole = iOffset++;
		}

		//////////////////////////////////////////////////////////////////////////////
		populateCode(pRootNode, m_iByteCode, iOffset);
		//////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////
		EMIT_NOOFFSETINCR(OPCODE::HLT);
		std::cout << iOffset << ". " << "HLT" << std::endl;

		emit(iOffset, m_iByteCode, iMainRetAddressHole);
		//////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////
		// Map main() jump offset.
		std::map<std::string, FunctionInfo*>::const_iterator itrFuncMain = m_MapFunctionInfos.find("main");
		assert(itrFuncMain != m_MapFunctionInfos.end());
		if (itrFuncMain != m_MapFunctionInfos.end())
		{
			FunctionInfo* pMainFuncInfo = (FunctionInfo*)itrFuncMain->second;

			emit(pMainFuncInfo->m_iStartOffsetInCode, m_iByteCode, iJumpMainOffsetHole);
#if (VERBOSE == 1)
			std::cout << pMainFuncInfo->m_iStartOffsetInCode << std::endl;
#endif
		}
		//////////////////////////////////////////////////////////////////////////////

		iOffset = 0;
		printAssembly(m_iByteCode, iOffset, m_vVariables, m_vStrings);
	}
}

void GrammerUtils::populateStrings(Tree* pParentNode, std::vector<std::string>& sVector)
{
	if (pParentNode != nullptr)
	{
		std::vector<Tree*> vStatements = pParentNode->m_vStatements;
		for (Tree* pNode : vStatements)
		{
			switch (pNode->m_eASTNodeType)
			{
				case ASTNodeType::ASTNode_FUNCTIONDEF:
				case ASTNodeType::ASTNode_FUNCTIONCALL:
				case ASTNodeType::ASTNode_WHILE:
				case ASTNodeType::ASTNode_PRINT:
				{
					populateStrings(pNode, sVector);
				}
				break;
				case ASTNodeType::ASTNode_STRING:
				case ASTNodeType::ASTNode_CHARACTER:
				{
					addString(pNode->m_sText, sVector);
				}
				break;
				case ASTNodeType::ASTNode_IF:
				{
					populateStrings(pNode, sVector);
					Tree* pElseNode = pNode->m_pRightNode;
					if (pElseNode != nullptr)
					{
						populateStrings(pElseNode, sVector);
					}
				}
				break;
			}
		}
	}
}

void GrammerUtils::addString(std::string& sText, std::vector<std::string>& vVector)
{
	if (vVector.size() > 0)
	{
		bool bFound = false;
		for (std::string sStr : vVector)
		{
			if (sStr == sText)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			vVector.push_back(sText);
		}
	}
	else
		vVector.push_back(sText);
}

void GrammerUtils::populateCode(Tree* pNode, int* pByteCode, int& iOffset)
{
	if (pNode != nullptr)
	{
		int i_IfWhile_JCondition_Hole = 0, i_While_Loop_Hole = 0, i_ElseEnd_JMP_Offset = 0;

		// Prologues
		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_FUNCTIONDEF:
			{
				handleFunctionDef(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_FUNCTIONSTART:
			{
				handleFunctionStart(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_FUNCTIONEND:
			{
				handleFunctionEnd(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_FUNCTIONCALLEND:
			{
				handleFunctionCall(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_CHARACTER:
			{
				handleCharacter(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_INTEGER:
			{
				handleInteger(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_IDENTIFIER:
			{
				handleIdentifier(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_STRING:
			{
				handleString(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_EXPRESSION:
			{
				handleExpression(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_PRIMITIVETYPEINT:
			{
				handlePrimitiveInt(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_ASSIGN:
			{
				handleAssign(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_IF:
			case ASTNodeType::ASTNode_WHILE:
			{
				handleIfWhile_Prologue(pNode, pByteCode, iOffset, i_While_Loop_Hole, i_IfWhile_JCondition_Hole);
			}
			break;
		}

		// Intermediates
		handleStatements(pNode, pByteCode, iOffset);

		// Epilogues
		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_RETURNSTMT:
			{
				handleReturnStatement(pNode, pByteCode, iOffset);
			}
			break;
			case ASTNodeType::ASTNode_IF:
			{
				handleIf_Epilogue(pNode, pByteCode, iOffset, i_ElseEnd_JMP_Offset, i_IfWhile_JCondition_Hole);
			}
			break;
			case ASTNodeType::ASTNode_WHILE:
			{
				handleWhile_Epilogue(pNode, pByteCode, iOffset, i_While_Loop_Hole, i_IfWhile_JCondition_Hole);
			}
			break;
		}
	}
}

void GrammerUtils::emit(int iCode, int* pByteCode, int iOffset)
{
	if (iOffset < MAX_BYTECODE_SIZE)
	{
		pByteCode[iOffset] = iCode;
	}
}

void GrammerUtils::emit(OPCODE eOPCODE, int iOperand1, int iOperand2, int* pByteCode, int& iOffset)
{
	switch (eOPCODE)
	{
		case OPCODE::SUB_REG:
		{
			EMIT(eOPCODE);
			EMIT(iOperand1);
			EMIT(iOperand2);

			#if (VERBOSE == 1)
				std::cout << (iOffset - 3) << ". " << opCodeMap[(int)eOPCODE].sOpCode;
				std::cout << " " << registerMap[iOperand1].sRegister;
				std::cout << " " << iOperand2 << std::endl;
			#endif
		}
		break;
	}
}

void GrammerUtils::emit(OPCODE eOPCODE, int iOperand, int* pByteCode, int& iOffset)
{
	switch (eOPCODE)
	{
		case OPCODE::STORE:
		case OPCODE::FETCH:
		case OPCODE::PUSHI:
		{
			EMIT(eOPCODE);
			EMIT(iOperand);

			#if (VERBOSE == 1)
				std::cout << (iOffset - 1) << ". " << opCodeMap[(int)eOPCODE].sOpCode << " ";
				std::cout << iOperand << std::endl;
			#endif
		}
		break;
		case OPCODE::PUSHR:
		case OPCODE::POPR:
		{
			EMIT(eOPCODE);
			EMIT(iOperand);

			#if (VERBOSE == 1)
				std::cout << (iOffset - 1) << ". " << opCodeMap[(int)eOPCODE].sOpCode << " ";
				std::cout << registerMap[iOperand].sRegister << std::endl;
			#endif
		}
		break;
		case OPCODE::RET:
		{
			EMIT(eOPCODE);

			#if (VERBOSE == 1)
				std::cout << (iOffset - 1) << ". " << opCodeMap[(int)eOPCODE].sOpCode << std::endl << std::endl;
			#endif
		}
		break;
		case OPCODE::CALL:
		{
			EMIT(eOPCODE);
			EMIT(iOperand);

			#if (VERBOSE == 1)
				std::cout << (iOffset - 1) << ". " << opCodeMap[(int)eOPCODE].sOpCode << " ";
				std::cout << iOperand << std::endl;
			#endif
		}
		break;
		case OPCODE::ADD:
		case OPCODE::SUB:
		case OPCODE::MUL:
		case OPCODE::DIV:
		case OPCODE::MOD:
		case OPCODE::JMP_LT:
		case OPCODE::JMP_LTEQ:
		case OPCODE::JMP_GT:
		case OPCODE::JMP_GTEQ:
		case OPCODE::JMP_EQ:
		case OPCODE::JMP_NEQ:
		case OPCODE::LOGICALAND:
		case OPCODE::LOGICALOR:
		case OPCODE::_NOT:
		case OPCODE::NEGATE:
		case OPCODE::PRTC:
		case OPCODE::PRTI:
		case OPCODE::PRTS:
		{
			EMIT(eOPCODE);

			#if (VERBOSE == 1)
				std::cout << (iOffset - 1) << ". " << opCodeMap[(int)eOPCODE].sOpCode << std::endl;
			#endif
		}
		break;
	}
}

int GrammerUtils::getVariablePosition(const char* pIdentifier)
{
	if (pIdentifier != nullptr)
	{
		std::string str = pIdentifier;
		int iCount = 0;
		for (std::string sVar : m_vVariables)
		{
			if (sVar == str)
				return iCount;
			iCount++;
		}

		return -1;
	}
}

int	GrammerUtils::getStringPosition(const char* sString)
{
	if (sString != nullptr)
	{
		std::string str = sString;
		int iCount = 0;
		for (std::string sVar : m_vStrings)
		{
			if (sVar == str)
				return iCount;
			iCount++;
		}

		return -1;
	}
}

void GrammerUtils::handleFunctionDef(Tree* pNode, int* pByteCode, int& iOffset)
{
	std::string sFuncName = pNode->m_sText;
	Tree* pReturnTypeNode = pNode->m_pLeftNode;
	Tree* pArgListNode = pNode->m_pRightNode;

	m_pCurrentFunction = new FunctionInfo(pNode, iOffset);
	if (m_pCurrentFunction != nullptr)
	{
		m_MapFunctionInfos[sFuncName] = m_pCurrentFunction;
	}
}

void GrammerUtils::handleFunctionStart(Tree* pNode, int* pByteCode, int& iOffset)
{
	std::cout << m_pCurrentFunction->m_sFunctionName << ":" << std::endl;

	// Stack Frame: Subtract local variable count from ESP.
	EMIT_2(OPCODE::SUB_REG, EREGISTERS::RSP, -m_pCurrentFunction->getLocalVariableCount());
}

void GrammerUtils::handleFunctionEnd(Tree* pNode, int* pByteCode, int& iOffset)
{
	// Stack Frame: Add local variable count from ESP.
	EMIT_2(OPCODE::SUB_REG, EREGISTERS::RSP, m_pCurrentFunction->getLocalVariableCount());

	// Stack Frame: Add argument count from ESP.
	EMIT_2(OPCODE::SUB_REG, EREGISTERS::RSP, m_pCurrentFunction->getArgumentsCount());

	// Pop
	EMIT_1(OPCODE::POPR, EREGISTERS::RBP);
	
	// Ret
	EMIT_1(OPCODE::RET, 0);

	if (m_pCurrentFunction->m_pFunctionReturnType->m_sText != "void")
	{
		////// Push the already Popped Return Value in EAX onto the stack.
		EMIT_1(OPCODE::PUSHR, EREGISTERS::RAX);
	}
}

void GrammerUtils::handleFunctionCall(Tree* pNode, int* pByteCode, int& iOffset)
{
	// Callee function name
	std::string sFuncCallee = pNode->m_sText;

	// Check if the 'Callee' is our list !
	FunctionInfo* pCalleeFunctionInfo = m_MapFunctionInfos[sFuncCallee];

	assert(pCalleeFunctionInfo != nullptr);
	if (pCalleeFunctionInfo != nullptr)
	{
		// Get Actual Function Def details like argument Count & function signature.
		Tree* pFUNCDEF_ArgListNode = pCalleeFunctionInfo->m_pFunctionArguments;
		int iFUNCDEF_ArgCount = pFUNCDEF_ArgListNode->m_vStatements.size();
		std::string sFUNCDEF_Signature = pFUNCDEF_ArgListNode->m_sAdditionalInfo;

		// Get Callee Function Def details like argument Count & function signature.
		Tree* pCALLEE_Node = pNode->m_pParentNode;
		int iCALLEE_ArgCount = pCALLEE_Node->m_vStatements.size() - 1/*ASTNode_FUNCTIONCALLEND*/;
		std::string sCALLEE_Signature = pCALLEE_Node->m_sAdditionalInfo;

		// Check if the Callee meets Actual function requirements.
		// Else give a compilation error here.
		assert((iFUNCDEF_ArgCount == iCALLEE_ArgCount)
			&&
			(sFUNCDEF_Signature == sCALLEE_Signature));

		if (	(iFUNCDEF_ArgCount == iCALLEE_ArgCount)
				&&
				(sFUNCDEF_Signature == sCALLEE_Signature)
		) {
			/*
			//////////////////////////////////////////////////////
			//// STACK FRAME
			//////////////////////////////////////////////////////
			Local(N)	<-- ESP		EBP + N										|	- High Mem
			...						...											|
			Local(0)				EBP + 1										|
			ARG0		<--	EBP		EBP											|
			...						...											|
			ARGN		<--|		EBP - N										|	    /\
			|--		Function arguments. Last Arg 1st(R to L).	|		||
			EBP			<-- Old EBP												|		||
			RET			<-- Call Return Address									|	- Low Mem
			//////////////////////////////////////////////////////
			*/

			//////////////////////////////////////////////////////
			// RET
			EMIT(OPCODE::PUSHI);
			int iReturnAddressOffsetHole = iOffset++;

			// EBP
			EMIT_1(OPCODE::PUSHR, EREGISTERS::RBP);

			//////////////////////////////////////////////////////
			// If the Callee has arguments, they need to be pushed onto the stack.
			if (iCALLEE_ArgCount > 0)
			{
				// 1. Push arguments onto the stack...
				std::vector<Tree*>::reverse_iterator rItr = pCALLEE_Node->m_vStatements.rbegin();
				for (; rItr != pCALLEE_Node->m_vStatements.rend(); ++rItr)
				{
					Tree* pArgNode = *rItr;
					if (pArgNode->m_eASTNodeType != ASTNodeType::ASTNode_FUNCTIONCALLEND)
					{
						populateCode(pArgNode, pByteCode, iOffset);
					}
				}
			}

			//////////////////////////////////////////////////////
			// Call
			int iCalleeFunctionAddress = pCalleeFunctionInfo->m_iStartOffsetInCode;

			EMIT_1(OPCODE::CALL, iCalleeFunctionAddress);

			//////////////////////////////////////////////////////
			// Patch the return address
			emit(iOffset, m_iByteCode, iReturnAddressOffsetHole);

			#if (VERBOSE == 1)
				std::cout << "------" << "iReturnAddressOffsetHole [" << iReturnAddressOffsetHole << "] = " << iOffset << std::endl;
			#endif
		}
	}
}

void GrammerUtils::handlePreFixExpression(Tree* pPreFixNode, int* pByteCode, int& iOffset)
{
	if (pPreFixNode != nullptr)
	{
		for (Tree* pChild : pPreFixNode->m_vStatements)
		{
			ASTNodeType eASTNodeType = pChild->m_eASTNodeType;
			switch (eASTNodeType)
			{
				case ASTNodeType::ASTNode_PREDECR:
				case ASTNodeType::ASTNode_PREINCR:
				{
					/////////////////////////////////////////////////
					// 1. Fetch variable value & store it onto the stack
					EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(pChild->m_sText.c_str()));

					/////////////////////////////////////////////////
					// 2. Push integer 1.
					EMIT_1(OPCODE::PUSHI, (eASTNodeType == ASTNodeType::ASTNode_PREINCR) ? 1 : -1);

					/////////////////////////////////////////////////
					// 3. Add the 2 operands
					EMIT_1(OPCODE::ADD, 0);

					//////////////////////////////////////////////////////
					// Store the decremented/incremented value from the stack back to the variable
					EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pChild->m_sText.c_str()));
				}
				break;
			}
		}
	}
}

void GrammerUtils::handleExpression(Tree* pNode, int* pByteCode, int& iOffset)
{
	std::string sRValuePostFixExpression = pNode->m_sText;
	StringTokenizer* st = StringTokenizer::create(sRValuePostFixExpression.c_str());
	st->tokenize();
	while (st->hasMoreTokens())
	{
		Token prevTok = st->prevToken();
		Token tok = st->nextToken();

		TokenType::Type eCurrTokenType = tok.getType();
		if (eCurrTokenType == TokenType::Type::TK_WHITESPACE || eCurrTokenType == TokenType::Type::TK_EOI || eCurrTokenType == TokenType::Type::TK_COMMA)
			continue;

		switch (eCurrTokenType)
		{
			case TokenType::Type::TK_CHARACTER:
				EMIT_1(OPCODE::PUSHI, atoi(tok.getText()));
			break;
			case TokenType::Type::TK_INTEGER:
				EMIT_1(OPCODE::PUSHI, atoi(tok.getText()));
			break;
			case TokenType::Type::TK_IDENTIFIER:
				EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(tok.getText()));
			break;
			case TokenType::Type::TK_STRING:
				EMIT_1(OPCODE::PUSHI, getStringPosition(tok.getText()));
			break;
			case TokenType::Type::TK_MUL:
				EMIT_1(OPCODE::MUL, 0);
			break;
			case TokenType::Type::TK_DIV:
				EMIT_1(OPCODE::DIV, 0);
			break;
			case TokenType::Type::TK_MOD:
				EMIT_1(OPCODE::MOD, 0);
			break;
			case TokenType::Type::TK_ADD:
				EMIT_1(OPCODE::ADD, 0);
			break;
			case TokenType::Type::TK_SUB:
				EMIT_1(OPCODE::SUB, 0);
			break;
			case TokenType::Type::TK_LT:
				EMIT_1(OPCODE::JMP_LT, 0);
			break;
			case TokenType::Type::TK_LTEQ:
				EMIT_1(OPCODE::JMP_LTEQ, 0);
			break;
			case TokenType::Type::TK_GT:
				EMIT_1(OPCODE::JMP_GT, 0);
			break;
			case TokenType::Type::TK_GTEQ:
				EMIT_1(OPCODE::JMP_GTEQ, 0);
			break;
			case TokenType::Type::TK_EQ:
				EMIT_1(OPCODE::JMP_EQ, 0);
			break;
			case TokenType::Type::TK_NEQ:
				EMIT_1(OPCODE::JMP_NEQ, 0);
			break;
			case TokenType::Type::TK_LOGICALAND:
				EMIT_1(OPCODE::LOGICALAND, 0);
			break;
			case TokenType::Type::TK_LOGICALOR:
				EMIT_1(OPCODE::LOGICALOR, 0);
			break;
			case TokenType::Type::TK_NOT:
				EMIT_1(OPCODE::_NOT, 0);
			break;
			case TokenType::Type::TK_NEGATE:
				EMIT_1(OPCODE::NEGATE, 0);
			break;
		}
	}
}

void GrammerUtils::handlePostFixExpression(Tree* pPostFixNode, int* pByteCode, int& iOffset)
{
	if (pPostFixNode != nullptr)
	{
		for (Tree* pChild : pPostFixNode->m_vStatements)
		{
			ASTNodeType eASTNodeType = pChild->m_eASTNodeType;
			switch (eASTNodeType)
			{
			case ASTNodeType::ASTNode_POSTDECR:
			case ASTNodeType::ASTNode_POSTINCR:
			{
				/////////////////////////////////////////////////
				// 1. Fetch variable value & store it onto the stack
				EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(pChild->m_sText.c_str()));

				/////////////////////////////////////////////////
				// 2. Push integer 1.
				EMIT_1(OPCODE::PUSHI, (eASTNodeType == ASTNodeType::ASTNode_POSTINCR) ? 1 : -1);

				/////////////////////////////////////////////////
				// 3. Add the 2 operands
				EMIT_1(OPCODE::ADD, 0);

				//////////////////////////////////////////////////////
				// Store the decremented/incremented value from the stack back to the variable
				EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pChild->m_sText.c_str()));
			}
			break;
			}
		}
	}
}

void GrammerUtils::handleCharacter(Tree* pNode, int* pByteCode, int& iOffset)
{
	char pStr[255] = { 0 };
	sprintf_s(pStr, "%d", pNode->m_sText.c_str()[0]);
	int ch = atoi(pStr);

	EMIT_1(OPCODE::PUSHI, ch);
}

void GrammerUtils::handleInteger(Tree* pNode, int* pByteCode, int& iOffset)
{
	EMIT_1(OPCODE::PUSHI, atoi(pNode->m_sText.c_str()));
}

void GrammerUtils::handleIdentifier(Tree* pNode, int* pByteCode, int& iOffset)
{
	EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handleString(Tree* pNode, int* pByteCode, int& iOffset)
{
	EMIT_1(OPCODE::PUSHI, getStringPosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handlePrimitiveInt(Tree* pNode, int* pByteCode, int& iOffset)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
													// In case of ASSIGN, right node will be the lvalue.

	if (pExpressionNode->m_vStatements.size() > 0)
	{
		for (Tree* pChild : pExpressionNode->m_vStatements)
		{
			populateCode(pChild, pByteCode, iOffset);
		}

		EMIT_1(OPCODE::PUSHR, EREGISTERS::RAX);
	}
	else
		populateCode(pExpressionNode, pByteCode, iOffset);

	EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handleAssign(Tree* pNode, int* pByteCode, int& iOffset)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
													// In case of ASSIGN, right node will be the lvalue.

	// Compute PreFixExpr
	{
		handlePreFixExpression(pExpressionNode->m_pLeftNode, pByteCode, iOffset);
	}

	populateCode(pExpressionNode, pByteCode, iOffset);

	EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pNode->m_sText.c_str()));

	// Compute PreFixExpr
	{
		handlePostFixExpression(pExpressionNode->m_pRightNode, pByteCode, iOffset);
	}
}

void GrammerUtils::handleReturnStatement(Tree* pNode, int* pByteCode, int& iOffset)
{
	///////////////////////////////////////////////////////////////
	// The expression result is already pushed onto the stack !
	// Need to retrieve the result & save it in EAX.
	EMIT_1(OPCODE::POPR, EREGISTERS::RAX);
}

void GrammerUtils::handleIfWhile_Prologue(Tree* pNode, int* pByteCode, int& iOffset, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
	i_While_Loop_Hole = iOffset;

	populateCode(pExpressionNode, pByteCode, iOffset);

	EMIT(OPCODE::JZ);
	#if (VERBOSE == 1)
		std::cout << (iOffset - 1) << ". " << "JZ" << std::endl;
	#endif
	i_IfWhile_JCondition_Hole = iOffset++;
}

void GrammerUtils::handleIf_Epilogue(Tree* pNode, int* pByteCode, int& iOffset, int& i_ElseEnd_JMP_Offset, int& i_IfWhile_JCondition_Hole)
{
	Tree* pElseNode = pNode->m_pRightNode;
	EMIT(OPCODE::JMP);
	#if (VERBOSE == 1)
		std::cout << (iOffset - 1) << ". " << "JMP" << std::endl;
	#endif
	i_ElseEnd_JMP_Offset = iOffset++;

	emit(iOffset, m_iByteCode, i_IfWhile_JCondition_Hole);
	#if (VERBOSE == 1)
		std::cout << "------" << "i_IfWhile_JCondition_Hole [" << i_IfWhile_JCondition_Hole << "] = " << iOffset << std::endl;
	#endif
	if (pElseNode != nullptr)
	{
		populateCode(pElseNode, pByteCode, iOffset);
	}

	emit(iOffset, m_iByteCode, i_ElseEnd_JMP_Offset);
	#if (VERBOSE == 1)
		std::cout << "------" << "i_ElseEnd_JMP_Offset [" << i_ElseEnd_JMP_Offset << "] = " << iOffset << std::endl;
	#endif
}

void GrammerUtils::handleWhile_Epilogue(Tree* pNode, int* pByteCode, int& iOffset, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole)
{
	EMIT(OPCODE::JMP);
	#if (VERBOSE == 1)
		std::cout << (iOffset - 1) << ". " << "JMP" << std::endl;
	#endif

	emit(i_While_Loop_Hole, m_iByteCode, iOffset++);
	emit(iOffset, m_iByteCode, i_IfWhile_JCondition_Hole);
	#if (VERBOSE == 1)
		std::cout << "------" << "i_While_Loop_Hole [" << (iOffset - 1) << "] = " << i_While_Loop_Hole << std::endl;
		std::cout << "------" << "i_IfWhile_JCondition_Hole [" << i_IfWhile_JCondition_Hole << "] = " << iOffset << std::endl;
	#endif
}

void GrammerUtils::handleStatements(Tree* pNode, int* pByteCode, int& iOffset)
{
	std::vector<Tree*> vStatements = pNode->m_vStatements;
	for (Tree* pChildNode : vStatements)
	{
		////////////////////////////////////////////////////////////////////////////
		if (pNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONCALL)
		{
			// Bail out for the function arguments.
			// They will be processed in 'ASTNode_FUNCTIONCALLEND'
			if(pChildNode->m_eASTNodeType != ASTNodeType::ASTNode_FUNCTIONCALLEND)
				continue;
		}
		////////////////////////////////////////////////////////////////////////////

		ASTNodeType eASTNodeType = pNode->m_eASTNodeType;
		populateCode(pChildNode, pByteCode, iOffset);

		switch (eASTNodeType)
		{
			case ASTNodeType::ASTNode_PUTC:
			{
				EMIT_1(OPCODE::PRTC, 0);
			}
			break;
			case ASTNodeType::ASTNode_PRINT:
			{
				std::vector<Tree*> vStatements = pNode->m_vStatements;
				switch (pChildNode->m_eASTNodeType)
				{
					case ASTNodeType::ASTNode_INTEGER:
					case ASTNodeType::ASTNode_IDENTIFIER:
					case ASTNodeType::ASTNode_EXPRESSION:
						EMIT_1(OPCODE::PRTI, 0);
					break;
					case ASTNodeType::ASTNode_STRING:
						EMIT_1(OPCODE::PRTS, 0);
					break;
					case ASTNodeType::ASTNode_CHARACTER:
						EMIT_1(OPCODE::PRTC, 0);
					break;
				}
			}
			break;
		}
	}
}

void GrammerUtils::printHeaders(RandomAccessFile* pRaf, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings)
{
	pRaf->writeByte(vStrings.size());
	for (std::string sString : vStrings)
	{
		pRaf->writeByte(sString.length());
		pRaf->write(sString.c_str());
	}
}

void GrammerUtils::printAssembly(int* iByteCode, int iOffset, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings)
{
	RandomAccessFile* pRaf = new RandomAccessFile();
	bool bCanWrite = pRaf->openForWrite("main.o");

	if (bCanWrite)
	{
		printHeaders(pRaf, vVariables, vStrings);
	}

	OPCODE eOpcode = OPCODE::NOP;
	while (true)
	{
		eOpcode = (OPCODE)iByteCode[iOffset];
		CodeMap pMachineInstruction = opCodeMap[(int)eOpcode];

		// Print to stdout
		{
			std::cout << iOffset << ". " << pMachineInstruction.sOpCode;

			for(int i = 1; i < pMachineInstruction.iOpcodeOperandCount; i++)
			{
				std::cout << " " << iByteCode[iOffset + i];
			}
			std::cout << std::endl;
		}

		// Print to file
		if(bCanWrite)
		{
			pRaf->writeInt((int)pMachineInstruction.eOpCode);
			for (int i = 1; i < pMachineInstruction.iOpcodeOperandCount; i++)
			{
				pRaf->writeInt(iByteCode[iOffset + i]);
			}
		}

		iOffset += pMachineInstruction.iOpcodeOperandCount;

		if (eOpcode == OPCODE::HLT)
			break;
	}

	pRaf->close();
}
