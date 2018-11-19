#include "GrammerUtils.h"
#include "ByteArrayOutputStream.h"
#include "ByteArrayInputStream.h"
#include <assert.h>

Token									GrammerUtils::m_pToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);
Token									GrammerUtils::m_pPrevToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);

std::vector<std::string>				GrammerUtils::m_vKeywords;

StringTokenizer*						GrammerUtils::m_pStrTok = NULL;
int										GrammerUtils::iTabCount = 0;
std::vector<std::string>				GrammerUtils::m_vVariables;
std::vector<std::string>				GrammerUtils::m_vStrings;
int8_t									GrammerUtils::m_iByteCode[MAX_BYTECODE_SIZE];
std::map<std::string, FunctionInfo*>	GrammerUtils::m_MapFunctionInfos;
FunctionInfo*							GrammerUtils::m_pCurrentFunction;
ByteArrayOutputStream*					GrammerUtils::m_pBAOS;
ByteArrayInputStream*					GrammerUtils::m_pBAIS;

#define VERBOSE	1
#define EMIT_1(__ICODE__, __OPERAND__)					emit(__ICODE__, (int)__OPERAND__);
#define EMIT_2(__ICODE__, __OPERAND1__, __OPERAND2__)	emit(__ICODE__, (int)__OPERAND1__, (int)__OPERAND2__);

#define EMIT(__ICODE__)									emit((int)__ICODE__);
#define EMIT_NOOFFSETINCR(__ICODE__)					emit((int)__ICODE__);

#define EMIT_BYTE(__ICODE__)							emitByte((int8_t)__ICODE__);
#define EMIT_INT(__ICODE__)								emitInt((int32_t)__ICODE__);

#define EMIT_INT_ATPOS(__ICODE__, __IOFFSET__)			emitIntAtPos((int32_t)__ICODE__, __IOFFSET__);

#define CURRENT_OFFSET									m_pBAOS->getCurrentOffset()
#define CURRENT_OFFSET_READ								m_pBAIS->getCurrentOffset()

