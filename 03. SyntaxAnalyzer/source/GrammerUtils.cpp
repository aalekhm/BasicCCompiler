#include "GrammerUtils.h"
#include "ByteArrayOutputStream.h"
#include "ByteArrayInputStream.h"
#include <assert.h>
#include "TinyCReader.h"

Token									GrammerUtils::m_pToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);
Token									GrammerUtils::m_pPrevToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);

std::vector<std::string>				GrammerUtils::m_vKeywords;
std::vector<std::string>				GrammerUtils::m_vTypes;
std::vector<std::string>				GrammerUtils::m_vUserDefinedTypes;

StringTokenizer*						GrammerUtils::m_pStrTok = NULL;
int										GrammerUtils::iTabCount = 0;
std::vector<std::string>				GrammerUtils::m_vStrings;
int8_t									GrammerUtils::m_iByteCode[MAX_BYTECODE_SIZE];
std::map<std::string, FunctionInfo*>	GrammerUtils::m_MapGlobalFunctions;
std::map<std::string, StructInfo*>		GrammerUtils::m_MapGlobalStructs;
FunctionInfo*							GrammerUtils::m_pCurrentFunction;
StructInfo*								GrammerUtils::m_pCurrentStruct;
ByteArrayOutputStream*					GrammerUtils::m_pBAOS;
ByteArrayInputStream*					GrammerUtils::m_pBAIS;

std::vector<Tree*>						FunctionInfo::m_vStaticVariables;

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

#define GET_VARIABLE_POSITION(__VAR__NAME__)			m_pCurrentFunction->getLocalVariablePosition(__VAR__NAME__)
#define IS_POINTER_TYPE(__VAR_NAME__)					m_pCurrentFunction->IsVariableAPointerType(__VAR_NAME__)
#define GET_VARIABLE_NODETYPE(__VAR_NAME__)				m_pCurrentFunction->getVariableNodeType(__VAR_NAME__)

#define IS_STATEMENT_INSIDE_FUNCTION					if(m_pCurrentFunction != nullptr)
#define CREATE_NODE_OF_AST(__eASTType__, __TEXT__)		createNodeOfType(__eASTType__, __TEXT__)

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
	{ "BITWISEOR",	OPCODE::BITWISEOR,	1,  PRIMIIVETYPE::INT_8},
	{ "BITWISEAND",	OPCODE::BITWISEAND,	1,  PRIMIIVETYPE::INT_8},
	{ "BITWISEXOR",	OPCODE::BITWISEXOR,	1,  PRIMIIVETYPE::INT_8},
	{ "BITWISENOT",	OPCODE::BITWISENOT,	1,  PRIMIIVETYPE::INT_8},
	{ "BITWISELEFTSHIFT",	OPCODE::BITWISELEFTSHIFT,	1,  PRIMIIVETYPE::INT_8},
	{ "BITWISERIGHTSHIFT",	OPCODE::BITWISERIGHTSHIFT,	1,  PRIMIIVETYPE::INT_8},
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

	{ "MALLOC",		OPCODE::MALLOC,		1,  PRIMIIVETYPE::INT_8},
	{ "FREE",		OPCODE::FREE,		2,  PRIMIIVETYPE::INT_32},

	{ "LDA",		OPCODE::LDA,		2,  PRIMIIVETYPE::INT_32},
	{ "STA",		OPCODE::STA,		2,  PRIMIIVETYPE::INT_32},
	{ "CLR",		OPCODE::CLR,		7,  PRIMIIVETYPE::INT_32},

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

