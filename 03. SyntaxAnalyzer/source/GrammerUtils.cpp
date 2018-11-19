#include "GrammerUtils.h"

Token						GrammerUtils::m_pToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);
Token						GrammerUtils::m_pPrevToken(TokenType::Type::TK_UNKNOWN, "", -1, -1);

std::vector<std::string>	GrammerUtils::m_vKeywords;

StringTokenizer*			GrammerUtils::m_pStrTok = NULL;
int							GrammerUtils::iTabCount = -1;
std::vector<std::string>	GrammerUtils::m_vVariables;
std::vector<std::string>	GrammerUtils::m_vStrings;
int							GrammerUtils::m_iByteCode[MAX_BYTECODE_SIZE];

#define SHOW_VERBOSE_MORE 1

struct CodeMap
{
	const char*		sOpCode;
	OPCODE			eOpCode;
	int				iOpcodeOperandCount;
} opCodeMap[] =
{
	{ "NOP",		OPCODE::NOP,		1},
	{ "FETCH",		OPCODE::FETCH,		2},
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
	{ "HLT",		OPCODE::HLT,		1 },
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

	if (	m_pToken.getType() == TokenType::Type::TK_EOL
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
		case ASTNodeType::ASTNode_ASSIGN:
		{
			std::cout << pRightNode->m_sText << " = " << pLeftNode->m_sText << ";";
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
			std::cout << "put(";
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
		}
		break;
		case ASTNodeType::ASTNode_EXPRESSION:
		{
			std::cout << pNode->m_sText << " ";
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

			printTabs(); std::cout << "}" << std::endl;
		}
		break;
		case ASTNodeType::ASTNode_WHILE:
		{
			if (iTabCount >= 0)
				iTabCount--;

			printTabs(); std::cout << "}" << std::endl;
		}
		break;
	}

	std::cout << std::endl;
}

void GrammerUtils::generateCode(Tree* pRootNode)
{
	printAST(pRootNode);
	
	/*
	populateVariables(pRootNode, m_vVariables);
	int iDataSize = m_vVariables.size();

	populateStrings(pRootNode, m_vStrings);
	int iStrings = m_vStrings.size();

	int iOffset = 0;
	{
		populateCode(pRootNode, m_iByteCode, iOffset);
		emit((int)OPCODE::HLT, m_iByteCode, iOffset);
			std::cout << (iOffset - 1) << ". " << "HLT" << std::endl;

		iOffset = 0;
		printAssembly(m_iByteCode, iOffset, m_vVariables, m_vStrings);
	}
	//*/
}

void GrammerUtils::populateVariables(Tree* pParentNode, std::vector<std::string>& sVector)
{
	if(pParentNode != nullptr)
	{
		std::vector<Tree*> vStatements = pParentNode->m_vStatements;
		for (Tree* pNode : vStatements)
		{
			switch (pNode->m_eASTNodeType)
			{
				case ASTNodeType::ASTNode_ASSIGN:
				{
					addString(pNode->m_pRightNode->m_sText, sVector);
				}
				break;
				case ASTNodeType::ASTNode_IF:
				{
					populateVariables(pNode, sVector);
					Tree* pElseNode = pNode->m_pRightNode;
					if (pElseNode != nullptr)
					{
						populateVariables(pElseNode, sVector);
					}
				}
				break;
				case ASTNodeType::ASTNode_WHILE:
				{
					populateVariables(pNode, sVector);
				}
				break;
			}
		}
	}
}