CodeMap opCodeMap[] =
{
	{ "NOP",		OPCODE::NOP,		1,  PRIMIIVETYPE::INT_8},
	{ "FETCH",		OPCODE::FETCH,		2,  PRIMIIVETYPE::INT_32},
	{ "STORE",		OPCODE::STORE,		2,  PRIMIIVETYPE::INT_32},
	{ "PUSH",		OPCODE::PUSH,		2,  PRIMIIVETYPE::INT_32},
	{ "POP",		OPCODE::POP,		2,  PRIMIIVETYPE::INT_32},
	{ "MUL",		OPCODE::MUL,		1,  PRIMIIVETYPE::INT_8},
	{ "DIV",		OPCODE::DIV,		1,  PRIMIIVETYPE::INT_8},
	{ "MOD",		OPCODE::MOD,		1,  PRIMIIVETYPE::INT_8},
	{ "ADD",		OPCODE::ADD,		1,  PRIMIIVETYPE::INT_8},
	{ "SUB",		OPCODE::SUB,		1,  PRIMIIVETYPE::INT_8},
	{ "JMP_LT",		OPCODE::JMP_LT,		1,  PRIMIIVETYPE::INT_8},
	{ "JMP_LTEQ",	OPCODE::JMP_LTEQ,	1,  PRIMIIVETYPE::INT_8},
	{ "JMP_GT",		OPCODE::JMP_GT,		1,  PRIMIIVETYPE::INT_8},
	{ "JMP_GTEQ",	OPCODE::JMP_GTEQ,	1,  PRIMIIVETYPE::INT_8},
	{ "JMP_EQ",		OPCODE::JMP_EQ,		1,  PRIMIIVETYPE::INT_8},
	{ "JMP_NEQ",	OPCODE::JMP_NEQ,	1,  PRIMIIVETYPE::INT_8},
	{ "LOGICALOR",	OPCODE::LOGICALOR,	1,  PRIMIIVETYPE::INT_8},
	{ "LOGICALAND",	OPCODE::LOGICALAND,	1,  PRIMIIVETYPE::INT_8},
	{ "NOT",		OPCODE::_NOT,		1,  PRIMIIVETYPE::INT_8},
	{ "JMP",		OPCODE::JMP,		2,  PRIMIIVETYPE::INT_32},
	{ "JZ",			OPCODE::JZ,			2,  PRIMIIVETYPE::INT_32},
	{ "JNZ",		OPCODE::JNZ,		2,  PRIMIIVETYPE::INT_32},
	{ "PRTS",		OPCODE::PRTS,		1,  PRIMIIVETYPE::INT_8},
	{ "PRTC",		OPCODE::PRTC,		1,  PRIMIIVETYPE::INT_8},
	{ "PRTI",		OPCODE::PRTI,		1,  PRIMIIVETYPE::INT_8},
	{ "CALL",		OPCODE::CALL,		2,  PRIMIIVETYPE::INT_32},
	{ "RET",		OPCODE::RET,		1,  PRIMIIVETYPE::INT_8},
	{ "SUB_REG",	OPCODE::SUB_REG,	3,  PRIMIIVETYPE::INT_8},

	{ "PUSHI",		OPCODE::PUSHI,		2,  PRIMIIVETYPE::INT_32},
	{ "PUSHR",		OPCODE::PUSHR,		2,  PRIMIIVETYPE::INT_8},
	{ "POPI",		OPCODE::POPI,		2,  PRIMIIVETYPE::INT_8},
	{ "POPR",		OPCODE::POPR,		2,  PRIMIIVETYPE::INT_8},
	{ "NEGATE",		OPCODE::NEGATE,		1,  PRIMIIVETYPE::INT_8},

	{ "HLT",		OPCODE::HLT,		1,  PRIMIIVETYPE::INT_8},
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
	if (iTabCount > 0)
	{
		for (int i = 0; i < iTabCount; i++)
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

			std::cout << ") {" << std::endl;
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
		case ASTNodeType::ASTNode_PREDECR:
		case ASTNodeType::ASTNode_PREINCR:
		{
			std::cout << ((pNode->m_eASTNodeType == ASTNodeType::ASTNode_PREDECR) ? "--" : "++") << pNode->m_sText << ";";
		}
		break;
		case ASTNodeType::ASTNode_POSTDECR:
		case ASTNodeType::ASTNode_POSTINCR:
		{
			std::cout << pNode->m_sText << ((pNode->m_eASTNodeType == ASTNodeType::ASTNode_POSTDECR) ? "--" : "++") << ";";
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

			// PreFix
			if (pExpressionNode->m_pLeftNode != nullptr)
			{
				for (Tree* pChildNode : pExpressionNode->m_pLeftNode->m_vStatements)
				{
					printAST(pChildNode);
				}
			}

			std::cout << pIdentifierNode->m_sText << " = " << pExpressionNode->m_sText << ";" << std::endl;

			// PostFix
			if (pExpressionNode->m_pRightNode != nullptr)
			{
				for (Tree* pChildNode : pExpressionNode->m_pRightNode->m_vStatements)
				{
					printAST(pChildNode);
				}
			}

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
			Tree* pExpressionNode = pLeftNode;

			// PreFix
			if (pExpressionNode->m_pLeftNode != nullptr)
			{
				for (Tree* pChildNode : pExpressionNode->m_pLeftNode->m_vStatements)
				{
					printAST(pChildNode);
				}
			}

			std::cout << "while(" << pLeftNode->m_sText << ") {" << std::endl;
			
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_SWITCH:
		{
			std::cout << "switch(" << pNode->m_pLeftNode->m_sText << ") {" << std::endl;
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_SWITCHCASE:
		{
			std::cout << "case " << pNode->m_sText << ":" << std::endl;
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_SWITCHDEFAULT:
		{
			std::cout << "default:" << std::endl;
			iTabCount++;
		}
		break;
		case ASTNodeType::ASTNode_SWITCHBREAK:
		{
			std::cout << "break;";
			bProcessChildren = false;
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
			Tree* pExpressionNode = pLeftNode;

			// PostFix
			if (pExpressionNode->m_pRightNode != nullptr)
			{
				for (Tree* pChildNode : pExpressionNode->m_pRightNode->m_vStatements)
				{
					printAST(pChildNode);
				}
			}

			if (iTabCount >= 0)
				iTabCount--;

			printTabs(); std::cout << "}";
		}
		break;
		case ASTNodeType::ASTNode_SWITCH:
		{
			if (iTabCount >= 0)
				iTabCount--;

			printTabs(); std::cout << "}";
		}
		break;
		case ASTNodeType::ASTNode_SWITCHCASE:
		{
			if (iTabCount >= 0)
				iTabCount--;
		}
		break;
		case ASTNodeType::ASTNode_SWITCHDEFAULT:
		{
			printTabs();
			std::cout << "break;" << std::endl;

			if (iTabCount >= 0)
				iTabCount--;
		}
		break;
	}

	std::cout << std::endl;
}

void GrammerUtils::generateCode(Tree* pRootNode)
{
	m_pBAOS = new ByteArrayOutputStream(m_iByteCode, MAX_BYTECODE_SIZE);
	m_pBAIS = new ByteArrayInputStream(m_iByteCode, MAX_BYTECODE_SIZE);

	printAST(pRootNode);

	populateStrings(pRootNode, m_vStrings);
	int iStrings = m_vStrings.size();

	int iOffset = 0;
	{
		//////////////////////////////////////////////////////////////////////////////
		// Call to main()
		uint32_t iJumpMainOffsetHole = 0;
		uint32_t iMainRetAddressHole = 0;
		{
			// RET
			EMIT_BYTE(OPCODE::PUSHI);
			iMainRetAddressHole = CURRENT_OFFSET;
			EMIT_INT(0);

			// EBP
			EMIT_BYTE(OPCODE::PUSHR);
			EMIT_BYTE(EREGISTERS::RBP);

			// Currently main() does not take any arguments.
			// Else push arguments here...

			// Call to main(), offset populated later.
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << "CALL ";
#endif
			EMIT_BYTE(OPCODE::CALL);

			iJumpMainOffsetHole = CURRENT_OFFSET;
			EMIT_INT(0);
		}

		//////////////////////////////////////////////////////////////////////////////
		populateCode(pRootNode);
		//////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////
		EMIT_INT_ATPOS(CURRENT_OFFSET, iMainRetAddressHole);//emit(iOffset, iMainRetAddressHole);

		std::cout << CURRENT_OFFSET << ". " << "HLT" << std::endl;
		EMIT_BYTE(OPCODE::HLT);
		//////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////
		// Map main() jump offset.
		std::map<std::string, FunctionInfo*>::const_iterator itrFuncMain = m_MapFunctionInfos.find("main");
		assert(itrFuncMain != m_MapFunctionInfos.end());
		if (itrFuncMain != m_MapFunctionInfos.end())
		{
			FunctionInfo* pMainFuncInfo = (FunctionInfo*)itrFuncMain->second;

			EMIT_INT_ATPOS(pMainFuncInfo->m_iStartOffsetInCode, iJumpMainOffsetHole);
#if (VERBOSE == 1)
			std::cout << pMainFuncInfo->m_iStartOffsetInCode << std::endl;
#endif
		}
		//////////////////////////////////////////////////////////////////////////////

		printAssembly(m_iByteCode, m_vVariables, m_vStrings);
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
				case ASTNodeType::ASTNode_SWITCH:
				{
					for (Tree* pSwitchCaseNode : pNode->m_vStatements)
					{
						populateStrings(pSwitchCaseNode, sVector);
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

void GrammerUtils::populateCode(Tree* pNode)
{
	if (pNode != nullptr)
	{
		bool bProcessStatements = true;

		// Prologues
		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_FUNCTIONDEF:
			{
				handleFunctionDef(pNode);
			}
			break;
			case ASTNodeType::ASTNode_FUNCTIONSTART:
			{
				handleFunctionStart(pNode);
			}
			break;
			case ASTNodeType::ASTNode_FUNCTIONEND:
			{
				handleFunctionEnd(pNode);
			}
			break;
			case ASTNodeType::ASTNode_FUNCTIONCALLEND:
			{
				handleFunctionCall(pNode);
			}
			break;
			case ASTNodeType::ASTNode_PREDECR:
			case ASTNodeType::ASTNode_PREINCR:
			{
				handlePreFixExpression(pNode);
			}
			break;
			case ASTNodeType::ASTNode_POSTDECR:
			case ASTNodeType::ASTNode_POSTINCR:
			{
				handlePostFixExpression(pNode);
			}
			break;
			case ASTNodeType::ASTNode_CHARACTER:
			{
				handleCharacter(pNode);
			}
			break;
			case ASTNodeType::ASTNode_INTEGER:
			{
				handleInteger(pNode);
			}
			break;
			case ASTNodeType::ASTNode_IDENTIFIER:
			{
				handleIdentifier(pNode);
			}
			break;
			case ASTNodeType::ASTNode_STRING:
			{
				handleString(pNode);
			}
			break;
			case ASTNodeType::ASTNode_EXPRESSION:
			{
				handleExpression(pNode);
			}
			break;
			case ASTNodeType::ASTNode_PRIMITIVETYPEINT:
			{
				handlePrimitiveInt(pNode);
			}
			break;
			case ASTNodeType::ASTNode_ASSIGN:
			{
				handleAssign(pNode);
			}
			break;
			case ASTNodeType::ASTNode_IF:
			case ASTNodeType::ASTNode_WHILE:
			{
				handleIfWhile(pNode);
				bProcessStatements = false;
			}
			break;
			case ASTNodeType::ASTNode_SWITCH:
			{
				handleSwitch(pNode);
				bProcessStatements = false;					// Since we are processing the child statements in "handleSwitch()"
			}
			break;
		}

		if (bProcessStatements)
		{
			// Intermediates
			handleStatements(pNode);
		}

		// Epilogues
		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_RETURNSTMT:
			{
				handleReturnStatement(pNode);
			}
			break;
		}
	}
}

void GrammerUtils::emitByte(int8_t iCode)
{
	m_pBAOS->writeByte(iCode);
}

void GrammerUtils::emitInt(int32_t iCode)
{
	m_pBAOS->writeInt(iCode);
}

void GrammerUtils::emitIntAtPos(int32_t iCode, uint32_t iOffset)
{
	m_pBAOS->writeIntAtPos(iCode, iOffset);
}

void GrammerUtils::emit(OPCODE eOPCODE, int iOperand1, int iOperand2)
{
	switch (eOPCODE)
	{
		case OPCODE::SUB_REG:
		{
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode;
			std::cout << " " << registerMap[iOperand1].sRegister;
			std::cout << " " << iOperand2 << std::endl;
#endif
			EMIT_BYTE(eOPCODE);
			EMIT_BYTE(iOperand1);
			EMIT_BYTE(iOperand2);
		}
		break;
	}
}

void GrammerUtils::emit(OPCODE eOPCODE, int iOperand)
{
	switch (eOPCODE)
	{
		case OPCODE::STORE:
		case OPCODE::FETCH:
		case OPCODE::PUSHI:
		{
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode << " ";
			std::cout << iOperand << std::endl;
#endif
			EMIT_BYTE(eOPCODE);
			EMIT_INT(iOperand);
		}
		break;
		case OPCODE::PUSHR:
		case OPCODE::POPR:
		{
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode << " ";
			std::cout << registerMap[iOperand].sRegister << std::endl;
#endif
			EMIT_BYTE(eOPCODE);
			EMIT_BYTE(iOperand);
		}
		break;
		case OPCODE::RET:
		{
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode << std::endl << std::endl;
#endif
			EMIT_BYTE(eOPCODE);
		}
		break;
		case OPCODE::CALL:
		{
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode << " ";
			std::cout << iOperand << std::endl;
#endif
			EMIT_BYTE(eOPCODE);
			EMIT_INT(iOperand);
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
#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode << std::endl;
#endif
			EMIT_BYTE(eOPCODE);
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

void GrammerUtils::handleFunctionDef(Tree* pNode)
{
	std::string sFuncName = pNode->m_sText;
	Tree* pReturnTypeNode = pNode->m_pLeftNode;
	Tree* pArgListNode = pNode->m_pRightNode;

	m_pCurrentFunction = new FunctionInfo(pNode, CURRENT_OFFSET);
	if (m_pCurrentFunction != nullptr)
	{
		m_MapFunctionInfos[sFuncName] = m_pCurrentFunction;
	}
}

void GrammerUtils::handleFunctionStart(Tree* pNode)
{
	std::cout << m_pCurrentFunction->m_sFunctionName << ":" << std::endl;

	// Stack Frame: Subtract local variable count from ESP.
	EMIT_2(OPCODE::SUB_REG, EREGISTERS::RSP, -m_pCurrentFunction->getLocalVariableCount());
}

void GrammerUtils::handleFunctionEnd(Tree* pNode)
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

void GrammerUtils::handleFunctionCall(Tree* pNode)
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
			|--		Function arguments. Last Arg 1st(R to L).					|		||
			EBP			<-- Old EBP												|		||
			RET			<-- Call Return Address									|	- Low Mem
			//////////////////////////////////////////////////////
			*/

			//////////////////////////////////////////////////////
			// RET
			EMIT_BYTE(OPCODE::PUSHI);
			int iReturnAddressOffsetHole = CURRENT_OFFSET;
			EMIT_INT(0);

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
						populateCode(pArgNode);
					}
				}
			}

			//////////////////////////////////////////////////////
			// Call
			int iCalleeFunctionAddress = pCalleeFunctionInfo->m_iStartOffsetInCode;

			EMIT_1(OPCODE::CALL, iCalleeFunctionAddress);

			//////////////////////////////////////////////////////
			// Patch the return address
			EMIT_INT_ATPOS(CURRENT_OFFSET, iReturnAddressOffsetHole);

			#if (VERBOSE == 1)
				std::cout << "------" << "iReturnAddressOffsetHole [" << iReturnAddressOffsetHole << "] = " << CURRENT_OFFSET << std::endl;
			#endif
		}
	}
}

void GrammerUtils::handlePreFixExpression(Tree* pPreFixNode)
{
	if (pPreFixNode != nullptr)
	{
		bool bAddCode = false;
		ASTNodeType eASTNodeType = pPreFixNode->m_eASTNodeType;
		switch (eASTNodeType)
		{
			case ASTNodeType::ASTNode_EXPRESSION_PREFIX:
			{
				for (Tree* pChild : pPreFixNode->m_vStatements)
				{
					handlePreFixExpression(pChild);
				}
			}
			break;
			case ASTNodeType::ASTNode_PREDECR:
			case ASTNodeType::ASTNode_PREINCR:
			{
				bAddCode = true;
			}
			break;
		}

		if (bAddCode)
		{
			/////////////////////////////////////////////////
			// 1. Fetch variable value & store it onto the stack
			EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(pPreFixNode->m_sText.c_str()));

			/////////////////////////////////////////////////
			// 2. Push integer 1.
			EMIT_1(OPCODE::PUSHI, (eASTNodeType == ASTNodeType::ASTNode_PREINCR) ? 1 : -1);

			/////////////////////////////////////////////////
			// 3. Add the 2 operands
			EMIT_1(OPCODE::ADD, 0);

			//////////////////////////////////////////////////////
			// Store the decremented/incremented value from the stack back to the variable
			EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pPreFixNode->m_sText.c_str()));
		}
	}
}