void GrammerUtils::printAST(Tree* pNode, bool bPrintTabs/* = true*/)
{
	Tree* pLeftNode = pNode->m_pLeftNode;
	Tree* pRightNode = pNode->m_pRightNode;
	bool bProcessChildren = true;

	if (bPrintTabs)
	{
		printTabs();
	}

	switch (pNode->m_eASTNodeType)
	{
		case ASTNodeType::ASTNode_STRUCTDEF:
		{
			std::cout << "struct " << pNode->m_sText << std::endl;
			std::cout << "{" << std::endl;
			iTabCount++;

			bProcessChildren = true;
		}
		break;
		case ASTNodeType::ASTNode_MEMBERACCESS:
		{
			std::cout << pNode->m_sText << "->";
			if (pNode->m_vStatements.size() > 0)
			{
				Tree* pFirstChild = pNode->m_vStatements[0];
				if (pFirstChild != nullptr)
				{
					ASTNodeType eChilsASTNodeType = pFirstChild->m_eASTNodeType;
					if (eChilsASTNodeType == ASTNodeType::ASTNode_FUNCTIONCALL)
					{
						printAST(pFirstChild, false);
					}
				}
			}

			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_TYPESTATIC:
		{
			std::cout << "static ";
			std::string sType = pNode->getAdditionalInfoFor("type");
			std::cout << sType << "* " << pNode->m_sText << ";";
		}
		break;
		case ASTNodeType::ASTNode_FUNCTIONDEF:
		{
			Tree* pReturnTypeNode = pNode->m_pLeftNode;
			Tree* pArgListNode = pNode->m_pRightNode;

			std::cout << pReturnTypeNode->m_sText << " " << pNode->m_sText << "(";

			// Arg List
			for (Tree* pArgNode : pArgListNode->m_vStatements)
			{
				std::cout << pArgNode->getAdditionalInfoFor("type") << " " << pArgNode->m_sAdditionalInfo << ", ";
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
				if (pChildNode->m_eASTNodeType != ASTNodeType::ASTNode_FUNCTIONCALLEND)
					std::cout << pChildNode->m_sText << ", ";
			}
			std::cout << ");";

			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_TYPE:
		{
			std::cout << pNode->getAdditionalInfoFor("type");
			std::cout << (pNode->m_bIsPointerType ? "*" : "");
			std::cout << " ";
			std::cout << pNode->m_sText;

			Tree* pExpressionNode = pNode->m_pLeftNode;
			if (pExpressionNode != nullptr)
			{
				std::cout << " = ";

				if (pExpressionNode->m_vStatements.size() > 0)
				{
					for (Tree* pChild : pExpressionNode->m_vStatements)
					{
						if (pChild->m_eASTNodeType == ASTNodeType::ASTNode_MEMBERACCESS)
							printAST(pChild, false);
						else
							std::cout << pChild->m_sText << " ";
					}
				}
				else
					if (pExpressionNode != nullptr)
						std::cout << pExpressionNode->m_sText;
			}
		}
		break;
		case ASTNodeType::ASTNode_TYPEARRAY:
		{
			Tree* pArraySizeLeaf = pNode->m_pLeftNode;
			Tree* pArrayElementsLeaf = pNode->m_pRightNode;

			std::cout << pNode->getAdditionalInfoFor("type") << " " << pNode->m_sAdditionalInfo << "[";
			if (pArraySizeLeaf != nullptr)
			{
				std::cout << pArraySizeLeaf->m_sText;
			}

			std::cout << "]";

			if (pArrayElementsLeaf != nullptr)
			{
				std::cout << " " << "=" << " ";
				std::cout << "{";

				for (Tree* pArrayElement : pArrayElementsLeaf->m_vStatements)
				{
					std::cout << " " << pArrayElement->m_sText << ",";
				}

				std::cout << "}";
			}

			std::cout << ";";
		}
		break;
		case ASTNodeType::ASTNode_TYPESTRUCT:
		{
			std::string sStructType = pNode->getAdditionalInfoFor("type");
			std::cout << sStructType << "* " << pNode->m_sText << " = new " << sStructType << "(";

			Tree* pConstructorCallNode = pNode->m_vStatements[0];
			for (Tree* pChild : pConstructorCallNode->m_vStatements)
			{
				if(pChild->m_eASTNodeType != ASTNodeType::ASTNode_CONSTRUCTORCALLEND)
					std::cout << pChild->m_sText << ",";
			}

			std::cout << ")";
			std::cout << ";";

			bProcessChildren = false;
		}
		break;
		case ASTNodeType::ASTNode_MALLOC:
		{
			std::cout << "malloc(";

			Tree* pExpressionNode = pNode->m_pLeftNode;
			std::cout << pExpressionNode->m_sText;

			std::cout << ");";
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
		case ASTNodeType::ASTNode_ASSIGN:
		{
			Tree* pIdentifierNode = pRightNode;
			Tree* pExpressionNode = pLeftNode;
			bool bPrintExpressionChilds = true;

			// PreFix
			if (pExpressionNode->m_pLeftNode != nullptr)
			{
				for (Tree* pChildNode : pExpressionNode->m_pLeftNode->m_vStatements)
				{
					printAST(pChildNode);
				}
			}

			std::string sIdentifierText = "";
			switch (pIdentifierNode->m_eASTNodeType)
			{
				case ASTNodeType::ASTNode_IDENTIFIER:
				{
					sIdentifierText += pIdentifierNode->m_sText;
				}
				break;
				case ASTNodeType::ASTNode_DEREF:
				case ASTNodeType::ASTNode_DEREFARRAY:
				{
					sIdentifierText = "@";
					sIdentifierText += pIdentifierNode->m_sText;
				}
				break;
				case ASTNodeType::ASTNode_MEMBERACCESS:
				{
					sIdentifierText = pNode->m_sText;
					sIdentifierText += "->";
					sIdentifierText += pIdentifierNode->m_sText;

					bPrintExpressionChilds = false;
				}
				break;
			}

			if (pExpressionNode->m_vStatements.size() > 0 && bPrintExpressionChilds)
			{
				std::cout << sIdentifierText << " = ";
				for (Tree* pChildNode : pExpressionNode->m_vStatements) // Eg. malloc()
				{
					printAST(pChildNode, false);
				}
			}
			else
				std::cout << sIdentifierText << " = " << pExpressionNode->m_sText << ";" << std::endl;

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
		case ASTNodeType::ASTNode_FREE:
		{
			std::cout << "free(" << pNode->m_sText.c_str() << ")";
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
		case ASTNodeType::ASTNode_STRUCTDEF:
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
		case ASTNodeType::ASTNode_TYPE:
		{
			std::cout << ";";
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
		EMIT_INT_ATPOS(CURRENT_OFFSET, iMainRetAddressHole);

		std::cout << CURRENT_OFFSET << ". " << "HLT" << std::endl;
		EMIT_BYTE(OPCODE::HLT);
		//////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////
		// Map main() jump offset.
		std::map<std::string, FunctionInfo*>::const_iterator itrFuncMain = m_MapGlobalFunctions.find("main");
		assert(itrFuncMain != m_MapGlobalFunctions.end());
		if (itrFuncMain != m_MapGlobalFunctions.end())
		{
			FunctionInfo* pMainFuncInfo = (FunctionInfo*)itrFuncMain->second;

			EMIT_INT_ATPOS(pMainFuncInfo->m_iStartOffsetInCode, iJumpMainOffsetHole);
#if (VERBOSE == 1)
			std::cout << pMainFuncInfo->m_iStartOffsetInCode << std::endl;
#endif
		}
		//////////////////////////////////////////////////////////////////////////////

		printAssembly(m_iByteCode, m_vStrings);
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
				case ASTNodeType::ASTNode_STRUCTDEF:
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
			case ASTNodeType::ASTNode_STRUCTDEF:
			{
				handleStructDef(pNode);
			}
			break;
			case ASTNodeType::ASTNode_STRUCTSTART:
			{

			}
			break;
			case ASTNodeType::ASTNode_STRUCTEND:
			{
				bool bDefaultAdded = false;
				std::string sStructName = m_pCurrentStruct->m_sStructName;
				Tree* pStructNode = m_pCurrentStruct->m_pNode;
				if (NOT m_pCurrentStruct->m_bHasConstructor)
				{
					Tree* pDefaultConstructor = createFunctionWithNoArguments(sStructName.c_str(), "void");
					pStructNode->addChild(pDefaultConstructor);

					bDefaultAdded = true;
				}

				if (NOT m_pCurrentStruct->m_bHasDestructor)
				{
					std::string sDestructorName = "#" + sStructName;
					Tree* pDestructor = createFunctionWithNoArguments(sDestructorName.c_str(), "void");
					pStructNode->addChild(pDestructor);

					bDefaultAdded = true;
				}

				if (bDefaultAdded)
				{
					Tree* pStructEndNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_STRUCTEND, "");
					pStructNode->addChild(pStructEndNode);
				}
				else
					m_pCurrentStruct = nullptr;
			}
			break;
			case ASTNodeType::ASTNode_TYPESTATIC:
			{
				handleStatics(pNode);
			}
			break;
			case ASTNodeType::ASTNode_TYPESTRUCT:
			{
				handleTypeStructs(pNode);
				bProcessStatements = false;
			}
			break;
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
			case ASTNodeType::ASTNode_MEMBERACCESS:
			{
				std::string sPointerName = pNode->m_sText;
				if(pNode->m_vStatements.size() > 0)
				{
					Tree* pFunctionCallNode = pNode->m_vStatements[0];
					if (pFunctionCallNode != nullptr)
					{
						ASTNodeType eChilsASTNodeType = pFunctionCallNode->m_eASTNodeType;
						if (eChilsASTNodeType == ASTNodeType::ASTNode_FUNCTIONCALL)
						{
							std::string sType = GET_VARIABLE_NODETYPE(sPointerName.c_str());
							pFunctionCallNode->setAdditionalInfo("memberFunctionOf", sType);

							// 1. Fetch 'this' into 'ECX'
							EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(sPointerName.c_str()));		// Get the 'this' pointer value
							EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// & push it in 'ECX'
							
							// 2. Make the function call.
							populateCode(pFunctionCallNode);

							//// 3. Clear off 'ECX' that holds the address of 'this'
							//EMIT_1(OPCODE::PUSH, 0);
							//EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// Clear off 'ECX'.
						}
					}
				}

				bProcessStatements = false;
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
			case ASTNodeType::ASTNode_TYPE:
			{
				IS_STATEMENT_INSIDE_FUNCTION
				{
					if (NOT pNode->m_bIsPointerType)
						handlePrimitiveInt(pNode);
				}
			}
			break;
			case ASTNodeType::ASTNode_TYPEARRAY:
			{
				IS_STATEMENT_INSIDE_FUNCTION
				{
					handleTypeArray(pNode);
				}
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
			case ASTNodeType::ASTNode_MALLOC:
			{
				handleMalloc(pNode);
				bProcessStatements = false;					// Since we are processing the child statements in "handleMalloc()"
			}
			break;
			case ASTNodeType::ASTNode_FREE:
			{
				handleFree(pNode);
				bProcessStatements = false;					// No childrens.
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
			case ASTNodeType::ASTNode_TYPE:
			{
				IS_STATEMENT_INSIDE_FUNCTION
				{
					if (pNode->m_bIsPointerType)
						handlePrimitivePtrEpilogue(pNode);
				}
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
	case OPCODE::FREE:
	case OPCODE::STA:
	case OPCODE::LDA:
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
	case OPCODE::BITWISEAND:
	case OPCODE::BITWISEOR:
	case OPCODE::BITWISEXOR:
	case OPCODE::BITWISENOT:
	case OPCODE::BITWISELEFTSHIFT:
	case OPCODE::BITWISERIGHTSHIFT:
	case OPCODE::_NOT:
	case OPCODE::NEGATE:
	case OPCODE::PRTC:
	case OPCODE::PRTI:
	case OPCODE::PRTS:
	case OPCODE::MALLOC:
	case OPCODE::RET:
	{
#if (VERBOSE == 1)
		std::cout << CURRENT_OFFSET << ". " << opCodeMap[(int)eOPCODE].sOpCode << std::endl;
#endif
		EMIT_BYTE(eOPCODE);
	}
	break;
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

void GrammerUtils::handleStructDef(Tree* pNode)
{
	std::string sStructName = pNode->m_sText;

	m_pCurrentStruct = new StructInfo(pNode);
	if (m_pCurrentStruct != nullptr)
	{
		m_MapGlobalStructs[sStructName] = m_pCurrentStruct;
	}
}

Tree* GrammerUtils::createASTForArrayType(Tree* pASTArrayTypeSrc)
{
	Tree* pPrimTypeArrayNode = nullptr;
	if (pASTArrayTypeSrc != nullptr)
	{
		pPrimTypeArrayNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_TYPEARRAY, pASTArrayTypeSrc->m_sText.c_str());
		{
			pPrimTypeArrayNode->m_sAdditionalInfo.append(pASTArrayTypeSrc->m_sAdditionalInfo);
			pPrimTypeArrayNode->m_bIsPointerType = true;
			pPrimTypeArrayNode->setAdditionalInfo("type", pASTArrayTypeSrc->getAdditionalInfoFor("type"));
			pPrimTypeArrayNode->m_pParentNode = nullptr;

			// optional Array Size
			{
				std::string sArraySize = pASTArrayTypeSrc->m_pLeftNode->m_sText;
				Tree* pArraySizeLeaf = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_INTEGER, sArraySize.c_str());
				{
					pPrimTypeArrayNode->m_pLeftNode = pArraySizeLeaf;
					pArraySizeLeaf->m_pParentNode = pPrimTypeArrayNode;
				}
			}

			//= 

			// Optional ELements
			{
				//int32_t arr[] = {10, 20, 30, 40};
				Tree* pArrayElementsLeaf = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_TYPEARRAYELEMENTS, "");
				pPrimTypeArrayNode->m_pRightNode = pArrayElementsLeaf;
				pArrayElementsLeaf->m_pParentNode = pPrimTypeArrayNode;

				Tree* pSrcArrayElementsNode = pASTArrayTypeSrc->m_pRightNode;
				if (pSrcArrayElementsNode != nullptr)
				{
					pArrayElementsLeaf->m_sText = pSrcArrayElementsNode->m_sText;
					for (Tree* pExpressionsLeaf : pSrcArrayElementsNode->m_vStatements)
					{
						Tree* pExpressionArrayElementLeaf = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_EXPRESSION, "");
						pExpressionArrayElementLeaf->m_sText = pExpressionsLeaf->m_sText;

						pArrayElementsLeaf->addChild(pExpressionArrayElementLeaf);
					}
				}
			}
		}
	}

	return pPrimTypeArrayNode;
}

Tree* GrammerUtils::createASTForPointerType(Tree* pASTArrayTypeSrc)
{
	Tree* pPrimTypePointerNode = nullptr;
	if (pASTArrayTypeSrc != nullptr)
	{
		pPrimTypePointerNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_TYPE, pASTArrayTypeSrc->m_sText.c_str());
		{
			pPrimTypePointerNode->m_sAdditionalInfo.append(pASTArrayTypeSrc->m_sAdditionalInfo);
			pPrimTypePointerNode->m_bIsPointerType = true;
			pPrimTypePointerNode->setAdditionalInfo("type", pASTArrayTypeSrc->getAdditionalInfoFor("type"));
			pPrimTypePointerNode->m_pParentNode = nullptr;
		}
	}

	return pPrimTypePointerNode;
}

Tree* GrammerUtils::createASTForType(Tree* pASTArrayTypeSrc)
{
	Tree* pPrimTypeNode = nullptr;
	if (pASTArrayTypeSrc != nullptr)
	{
		pPrimTypeNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_TYPE, pASTArrayTypeSrc->m_sText.c_str());
		{
			pPrimTypeNode->m_sAdditionalInfo.append(pASTArrayTypeSrc->m_sAdditionalInfo);
			pPrimTypeNode->setAdditionalInfo("type", pASTArrayTypeSrc->getAdditionalInfoFor("type"));

			// =
			// Optional RHS(Expression)
			{
				Tree* pExpressionLeftLeaf = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_EXPRESSION, "");
				pPrimTypeNode->m_pLeftNode = pExpressionLeftLeaf;
				pExpressionLeftLeaf->m_pParentNode = pPrimTypeNode;

				pExpressionLeftLeaf->m_sText = pASTArrayTypeSrc->m_pLeftNode->m_sText;
			}
		}
	}

	return pPrimTypeNode;
}

Tree* GrammerUtils::createFreeASTForArrayType(Tree* pASTArrayTypeSrc)
{
	Tree* pFreeTypeNode = nullptr;
	if (pASTArrayTypeSrc != nullptr)
	{
		pFreeTypeNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FREE, pASTArrayTypeSrc->m_sText.c_str());
	}

	return pFreeTypeNode;
}

Tree* GrammerUtils::createFunctionWithNoArguments(const char* sFunctionName, const char* sReturnType)
{
	Tree* pFunctionDefNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONDEF, sFunctionName);
	{
		Tree* pReturnTypeNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONRETURNTYPE, sReturnType);
		Tree* pArgListNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONARGLIST, "");

		pReturnTypeNode->m_pParentNode = pFunctionDefNode;
		pArgListNode->m_pParentNode = pFunctionDefNode;

		pFunctionDefNode->m_pLeftNode = pReturnTypeNode;
		pFunctionDefNode->m_pRightNode = pArgListNode;

		// FunctionStart
		{
			Tree* pFuncStartNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONSTART, "");
			pFunctionDefNode->addChild(pFuncStartNode);
		}

		// Statements
		{

		}

		// FunctionEnd
		{
			Tree* pFuncEndNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONEND, "");
			pFunctionDefNode->addChild(pFuncEndNode);
		}
	}

	return pFunctionDefNode;
}