void GrammerUtils::populateStrings(Tree* pParentNode, std::vector<std::string>& sVector)
{
	if(pParentNode != nullptr)
	{
		std::vector<Tree*> vStatements = pParentNode->m_vStatements;
		for (Tree* pNode : vStatements)
		{
			switch (pNode->m_eASTNodeType)
			{
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
				case ASTNodeType::ASTNode_WHILE:
				{
					populateStrings(pNode, sVector);
				}
				break;
				case ASTNodeType::ASTNode_PRINT:
				{
					populateStrings(pNode, sVector);
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
	if(pNode != nullptr)
	{
		int i_IfWhile_JCondition_Hole = 0, i_While_Loop_Hole = 0, i_ElseEnd_JMP_Offset = 0;
		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_CHARACTER:
			{
				emit((int)OPCODE::PUSH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "PUSH ";
#endif
				char pStr[255] = { 0 };
				sprintf_s(pStr, "%d", pNode->m_sText.c_str()[0]);

				int ch = atoi(pStr);
				emit(ch, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << atoi(pNode->m_sText.c_str()) << std::endl;
#endif
			}
			break;
			case ASTNodeType::ASTNode_INTEGER:
			{
				emit((int)OPCODE::PUSH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "PUSH ";
#endif

				emit(atoi(pNode->m_sText.c_str()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << atoi(pNode->m_sText.c_str()) << std::endl;
#endif
			}
			break;
			case ASTNodeType::ASTNode_IDENTIFIER:
			{
				emit((int)OPCODE::FETCH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "FETCH ";
#endif

				emit(getVariablePosition(pNode->m_sText.c_str()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << getVariablePosition(pNode->m_sText.c_str()) << std::endl;
#endif
			}
			break;
			case ASTNodeType::ASTNode_STRING:
			{
				emit((int)OPCODE::PUSH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "PUSH ";
#endif

				emit(getStringPosition(pNode->m_sText.c_str()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << getStringPosition(pNode->m_sText.c_str()) << std::endl;
#endif
			}
			break;
			case ASTNodeType::ASTNode_EXPRESSION:
			{
				std::string sRValuePostFixExpression = pNode->m_sText;
				StringTokenizer* st = StringTokenizer::create(sRValuePostFixExpression.c_str());
				st->tokenize();
				while (st->hasMoreTokens())
				{
					Token tok = st->nextToken();
					TokenType::Type eTokenType = tok.getType();
					if (eTokenType == TokenType::Type::TK_WHITESPACE || eTokenType == TokenType::Type::TK_EOI || eTokenType == TokenType::Type::TK_COMMA)
						continue;

					switch (tok.getType())
					{
						case TokenType::Type::TK_CHARACTER:
						{
							emit((int)OPCODE::PUSH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "PUSH ";
#endif
							char pStr[255] = { 0 };
							sprintf_s(pStr, "%d", pNode->m_sText.c_str()[0]);

							int ch = atoi(pStr);
							emit(ch, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << atoi(pNode->m_sText.c_str()) << std::endl;
#endif
						}
						break;
						case TokenType::Type::TK_INTEGER:
						{
							emit((int)OPCODE::PUSH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "PUSH ";
#endif

							emit(atoi(tok.getText()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << atoi(tok.getText()) << std::endl;
#endif
						}
						break;
						case TokenType::Type::TK_IDENTIFIER:
						{
							emit((int)OPCODE::FETCH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "FETCH ";
#endif

							emit(getVariablePosition(tok.getText()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << getVariablePosition(tok.getText()) << std::endl;
#endif
						}
						break;
						case TokenType::Type::TK_STRING:
						{
							emit((int)OPCODE::PUSH, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "PUSH ";
#endif

							emit(getStringPosition(tok.getText()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << getStringPosition(tok.getText()) << std::endl;
#endif
						}
						break;
						case TokenType::Type::TK_MUL:
							emit((int)OPCODE::MUL, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "MUL" << std::endl;
#endif
							break;
						case TokenType::Type::TK_DIV:
							emit((int)OPCODE::DIV, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "DIV" << std::endl;
#endif
							break;
						case TokenType::Type::TK_MOD:
							emit((int)OPCODE::MOD, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "MOD" << std::endl;
#endif
							break;
						case TokenType::Type::TK_ADD:
							emit((int)OPCODE::ADD, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "ADD" << std::endl;
#endif
							break;
						case TokenType::Type::TK_SUB:
							emit((int)OPCODE::SUB, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "SUB" << std::endl;
#endif
							break;
						case TokenType::Type::TK_LT:
							emit((int)OPCODE::JMP_LT, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "JMP_LT" << std::endl;
#endif
							break;
						case TokenType::Type::TK_LTEQ:
							emit((int)OPCODE::JMP_LTEQ, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "JMP_LTEQ" << std::endl;
#endif
							break;
						case TokenType::Type::TK_GT:
							emit((int)OPCODE::JMP_GT, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "JMP_GT" << std::endl;
#endif
							break;
						case TokenType::Type::TK_GTEQ:
							emit((int)OPCODE::JMP_GTEQ, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "JMP_GTEQ" << std::endl;
#endif
							break;
						case TokenType::Type::TK_EQ:
							emit((int)OPCODE::JMP_EQ, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "JMP_EQ" << std::endl;
#endif
							break;
						case TokenType::Type::TK_NEQ:
							emit((int)OPCODE::JMP_NEQ, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "JMP_NEQ" << std::endl;
#endif
							break;
						case TokenType::Type::TK_LOGICALAND:
							emit((int)OPCODE::LOGICALAND, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "LOGICALAND" << std::endl;
#endif
							break;
						case TokenType::Type::TK_LOGICALOR:
							emit((int)OPCODE::LOGICALOR, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "LOGICALOR" << std::endl;
#endif
							break;
						case TokenType::Type::TK_NOT:
							emit((int)OPCODE::_NOT, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "NOT" << std::endl;
#endif
							break;
					}
				}
			}
			break;
			case ASTNodeType::ASTNode_ASSIGN:
			{
				Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
																// In case of ASSIGN, right node will be the lvalue.
				populateCode(pExpressionNode, pByteCode, iOffset);

				emit((int)OPCODE::STORE, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "STORE ";
#endif

				emit(getVariablePosition(pNode->m_pRightNode->m_sText.c_str()), pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << getVariablePosition(pNode->m_pRightNode->m_sText.c_str()) << std::endl;
#endif
			}
			break;
			case ASTNodeType::ASTNode_IF:
			case ASTNodeType::ASTNode_WHILE:
			{
				Tree* pExpressionNode = pNode->m_pLeftNode;		// Remember we have added expression node(rvalue) to any parent's Left.
				i_While_Loop_Hole = iOffset;

				populateCode(pExpressionNode, pByteCode, iOffset);
				
				emit((int)OPCODE::JZ, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "JZ" << std::endl;
#endif
				i_IfWhile_JCondition_Hole = iOffset++;
			}
			break;
		}

		std::vector<Tree*> vStatements = pNode->m_vStatements;
		for (Tree* pChildNode : vStatements)
		{
			ASTNodeType eASTNodeType = pNode->m_eASTNodeType;
			populateCode(pChildNode, pByteCode, iOffset);

			switch (eASTNodeType)
			{
				case ASTNodeType::ASTNode_PUTC:
				{
					emit((int)OPCODE::PRTC, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
					std::cout << (iOffset - 1) << ". " << "PRTC" << std::endl;
#endif
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
							emit((int)OPCODE::PRTI, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "PRTI" << std::endl;
#endif
							break;
						case ASTNodeType::ASTNode_STRING:
							emit((int)OPCODE::PRTS, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "PRTS" << std::endl;
#endif
							break;
						case ASTNodeType::ASTNode_CHARACTER:
							emit((int)OPCODE::PRTC, pByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
							std::cout << (iOffset - 1) << ". " << "PRTC" << std::endl;
#endif
							break;
					}
				}
				break;
			}
		}

		switch (pNode->m_eASTNodeType)
		{
			case ASTNodeType::ASTNode_IF:
			{
				Tree* pElseNode = pNode->m_pRightNode;
				emit((int)OPCODE::JMP, m_iByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "JMP" << std::endl;
#endif

				i_ElseEnd_JMP_Offset = iOffset++;

				emit(iOffset, m_iByteCode, i_IfWhile_JCondition_Hole);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << "------" << "i_IfWhile_JCondition_Hole [" << i_IfWhile_JCondition_Hole << "] = " << iOffset << std::endl;
#endif

				if (pElseNode != nullptr)
				{
					populateCode(pElseNode, pByteCode, iOffset);
				}

				emit(iOffset, m_iByteCode, i_ElseEnd_JMP_Offset);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << "------" << "i_ElseEnd_JMP_Offset [" << i_ElseEnd_JMP_Offset << "] = " << iOffset << std::endl;
#endif
			}
			break;
			case ASTNodeType::ASTNode_WHILE:
			{
				emit((int)OPCODE::JMP, m_iByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << (iOffset - 1) << ". " << "JMP" << std::endl;
#endif
				emit(i_While_Loop_Hole, m_iByteCode, iOffset++);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << "------" << "i_While_Loop_Hole [" << (iOffset-1) << "] = " << i_While_Loop_Hole << std::endl;
#endif

				emit(iOffset, m_iByteCode, i_IfWhile_JCondition_Hole);
#if (SHOW_VERBOSE_MORE == 1)
				std::cout << "------" << "i_IfWhile_JCondition_Hole [" << i_IfWhile_JCondition_Hole << "] = " << iOffset << std::endl;
#endif
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

void GrammerUtils::printHeaders(RandomAccessFile* pRaf, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings)
{
	pRaf->writeByte(m_vVariables.size());
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
	if(bCanWrite)
	{
		printHeaders(pRaf, vVariables, vStrings);

		OPCODE eOpcode = OPCODE::NOP;
		while(true)
		{
			eOpcode = (OPCODE)iByteCode[iOffset];
			CodeMap pMachineInstruction = opCodeMap[(int)eOpcode];

			// Print to stdout
			{
				std::cout << iOffset << ". " << pMachineInstruction.sOpCode;
				
				if (pMachineInstruction.iOpcodeOperandCount == 2)
				{
					std::cout << " " << iByteCode[iOffset + 1];
				}
				std::cout << std::endl;
			}

			// Print to file
			{
				pRaf->writeInt((int)pMachineInstruction.eOpCode);
				if (pMachineInstruction.iOpcodeOperandCount == 2)
				{
					pRaf->writeInt(iByteCode[iOffset + 1]);
				}
			}

			iOffset += pMachineInstruction.iOpcodeOperandCount;

			if (eOpcode == OPCODE::HLT)
				break;
		}
	}

	pRaf->close();
}