void GrammerUtils::handleExpression(Tree* pNode)
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

void GrammerUtils::handlePostFixExpression(Tree* pPostFixNode)
{
	if (pPostFixNode != nullptr)
	{
		bool bAddCode = false;
		ASTNodeType eASTNodeType = pPostFixNode->m_eASTNodeType;
		switch (eASTNodeType)
		{
			case ASTNodeType::ASTNode_EXPRESSION_POSTFIX:
			{
				for (Tree* pChild : pPostFixNode->m_vStatements)
				{
					handlePostFixExpression(pChild);
				}
			}
			break;
			case ASTNodeType::ASTNode_POSTDECR:
			case ASTNodeType::ASTNode_POSTINCR:
			{
				bAddCode = true;
			}
			break;
		}

		if (bAddCode)
		{
			/////////////////////////////////////////////////
			// 1. Fetch variable value & store it onto the stack
			EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(pPostFixNode->m_sText.c_str()));

			/////////////////////////////////////////////////
			// 2. Push integer 1.
			EMIT_1(OPCODE::PUSHI, (eASTNodeType == ASTNodeType::ASTNode_POSTINCR) ? 1 : -1);

			/////////////////////////////////////////////////
			// 3. Add the 2 operands
			EMIT_1(OPCODE::ADD, 0);

			//////////////////////////////////////////////////////
			// Store the decremented/incremented value from the stack back to the variable
			EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pPostFixNode->m_sText.c_str()));
		}
	}
}