Tree* GrammerUtils::createFunctionCallWithNoArguments(const char* sFunctionName)
{
	Tree* pFunctionCallNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONCALL, sFunctionName);
	{
		Tree* pFuncCallEndNode = CREATE_NODE_OF_AST(ASTNodeType::ASTNode_FUNCTIONCALLEND, sFunctionName);
		pFunctionCallNode->addChild(pFuncCallEndNode);
	}

	return pFunctionCallNode;
}

Tree* GrammerUtils::createNodeOfType(ASTNodeType eASTNodeType, const char* sText)
{
	Tree* pASTNode = TinyCReader::makeLeaf(eASTNodeType, sText);
	return pASTNode;
}

void GrammerUtils::addASTForStructMemberVariableConstruction(FunctionInfo* pFunctionInfo)
{
	Tree* pFunctionNode = pFunctionInfo->m_pNode;
	Tree* pCreatedASTNode = nullptr;
	for (Tree* pASTNode : m_pCurrentStruct->m_vMemberVariables)
	{
		switch (pASTNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_TYPE:
			{
				if (pASTNode->m_bIsPointerType)
				{
					//pCreatedASTNode = createASTForPointerType(pASTNode);
				}
				else
				{
					pCreatedASTNode = createASTForType(pASTNode);
				}
			}
			break;
			case ASTNodeType::ASTNode_TYPEARRAY:
			{
				pCreatedASTNode = createASTForArrayType(pASTNode);
			}
			break;
		}

		if (pCreatedASTNode != nullptr)
		{
			// Inserting @ position '1' as '0' is ASTNode_FUNCTIONSTART
			pFunctionNode->insertAt(1, pCreatedASTNode);
		}

		pCreatedASTNode = nullptr;
	}
}

void GrammerUtils::addASTForStructMemberVariableDestruction(FunctionInfo* pFunctionInfo)
{
	Tree* pFunctionNode = pFunctionInfo->m_pNode;
	Tree* pCreatedASTNode = nullptr;
	for (Tree* pASTNode : m_pCurrentStruct->m_vMemberVariables)
	{
		switch (pASTNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_TYPEARRAY:
			{
				pCreatedASTNode = createFreeASTForArrayType(pASTNode);
			}
			break;
		}

		if (pCreatedASTNode != nullptr)
		{
			// Inserting @ position '1' as '0' is ASTNode_FUNCTIONSTART
			pFunctionNode->insertAt(1, pCreatedASTNode);
		}

		pCreatedASTNode = nullptr;
	}
}

void GrammerUtils::handleStructConstructorOrDestructor(FunctionInfo* pFunctionInfo)
{
	// Check if current Function is Constructor or Destructor
	std::string sStructName = m_pCurrentStruct->m_sStructName;
	std::string sFunctionName = pFunctionInfo->m_sFunctionName;
	std::string sDestructorName = "#" + sStructName;

	// Constructor
	if (sFunctionName == sStructName)
	{
		// Constructor without Arguments
		if(pFunctionInfo->m_pFunctionArguments->m_sAdditionalInfo.empty())
			m_pCurrentStruct->m_bHasConstructor = true;
		addASTForStructMemberVariableConstruction(pFunctionInfo);
	}
	else // Destructor
	if (sFunctionName == sDestructorName)
	{
		m_pCurrentStruct->m_bHasDestructor = true;
		addASTForStructMemberVariableDestruction(pFunctionInfo);
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
		if (m_pCurrentStruct != nullptr)
		{
			m_pCurrentStruct->addFunction(m_pCurrentFunction);
			m_pCurrentFunction->setParent(m_pCurrentStruct);

			handleStructConstructorOrDestructor(m_pCurrentFunction);
		}
		else
		{
			m_MapGlobalFunctions[sFuncName] = m_pCurrentFunction;
		}
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

	m_pCurrentFunction = nullptr;
}

FunctionInfo* GrammerUtils::getFunctionInfo(Tree* pNode)
{
	FunctionInfo* pRETURN_FunctionInfo = nullptr;

	std::string sType = "";
	std::string sFuncCallee = pNode->m_sText;
	Tree* pParent_FunctionCall = pNode->m_pParentNode;
	if (pParent_FunctionCall != nullptr)
	{
		std::string sCallee_Signature = pParent_FunctionCall->m_sAdditionalInfo;
		int32_t iCallee_ArgCount = pParent_FunctionCall->m_vStatements.size() - 1;

		//////////////////////////////////////////////////////////
		// 1. Check if the function() is in a STRUCT.
		sType = pParent_FunctionCall->getAdditionalInfoFor("memberFunctionOf");
		if (NOT sType.empty())
		{
LABEL1:
			StructInfo* pStructInfo = m_MapGlobalStructs[sType];
			if (pStructInfo != nullptr)
			{
				if (pStructInfo != nullptr)
				{
					for (void* pVoidPtr : pStructInfo->m_vMemberFunctions)
					{
						FunctionInfo* pFunctionInfo = (FunctionInfo*)pVoidPtr;
						if (pFunctionInfo != nullptr)
						{
							std::string sFUNCDEF_Name = pFunctionInfo->m_sFunctionName;
							Tree* pFUNCDEF_ArgListNode = pFunctionInfo->m_pFunctionArguments;
							int iFUNCDEF_ArgCount = pFUNCDEF_ArgListNode->m_vStatements.size();
							std::string sFUNCDEF_Signature = pFUNCDEF_ArgListNode->m_sAdditionalInfo;

							if (sFuncCallee == sFUNCDEF_Name
								&&
								iCallee_ArgCount == iFUNCDEF_ArgCount
								&&
								sCallee_Signature == sFUNCDEF_Signature
							) {
								pRETURN_FunctionInfo = pFunctionInfo;
								break;
							}
						}
					}
				}
			}
		}

		//////////////////////////////////////////////////////////
		// 2. Check if its a global function().
		if (pRETURN_FunctionInfo == nullptr)
		{
			std::map<std::string, FunctionInfo*>::const_iterator itrFunction = m_MapGlobalFunctions.begin();
			for (; itrFunction != m_MapGlobalFunctions.end(); ++itrFunction)
			{
				FunctionInfo* pFunctionInfo = itrFunction->second;
				if (pFunctionInfo != nullptr)
				{
					std::string sFUNCDEF_Name = pFunctionInfo->m_sFunctionName;
					Tree* pFUNCDEF_ArgListNode = pFunctionInfo->m_pFunctionArguments;
					int iFUNCDEF_ArgCount = pFUNCDEF_ArgListNode->m_vStatements.size();
					std::string sFUNCDEF_Signature = pFUNCDEF_ArgListNode->m_sAdditionalInfo;

					if (sFuncCallee == sFUNCDEF_Name
						&&
						iCallee_ArgCount == iFUNCDEF_ArgCount
						&&
						sCallee_Signature == sFUNCDEF_Signature
					) {
						pRETURN_FunctionInfo = pFunctionInfo;
						break;
					}
				}
			}
		}

		//////////////////////////////////////////////////////////
		// 3. If even here 'pRETURN_FunctionInfo' is nullptr, then maybe its a member function call from inside another member function of a struct.
		if (pRETURN_FunctionInfo == nullptr)
		{
			if (m_pCurrentStruct != nullptr && m_pCurrentFunction != nullptr)
			{
				// Disguise this function call made inside 'm_pCurrentStruct'
				sType = m_pCurrentStruct->m_sStructName;
				goto LABEL1;
			}
		}
	}

	return pRETURN_FunctionInfo;
}

void GrammerUtils::handleFunctionCall(Tree* pNode)
{
	// Callee function name
	std::string sFuncCallee = pNode->m_sText;

	// Check if the 'Callee' is our list !
	FunctionInfo* pCalleeFunctionInfo = getFunctionInfo(pNode);

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

		if ((iFUNCDEF_ArgCount == iCALLEE_ArgCount)
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
#if (VERBOSE == 1)
			std::cout << "\t\t\t\t\t\tCALL " << "[" << sFuncCallee << "]" << std::endl;
#endif

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
			EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(pPreFixNode->m_sText.c_str()));

			/////////////////////////////////////////////////
			// 2. Push integer 1.
			{
				int8_t iIncrementValue = 1;
				if (IS_POINTER_TYPE(pPreFixNode->m_sText.c_str()))
				{
					std::string sType = GET_VARIABLE_NODETYPE(pPreFixNode->m_sText.c_str());
					iIncrementValue = sizeOf(sType);
				}

				EMIT_1(OPCODE::PUSHI, (eASTNodeType == ASTNodeType::ASTNode_PREINCR) ? iIncrementValue : -iIncrementValue);
			}

			/////////////////////////////////////////////////
			// 3. Add the 2 operands
			EMIT_1(OPCODE::ADD, 0);

			//////////////////////////////////////////////////////
			// Store the decremented/incremented value from the stack back to the variable
			EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pPreFixNode->m_sText.c_str()));
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
				EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(tok.getText()));
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
			case TokenType::Type::TK_BITWISEAND:
				EMIT_1(OPCODE::BITWISEAND, 0);
				break;
			case TokenType::Type::TK_BITWISEOR:
				EMIT_1(OPCODE::BITWISEOR, 0);
				break;
			case TokenType::Type::TK_BITWISEXOR:
				EMIT_1(OPCODE::BITWISEXOR, 0);
				break;
			case TokenType::Type::TK_BITWISENOT:
				EMIT_1(OPCODE::BITWISENOT, 0);
				break;
			case TokenType::Type::TK_BITWISELEFTSHIFT:
				EMIT_1(OPCODE::BITWISELEFTSHIFT, 0);
				break;
			case TokenType::Type::TK_BITWISERIGHTSHIFT:
				EMIT_1(OPCODE::BITWISERIGHTSHIFT, 0);
				break;
			case TokenType::Type::TK_NOT:
				EMIT_1(OPCODE::_NOT, 0);
				break;
			case TokenType::Type::TK_NEGATE:
				EMIT_1(OPCODE::NEGATE, 0);
				break;
			case TokenType::Type::TK_DEREF:
			{
				// Send in the 'CAST' value of the pointer Type(int8_t = 0xFF, int16_6 = 0xFFFF, int32_t = 0xFFFFFFFF)
				// which will be used @ runtime to make a 'CAST'.
				std::string sType = GET_VARIABLE_NODETYPE(prevTok.getText());
				uint32_t iCastValue = castValueFor(sType);
				EMIT_1(OPCODE::PUSHI, sizeOf(sType));		// Push the size of the node for ArrayIndexing.
				EMIT_1(OPCODE::LDA, iCastValue);
			}
			break;
			case TokenType::Type::TK_MEMBERACCESS:
			{
				// Member Access has the following notation:
				//		==> sObjectName->variableName
				std::string sObjectName = tok.getText();	// sObjectName
				Token tok = st->nextToken();				// "-"
				tok = st->nextToken();						// ">"
				tok = st->nextToken();						// variableName

				std::string sStructType = GET_VARIABLE_NODETYPE(sObjectName.c_str());
				StructInfo* pStructInfo = m_MapGlobalStructs[sStructType];
				assert(pStructInfo != nullptr);
				if (pStructInfo != nullptr)
				{
					// 1. Fetch 'this' into 'ECX'
					EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(sObjectName.c_str()));		// Get the 'this' pointer value
					EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// & push it in 'ECX'

					// 2. Fetch the value in the objects variable & push it onto the STACK.
					std::string sFullyQualifiedVariableName = sStructType + "_" + tok.getText();
					int32_t iPosition = pStructInfo->getMemberVariablePosition(sFullyQualifiedVariableName.c_str());
					EMIT_1(OPCODE::FETCH, iPosition);
				}
			}
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
			EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(pPostFixNode->m_sText.c_str()));

			/////////////////////////////////////////////////
			// 2. Push integer 1.
			{
				int8_t iIncrementValue = 1;
				if (IS_POINTER_TYPE(pPostFixNode->m_sText.c_str()))
				{
					std::string sType = GET_VARIABLE_NODETYPE(pPostFixNode->m_sText.c_str());
					iIncrementValue = sizeOf(sType);
				}

				EMIT_1(OPCODE::PUSHI, (eASTNodeType == ASTNodeType::ASTNode_POSTINCR) ? iIncrementValue : -iIncrementValue);
			}

			/////////////////////////////////////////////////
			// 3. Add the 2 operands
			EMIT_1(OPCODE::ADD, 0);

			//////////////////////////////////////////////////////
			// Store the decremented/incremented value from the stack back to the variable
			EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pPostFixNode->m_sText.c_str()));
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
	EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));
}