void GrammerUtils::handleCharacter(Tree* pNode)
{
	char pStr[255] = { 0 };
	sprintf_s(pStr, "%d", pNode->m_sText.c_str()[0]);
	int ch = atoi(pStr);

	EMIT_1(OPCODE::PUSHI, ch);
}

void GrammerUtils::handleInteger(Tree* pNode)
{
	EMIT_1(OPCODE::PUSHI, atoi(pNode->m_sText.c_str()));
}

void GrammerUtils::handleIdentifier(Tree* pNode)
{
	EMIT_1(OPCODE::FETCH, m_pCurrentFunction->getLocalVariablePosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handleString(Tree* pNode)
{
	EMIT_1(OPCODE::PUSHI, getStringPosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handlePrimitiveInt(Tree* pNode)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
													// In case of ASSIGN, right node will be the lvalue.

	if (pExpressionNode->m_vStatements.size() > 0)
	{
		for (Tree* pChild : pExpressionNode->m_vStatements)
		{
			populateCode(pChild);
		}

		EMIT_1(OPCODE::PUSHR, EREGISTERS::RAX);
	}
	else
		populateCode(pExpressionNode);

	EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handleAssign(Tree* pNode)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
													// In case of ASSIGN, right node will be the lvalue.

	// Compute PreFixExpr
	{
		handlePreFixExpression(pExpressionNode->m_pLeftNode);
	}

	populateCode(pExpressionNode);

	EMIT_1(OPCODE::STORE, m_pCurrentFunction->getLocalVariablePosition(pNode->m_sText.c_str()));

	// Compute PostFixExpr
	{
		handlePostFixExpression(pExpressionNode->m_pRightNode);
	}
}

void GrammerUtils::handleReturnStatement(Tree* pNode)
{
	///////////////////////////////////////////////////////////////
	// The expression result is already pushed onto the stack !
	// Need to retrieve the result & save it in EAX.
	EMIT_1(OPCODE::POPR, EREGISTERS::RAX);
}

void GrammerUtils::handleIfWhile(Tree* pNode)
{
	int i_IfWhile_JCondition_Hole = 0, i_While_Loop_Hole = 0, i_ElseEnd_JMP_Offset = 0;

	// Prologues
	{
		handleIfWhile_Prologue(pNode, i_While_Loop_Hole, i_IfWhile_JCondition_Hole);
	}

	// Intermediates
	{
		handleStatements(pNode);
	}

	// Epilogues
	{	
		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_IF:
			{
				handleIf_Epilogue(pNode, i_ElseEnd_JMP_Offset, i_IfWhile_JCondition_Hole);
			}
			break;
			case ASTNodeType::ASTNode_WHILE:
			{
				handleWhile_Epilogue(pNode, i_While_Loop_Hole, i_IfWhile_JCondition_Hole);
			}
			break;
		}
	}
}

void GrammerUtils::handleIfWhile_Prologue(Tree* pNode, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
	{
		i_While_Loop_Hole = CURRENT_OFFSET;

		handleExpression(pExpressionNode);

		EMIT_BYTE(OPCODE::JZ);
		#if (VERBOSE == 1)
			std::cout << CURRENT_OFFSET << ". " << "JZ" << std::endl;
		#endif

		i_IfWhile_JCondition_Hole = CURRENT_OFFSET;
		EMIT_INT(0);
	}
}

void GrammerUtils::handleIf_Epilogue(Tree* pNode, int& i_ElseEnd_JMP_Offset, int& i_IfWhile_JCondition_Hole)
{
	Tree* pElseNode = pNode->m_pRightNode;

#if (VERBOSE == 1)
	std::cout << CURRENT_OFFSET << ". " << "JMP" << std::endl;
#endif
	EMIT_BYTE(OPCODE::JMP);
	i_ElseEnd_JMP_Offset = CURRENT_OFFSET;
	EMIT_INT(0);

	EMIT_INT_ATPOS(CURRENT_OFFSET, i_IfWhile_JCondition_Hole);
	#if (VERBOSE == 1)
		std::cout << "------" << "i_IfWhile_JCondition_Hole [" << i_IfWhile_JCondition_Hole << "] = " << CURRENT_OFFSET << std::endl;
	#endif

	if (pElseNode != nullptr)
	{
		populateCode(pElseNode);
	}

	EMIT_INT_ATPOS(CURRENT_OFFSET, i_ElseEnd_JMP_Offset);
	#if (VERBOSE == 1)
		std::cout << "------" << "i_ElseEnd_JMP_Offset [" << i_ElseEnd_JMP_Offset << "] = " << CURRENT_OFFSET << std::endl;
	#endif
}

void GrammerUtils::handleWhile_Epilogue(Tree* pNode, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole)
{
#if (VERBOSE == 1)
	std::cout << CURRENT_OFFSET << ". " << "JMP" << std::endl;
#endif
	EMIT_BYTE(OPCODE::JMP);

#if (VERBOSE == 1)
	std::cout << "------" << "i_While_Loop_Hole [" << CURRENT_OFFSET << "] = " << i_While_Loop_Hole << std::endl;
	std::cout << "------" << "i_IfWhile_JCondition_Hole [" << i_IfWhile_JCondition_Hole << "] = " << CURRENT_OFFSET << std::endl;
#endif
	EMIT_INT(i_While_Loop_Hole);
	EMIT_INT_ATPOS(CURRENT_OFFSET, i_IfWhile_JCondition_Hole);
}

void GrammerUtils::handleSwitch(Tree* pNode)
{
	std::vector<uint32_t> vCaseStartOffsets;
	std::vector<uint32_t> vCaseBreakJmpHoles;

	handleSwitchCasePrologue(pNode, vCaseStartOffsets);
	handleSwitchCases(pNode, vCaseStartOffsets, vCaseBreakJmpHoles);
	handleSwitchCaseEpilogues(pNode, vCaseBreakJmpHoles);
}

void GrammerUtils::handleSwitchArgument(Tree* pNode)
{
	switch (pNode->m_eASTNodeType)
	{
		case ASTNodeType::ASTNode_IDENTIFIER:
		{
			handleIdentifier(pNode);
		}
		break;
		case ASTNodeType::ASTNode_INTEGER:
		{
			handleInteger(pNode);
		}
		break;
	}
}

void GrammerUtils::handleSwitchCasePrologue(Tree* pNode, std::vector<uint32_t>& vCaseStartOffsets)
{
	for (Tree* pSwitchCaseNode : pNode->m_vStatements)
	{
		switch (pSwitchCaseNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_SWITCHCASE:
			{
				handleSwitchArgument(pNode->m_pLeftNode);

				handleInteger(pSwitchCaseNode);
				EMIT_1(OPCODE::JMP_EQ, 0);

#if (VERBOSE == 1)
				std::cout << CURRENT_OFFSET << ". " << "JNZ" << std::endl;
#endif
				EMIT_BYTE(OPCODE::JNZ);

				vCaseStartOffsets.push_back(CURRENT_OFFSET);
				EMIT_INT(0);
			}
			break;
			case ASTNodeType::ASTNode_SWITCHDEFAULT:
			{
#if (VERBOSE == 1)
				std::cout << CURRENT_OFFSET << ". " << "JMP" << std::endl;
#endif
				EMIT_BYTE(OPCODE::JMP);

				vCaseStartOffsets.push_back(CURRENT_OFFSET);
				EMIT_INT(0);
			}
			break;
		}
	}
}

void GrammerUtils::handleSwitchCases(Tree* pNode, std::vector<uint32_t>& vCaseStartOffsets, std::vector<uint32_t>& vCaseBreakJmpHoles)
{
	int iCase = 0;
	for (Tree* pSwitchCaseNode : pNode->m_vStatements)
	{
		switch (pSwitchCaseNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_SWITCHCASE:
			{
				EMIT_INT_ATPOS(CURRENT_OFFSET, vCaseStartOffsets[iCase++]);
				handleStatements(pSwitchCaseNode);

				for (Tree* pChildNode : pSwitchCaseNode->m_vStatements)
				{
					if (pChildNode->m_eASTNodeType == ASTNodeType::ASTNode_SWITCHBREAK)
					{
#if (VERBOSE == 1)
						std::cout << CURRENT_OFFSET << ". " << "JMP" << std::endl;
#endif
						EMIT_BYTE(OPCODE::JMP);

						vCaseBreakJmpHoles.push_back(CURRENT_OFFSET);
						EMIT_INT(0);
					}
				}
			}
			break;
			case ASTNodeType::ASTNode_SWITCHDEFAULT:
			{
				EMIT_INT_ATPOS(CURRENT_OFFSET, vCaseStartOffsets[iCase++]);
				handleStatements(pSwitchCaseNode);

				for (Tree* pChildNode : pSwitchCaseNode->m_vStatements)
				{
					if (pChildNode->m_eASTNodeType == ASTNodeType::ASTNode_SWITCHBREAK)
					{
#if (VERBOSE == 1)
						std::cout << CURRENT_OFFSET << ". " << "JMP" << std::endl;
#endif
						EMIT_BYTE(OPCODE::JMP);
					}
				}
			}
			break;
		}
	}
}

void GrammerUtils::handleSwitchCaseEpilogues(Tree* pNode, std::vector<uint32_t>& vCaseBreakJmpHoles)
{
	for (int iBreakJumpHole : vCaseBreakJmpHoles)
	{
		EMIT_INT_ATPOS(CURRENT_OFFSET, iBreakJumpHole);
	}
}

void GrammerUtils::handleStatements(Tree* pNode)
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
		populateCode(pChildNode);

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

void GrammerUtils::printAssembly(int8_t* iByteCode, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings)
{
	RandomAccessFile* pRaf = new RandomAccessFile();
	bool bCanWrite = pRaf->openForWrite("main.o");

	if (bCanWrite)
	{
		printHeaders(pRaf, vVariables, vStrings);
	}

	m_pBAIS->reset();
	OPCODE eOpCode = OPCODE::NOP;
	while (true)
	{
		std::cout << CURRENT_OFFSET_READ;

		eOpCode = (OPCODE)m_pBAIS->readByte();
		if(bCanWrite)
			pRaf->writeByte((int)eOpCode);

		CodeMap pMachineInstruction = opCodeMap[(int)eOpCode];

		// Print to stdout
		{
			std::cout << ". " << pMachineInstruction.sOpCode;

			for(int i = 1; i < pMachineInstruction.iOpcodeOperandCount; i++)
			{
				std::cout << " ";
				switch (pMachineInstruction.ePRIMIIVETYPE)
				{
					case PRIMIIVETYPE::INT_8:
					{
						int8_t iByte = m_pBAIS->readByte();

						std::cout << (int32_t)iByte;
						if (bCanWrite)
							pRaf->writeByte(iByte);
					}
					break;
					case PRIMIIVETYPE::INT_16:
					{
						int16_t iShort = m_pBAIS->readShort();

						std::cout << (int32_t)iShort;
						if (bCanWrite)
							pRaf->writeShort(iShort);
					}
					break;
					case PRIMIIVETYPE::INT_32:
					{
						int32_t iInt = m_pBAIS->readInt();

						std::cout << iInt;
						if (bCanWrite)
							pRaf->writeInt(iInt);
					}
					break;
					case PRIMIIVETYPE::INT_64:
					{
						int64_t iLong = m_pBAIS->readLong();

						std::cout << iLong;
						if (bCanWrite)
							pRaf->writeLong(iLong);
					}
					break;
				}
			}

			std::cout << std::endl;
		}

		if (eOpCode == OPCODE::HLT)
			break;
	}

	pRaf->close();
}