void GrammerUtils::handleString(Tree* pNode)
{
	EMIT_1(OPCODE::PUSHI, getStringPosition(pNode->m_sText.c_str()));
}

void GrammerUtils::handlePrimitiveInt(Tree* pNode)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
													// In case of ASSIGN, right node will be the lvalue.
	if (pExpressionNode != nullptr)
	{
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

		cast(castValueFor(pNode->getAdditionalInfoFor("type")));
		EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));
	}
}

void GrammerUtils::handleTypeArray(Tree* pNode)
{
	Tree* pArraySizeLeaf = pNode->m_pLeftNode;
	Tree* pArrayElementsLeaf = pNode->m_pRightNode;

	std::string sType = pNode->getAdditionalInfoFor("type");
	int32_t iSize = 0;

	// Allocate the sizeOfArray * sizeof(arrayType)
	{
		if (pArraySizeLeaf != nullptr)
		{
			iSize = atoi(pArraySizeLeaf->m_sText.c_str());
		}
		else
		{
			iSize = pArrayElementsLeaf->m_vStatements.size();
		}

		EMIT_1(OPCODE::PUSHI, iSize);						// The sizeOfArray to allocate is equal to the number of array elements.
		EMIT_1(OPCODE::PUSHI, sizeOf(sType));				// Push variable TYPE onto the STACK as it will be
		EMIT_1(OPCODE::MUL, 0);								// Multiplied with the sizeOfArray.

		EMIT_1(OPCODE::MALLOC, 0);							// MALLOC will pull the amount of bytes to allocate from the STACK & reserve memory on heap.
															// The address of allocated memory location will be pushed onto the STACK.

		EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));	// Store the memory address in the array variable.
	}

	// int32_t arr[8] = {10, 20, 30, 40};
	//		==> arr[0]		= 10; ------|
	//		==> arr[1]		= 20;		|_____ (I)
	//		==> arr[2]		= 30;		|
	//		==> arr[3]		= 40;-------|
	//		==> arr[5..7]	= 0; ------------- (II)

	// Initialize array elements
	if (pArrayElementsLeaf != nullptr)
	{
		// (I)
		int32_t iCount = 0;
		for (Tree* pArrayElement : pArrayElementsLeaf->m_vStatements)
		{
			if (iCount < iSize)
			{
				populateCode(pArrayElement);

				storeValueAtPosForVariable(iCount, sType.c_str(), pNode->m_sText.c_str());
				iCount++;
			}
		}

		// (II)
		{
			// arr[5..7] = 0; ------------ - (II)

			std::string sVariableName = pNode->m_sText;
			int32_t iOperand1_PointerVariable = GET_VARIABLE_POSITION(sVariableName.c_str());	// 1. "arr" position in heap
			int32_t iOperand2_StartPos = iCount;												// 2. '5'
			int32_t iOperand3_LastPos = iSize;													// 3. Count (in this case, 3 i.e for 5, 6, 7)
			int32_t iOperand4_Value = 0;														// 4. '0'
			int32_t iOperand5_ArrayType = sizeOf(sType);										// 5. sizeof(Type) i.e bytes to get to access subsequent "arr" elements.
			int32_t iOperand6_CastValue = castValueFor(sType);									// 6. Cast Value of Type "arr" to perform relevant 'CAST'

			std::cout	<< CURRENT_OFFSET << ". " << opCodeMap[(int)OPCODE::CLR].sOpCode 
						<< " " << iOperand1_PointerVariable
						<< " " << iOperand2_StartPos
						<< " " << iOperand3_LastPos
						<< " " << iOperand4_Value
						<< " " << iOperand5_ArrayType
						<< " " << iOperand6_CastValue
						<< std::endl;

			EMIT_BYTE(OPCODE::CLR);
			EMIT_INT(iOperand1_PointerVariable);
			EMIT_INT(iOperand2_StartPos);
			EMIT_INT(iOperand3_LastPos);
			EMIT_INT(iOperand4_Value);
			EMIT_INT(iOperand5_ArrayType);
			EMIT_INT(iOperand6_CastValue);
		}
	}
}

void GrammerUtils::handlePrimitivePtrEpilogue(Tree* pNode)
{
	EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));
}

void GrammerUtils::handleAssign(Tree* pNode)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;
	Tree* pIdentifiedNode = pNode->m_pRightNode;	// Remember we have added expression node(rvalue) to any parent's Left.
													// In case of ASSIGN, right node will be the lvalue.

	// Compute PreFixExpr
	{
		handlePreFixExpression(pExpressionNode->m_pLeftNode);
	}

	populateCode(pExpressionNode);

	std::string sType = GET_VARIABLE_NODETYPE(pNode->m_sText.c_str());
	// Cast only int8_t, int16_t, int32_t, @DEREF assignments to their respective 'TYPES'.
	// DO NOT cast pointer assignments ie.
	//		int32_t* pPtr0, pPtr1;
	//		pPtr0 = pPtr1;	// DO NOT cast this assignment.
							// as its an address which shouldn't down casted.

	ASTNodeType	eIdentifierNodeASTNodeType = pIdentifiedNode->m_eASTNodeType;
	{
		switch (eIdentifierNodeASTNodeType)
		{
			case ASTNodeType::ASTNode_IDENTIFIER:
			{
				if (NOT IS_POINTER_TYPE(pNode->m_sText.c_str()))
					cast(castValueFor(sType));
				EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));
			}
			break;
			case ASTNodeType::ASTNode_DEREF:
			{
				// Push a 'fake' ArrayIndex of '0' onto the STACK i.e
				// @pVar = iRValue; ==> @pVar[0] = iRValue;
				storeValueAtPosForVariable(0, sType.c_str(), pNode->m_sText.c_str());
			}
			break;
			case ASTNodeType::ASTNode_DEREFARRAY:
			{
				Tree* pDerefExpressionLeaf = pIdentifiedNode->m_pLeftNode;
				if (pDerefExpressionLeaf != nullptr)
				{
					cast(castValueFor(sType));					// Perform relevant 'CAST'

					populateCode(pDerefExpressionLeaf);			// ArrayIndex pushed as an expression.
																// which will be read from STACK @ runtime.
																// @pVar[index] = iRValue;

					EMIT_1(OPCODE::PUSHI, sizeOf(sType));		// Push variable TYPE onto the STACK as it will be
																// required to access the array pointer.

					EMIT_1(OPCODE::STA, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));
				}
			}
			break;
			case ASTNodeType::ASTNode_MEMBERACCESS:
			{
				std::string sObjectName = pNode->m_sText;
				std::string sVariableName = pIdentifiedNode->m_sText;
				std::string sStructType = GET_VARIABLE_NODETYPE(sObjectName.c_str());
				StructInfo* pStructInfo = m_MapGlobalStructs[sStructType];
				assert(pStructInfo != nullptr);
				if (pStructInfo != nullptr)
				{
					// 1. Fetch 'this' into 'ECX'
					EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(sObjectName.c_str()));		// Get the 'this' pointer value
					EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// & push it in 'ECX'

					std::string sFullyQualifiedVariableName = sStructType + "_" + sVariableName;
					Tree* pVariableNode = pStructInfo->getMemberVariableASTNode(sFullyQualifiedVariableName.c_str());
					sType = pVariableNode->getAdditionalInfoFor("type");
					int32_t iPosition = pStructInfo->getMemberVariablePosition(sFullyQualifiedVariableName.c_str());

					// 2. Perform relevant cast before storing the value in objects variable.
					cast(castValueFor(sType));					// Perform relevant 'CAST'
					EMIT_1(OPCODE::STORE, iPosition);			// Store the rvalue in the variable.
				}
			}
			break;
		}
	}

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

void GrammerUtils::handleMalloc(Tree* pNode)
{
	Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
	populateCode(pExpressionNode);					// The amount of bytes to allocate will be calculated at runtime & pushed onto the STACK.

	EMIT_1(OPCODE::MALLOC, 0);						// MALLOC will pull the amount of bytes to allocate from the STACK & reserve memory on heap.
													// The address of allocated memory location will be pushed onto the STACK.
}

void GrammerUtils::handleFree(Tree* pNode)
{
	////////////////////////////////////////////////////////////////////
	// I. Search for the 'Pointer Variable' AST Node; 
	////////////////////////////////////////////////////////////////////
	std::string sPointerName = pNode->m_sText;
	Tree* pPointerNode = nullptr;
	if (m_pCurrentFunction != nullptr)
	{
		// 1. Search through the Local variable list.
		for (Tree* pLocalVar : m_pCurrentFunction->m_vLocalVariables)
		{
			if (pLocalVar->m_sText == sPointerName)
			{
				pPointerNode = pLocalVar;
				break;
			}
		}

		// 2. Search through the Static variable list.
		if (pPointerNode == nullptr)
		{
			for (Tree* pStaticVar : m_pCurrentFunction->m_vStaticVariables)
			{
				if (pStaticVar->m_sText == sPointerName)
				{
					pPointerNode = pStaticVar;
					break;
				}
			}
		}
	}

	if (pPointerNode == nullptr)
	{
		if (m_pCurrentStruct != nullptr)
		{
			// 3. Search through the Member variable list.
			for (Tree* pMemberVar : m_pCurrentStruct->m_vMemberVariables)
			{
				if (pMemberVar->m_sText == sPointerName)
				{
					pPointerNode = pMemberVar;
					break;
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////
	// II. Once found emit asm; 
	////////////////////////////////////////////////////////////////////
	assert(pPointerNode != nullptr);
	if (pPointerNode != nullptr)			// Pointer Variable AST found !
	{
		std::string sType = pPointerNode->getAdditionalInfoFor("type");
		if (sType == "int8_t"
			||
			sType == "int16_t"
			||
			sType == "int32_t"
		) {
			EMIT_1(OPCODE::FREE, GET_VARIABLE_POSITION(sPointerName.c_str()));
		}
		else
		{
			// 1. Fetch 'this' into 'ECX'
			EMIT_1(OPCODE::FETCH, GET_VARIABLE_POSITION(sPointerName.c_str()));		// Get the 'this' pointer value
			EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// & push it in 'ECX'
																					
			// 2. Call the 'Destructor' by creating a Dummy Destructor under the "free" ASTNode.
			std::string sDestructor = "#" + sType;
			Tree* pDefaultDestructor = createFunctionCallWithNoArguments(sDestructor.c_str());
			pDefaultDestructor->setAdditionalInfo("memberFunctionOf", sType);
			pNode->addChild(pDefaultDestructor);

			populateCode(pDefaultDestructor);

			// 3. Free the pointer itself that holds 'this' object.
			EMIT_1(OPCODE::FREE, GET_VARIABLE_POSITION(sPointerName.c_str()));

			// 4. Clear off 'ECX' that holds the address of 'this'
			EMIT_1(OPCODE::PUSH, 0);
			EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// Clear off 'ECX'.
		}
	}
}

void GrammerUtils::handleStatics(Tree* pNode)
{
	FunctionInfo::addStaticVariable(pNode);
}

void GrammerUtils::handleTypeStructs(Tree* pNode)
{
	//////////////////////////////////////////////////////////////////
	// 1. Allocate Memory on HEAP for the sizeOf(STRUCT)
	//		- Remember all the object allocations happen on the HEAP & not on the STACK.
	//		- No STACK based BUFFER OVERFLOW(BO) possible.
	//		- The objects once allocated on the HEAP, should be freed when they go out of scope.
	//////////////////////////////////////////////////////////////////
	std::string sType = pNode->getAdditionalInfoFor("type");
	EMIT_1(OPCODE::PUSHI, sizeOf(sType));									// Push variable size of STRUCT onto the STACK as it will be pulled in by MALLOC @ RT.

	EMIT_1(OPCODE::MALLOC, 0);												// MALLOC will pull the amount of bytes to allocate from the STACK & reserve memory on the HEAP.
																			// The address of allocated memory location will be pushed onto the STACK.

	EMIT_1(OPCODE::POPR, EREGISTERS::RCX);									// Save the Memory Address Loc in 'ECX', also called as "this" pointer.
	EMIT_1(OPCODE::PUSHR, EREGISTERS::RCX);									// Push the Memory Address again onto the STACK to STORE it in a variable.

	EMIT_1(OPCODE::STORE, GET_VARIABLE_POSITION(pNode->m_sText.c_str()));	// Store the memory address in the pointer(STRUCT*) variable.

	Tree* pConstructorCallNode = pNode->m_vStatements[0];
	if (pConstructorCallNode != nullptr && pConstructorCallNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONCALL)
	{
		populateCode(pConstructorCallNode);
	}

	EMIT_1(OPCODE::PUSHI, 0);												// Clear off 'ECX'.
	EMIT_1(OPCODE::POPR, EREGISTERS::RCX);
}

void GrammerUtils::handleStatements(Tree* pNode)
{
	int32_t iCount = 0;
	for(; iCount < pNode->m_vStatements.size(); iCount++)
	{
		Tree* pChildNode = pNode->m_vStatements.at(iCount);

		////////////////////////////////////////////////////////////////////////////
		if (pNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONCALL)
		{
			// Bail out for the function arguments.
			// They will be processed in 'ASTNode_FUNCTIONCALLEND'
			if (pChildNode->m_eASTNodeType != ASTNodeType::ASTNode_FUNCTIONCALLEND)
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

int32_t GrammerUtils::sizeOf(std::string sType)
{
	if (sType == "int8_t")
		return sizeof(int8_t);
	else
	if (sType == "int16_t")
		return sizeof(int16_t);
	else
	if (sType == "int32_t")
		return sizeof(int32_t);
	else
	{
		std::map<std::string, StructInfo*>::const_iterator itrStruct = m_MapGlobalStructs.begin();
		for (; itrStruct != m_MapGlobalStructs.end(); ++itrStruct)
		{
			std::string sStructName = itrStruct->first;
			if (sStructName == sType)
			{
				StructInfo* pStructInfo = itrStruct->second;
				return pStructInfo->sizeOf();
			}
		}
	}

	return 1;
}

int32_t GrammerUtils::castValueFor(std::string sType)
{
	// Cast the expression with relevant cast
	if (sType == "int8_t")
		return 0xFF;
	else
	if (sType == "int16_t")
		return 0xFFFF;
	else
	if (sType == "int32_t")
		return 0xFFFFFFFF;

	return 0;
}

void GrammerUtils::storeValueAtPosForVariable(int32_t iPos, const char* sType, const char* sVariableName)
{
	cast(castValueFor(sType));					// Perform relevant 'CAST'

	EMIT_1(OPCODE::PUSHI, iPos);				// Push a ArrayIndex of iPos onto the STACK i.e
												// @pVar = iRValue; ==> @pVar[iPos] = iRValue;

	EMIT_1(OPCODE::PUSHI, sizeOf(sType));		// Push variable TYPE onto the STACK as it will be
												// required to access the array pointer.

	EMIT_1(OPCODE::STA, GET_VARIABLE_POSITION(sVariableName));
}

void GrammerUtils::cast(int32_t iCastValue)
{
	// Cast the expresseion with relevant Type Value
	EMIT_1(OPCODE::PUSHI, iCastValue);
	EMIT_1(OPCODE::BITWISEAND, 0);
}

StructInfo* GrammerUtils::getStructOfObject(std::string sObjectName)
{
	StructInfo* pStructInfo = nullptr;
	std::string sStructType = GET_VARIABLE_NODETYPE(sObjectName.c_str());
	if (NOT sStructType.empty())
	{
		pStructInfo = m_MapGlobalStructs[sStructType];
	}

	return pStructInfo;
}

void GrammerUtils::printHeaders(RandomAccessFile* pRaf, std::vector<std::string>& vStrings)
{
	// Write String info
	pRaf->writeByte(vStrings.size());
	for (std::string sString : vStrings)
	{
		pRaf->writeByte(sString.length());
		pRaf->write(sString.c_str());
	}

	// Write Global(Static) variable info
	pRaf->writeInt(FunctionInfo::m_vStaticVariables.size());
}

void GrammerUtils::printAssembly(int8_t* iByteCode, std::vector<std::string>& vStrings)
{
	RandomAccessFile* pRaf = new RandomAccessFile();
	bool bCanWrite = pRaf->openForWrite("main.o");

	if (bCanWrite)
	{
		printHeaders(pRaf, vStrings);
	}

	m_pBAIS->reset();
	OPCODE eOpCode = OPCODE::NOP;
	while (true)
	{
		std::cout << CURRENT_OFFSET_READ;

		eOpCode = (OPCODE)m_pBAIS->readByte();
		if (bCanWrite)
			pRaf->writeByte((int)eOpCode);

		CodeMap pMachineInstruction = opCodeMap[(int)eOpCode];

		// Print to stdout
		{
			std::cout << ". " << pMachineInstruction.sOpCode;

			for (int i = 1; i < pMachineInstruction.iOpcodeOperandCount; i++)
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
