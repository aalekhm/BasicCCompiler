#include "BNFToCpp.h"
#include "NonTerminal.h"

#define WRITE_LINE(__rafOut__, __line__) __rafOut__->writeLine(__line__);
#define WRITE_BUF(__rafOut__, __line__) __rafOut__->write(__line__);

BNFToCpp::BNFToCpp()
{
}

void BNFToCpp::process(const char* sGrammerFile, const char* sClassName)
{
	m_sClassName = sClassName;

	if (tokenize(sGrammerFile))
	{
		nonTerminalize();
		generateDotH();
		generateDotCPP();
	}
}

bool BNFToCpp::tokenize(const char* sGrammerFile)
{
	bool bSuccess = false;
	RandomAccessFile* pRaf = nullptr;
	{
		pRaf = new RandomAccessFile();
		bSuccess = pRaf->openForRead(sGrammerFile);
		if (bSuccess)
		{
			unsigned long iLength = pRaf->length();
			char* sBuff = new char[iLength + strlen("\r\n") + 1];
			memset(sBuff, 0, iLength);

			unsigned long iBytesRead = pRaf->read(sBuff);
			if (iBytesRead > 0)
			{
				StringTokenizer* st = StringTokenizer::create(sBuff, false);
				st->tokenize();
				while (st->hasMoreTokens())
				{
					Token tok = st->nextToken();

					//if (!(tok.getType() == TokenType::Type::TK_EOL || tok.getType() == TokenType::Type::TK_WHITESPACE))
					//	std::cout << "BNFToCpp::tokenize() << Token :: " << TokenType::toString(tok.getType()) << "\t" << tok.getText() << std::endl;

					m_vTokens.push_back(tok);
				}
			}

			delete[] sBuff;
			sBuff = nullptr;
		}

		pRaf->close();
	}

	return bSuccess;
}

void BNFToCpp::nonTerminalize()
{
	NonTerminal* pCurrentNonTerminal = nullptr;
	bool bAddToken = true;
	for (Token tok : m_vTokens)
	{
		bAddToken = true;

		TokenType::Type eTokenType = tok.getType();
		switch (eTokenType)
		{
			case TokenType::Type::TK_BNFNONTERMINAL:
			{
				if (pCurrentNonTerminal == nullptr)
				{
					NonTerminal* pNonTerminal = NonTerminal::create(tok);
					pCurrentNonTerminal = pNonTerminal;
					m_vNonterminals.push_back(pNonTerminal);

					continue;
				}
			}
			break;
			case TokenType::Type::TK_STRING:
			{
				if (strcmp(tok.getText(), "") == 0)
					continue;

				m_vKeywords.push_back(tok);
			}
			break;
			case TokenType::Type::TK_WHITESPACE:
			case TokenType::Type::TK_EOL:
			case TokenType::Type::TK_EOI:
			case TokenType::Type::TK_SINGLELINECOMMENT:
			case TokenType::Type::TK_MULTILINECOMMENT:
			case TokenType::Type::TK_BNFASSIGNMENT:
			{
				continue;
			}
			break;
		}

		if (bAddToken)
		{
			pCurrentNonTerminal->addToken(tok);
		}

		if(eTokenType == TokenType::Type::TK_SEMICOL)
			pCurrentNonTerminal = nullptr;
	}
}

void BNFToCpp::generateDotH()
{
	RandomAccessFile* rafOut = new RandomAccessFile();
	std::string sHeaderFileName = m_sClassName + ".h";

	if (rafOut->openForWrite(sHeaderFileName.c_str()))
	{
		char sLine[255] = { 0 };
		{
			sprintf(sLine, "%sDotHIncludes.txt\0", m_sClassName.c_str());
			writeFromFile(rafOut, sLine);
		}
		
		sprintf(sLine, "class %s {", m_sClassName.c_str());	WRITE_LINE(rafOut, sLine);
		sprintf(sLine, "\tpublic:");						WRITE_LINE(rafOut, sLine);
		sprintf(sLine, "\t\t%s();", m_sClassName.c_str());	WRITE_LINE(rafOut, sLine);
															WRITE_LINE(rafOut, "");

		int iCount = m_vKeywords.size();
		// Keywords List
		{
			sprintf(sLine, "\t\tvoid addKeywords() {");		WRITE_LINE(rafOut, sLine);
			for (int i = 0; i < iCount; i++)
			{
				Token tok = m_vKeywords.at(i);
				sprintf(sLine, "\t\t\tGrammerUtils::m_vKeywords.push_back(\"%s\");", tok.getText());
				WRITE_LINE(rafOut, sLine);
			}
			WRITE_LINE(rafOut, "\t\t}\r\n");
		}

		iCount = m_vNonterminals.size();
		// NonTerminals as methods.
		{
			for (int i = 0; i < iCount; i++) 
			{
				NonTerminal* pNonTerminal = m_vNonterminals.at(i);
				Token tok = pNonTerminal->get();

				sprintf(sLine, "\t\tbool %s();", tok.getText());
				WRITE_LINE(rafOut, sLine);
			}
			WRITE_LINE(rafOut, "");
		}
		
		{
			sprintf(sLine, "%sDotHDeclares.txt\0", m_sClassName.c_str());
			writeFromFile(rafOut, sLine);
		}

		sprintf(sLine, "};");		WRITE_LINE(rafOut, sLine);
	}

	rafOut->close();
}

void BNFToCpp::generateDotCPP()
{
	RandomAccessFile* rafOut = new RandomAccessFile();
	std::string sCppFileName = m_sClassName + ".cpp";
	
	if (rafOut->openForWrite(sCppFileName.c_str()))
	{
		char sLine[255] = { 0 };
		{
			sprintf(sLine, "%sDotCppDefinitions.txt\0", m_sClassName.c_str());
			writeFromFile(rafOut, sLine);
			WRITE_LINE(rafOut, "");
		}

		int iCount = m_vNonterminals.size();
		for (int i = 0; i < iCount; i++)
		{
			NonTerminal* pNonTerminal = m_vNonterminals[i];
			const std::vector<Token>& vTokenList = pNonTerminal->getTokenList();

			onTokenCallback(rafOut, m_sClassName.c_str(), pNonTerminal->get());

			int iTokenListSize = vTokenList.size();
			for (int j = 0; j < iTokenListSize; j++)
			{
				Token tok = vTokenList[j];
				onTokenCallback(rafOut, m_sClassName.c_str(), tok);
			}
		}
	}

	rafOut->close();
}

void BNFToCpp::writeFromFile(RandomAccessFile* rafOutDest, const char* sSrcFileName)
{
	RandomAccessFile* rafIn = new RandomAccessFile();
	if (rafIn->openForRead(sSrcFileName))
	{
		char sLine[255] = { 0 };

		while ( NOT rafIn->isEOF() ) 
		{
			sprintf(sLine, rafIn->readLine().c_str());	WRITE_LINE(rafOutDest, sLine);
		}
	}

	rafIn->close();
}

void BNFToCpp::onTokenCallback(RandomAccessFile* rafOut, const char* sClassName, Token tok)
{
	char sLine[4096] = { 0 };
	TokenType::Type eTokenType = tok.getType();
	std::string sValue = tok.getText();

	switch(eTokenType)
	{
		case TokenType::Type::TK_BNFNONTERMINAL:
		{
			if (m_eGrammerState == EGrammerState::WRITING_FUNCTION_START)
			{
				sprintf(sLine, "bool %s::%s() {", sClassName, sValue.c_str());
				WRITE_LINE(rafOut, sLine);

				m_eGrammerState = EGrammerState::WRITING_FUNCTION;
			}
			else
			if(m_eGrammerState == EGrammerState::WRITING_FUNCTION)
			{
				if (m_bOptional) 
				{
					sprintf(sLine, "if(!%s()) {", sValue.c_str());	WRITE_LINE(rafOut, sLine);
					sprintf(sLine, "}");							WRITE_LINE(rafOut, sLine);
					sprintf(sLine, "else {");						WRITE_LINE(rafOut, sLine);

					m_bOptional = false;
				}
				else
				if (m_bWhile)
				{
					sprintf(sLine, "if(%s()) {", sValue.c_str());	WRITE_LINE(rafOut, sLine);
					m_bWhile = false;
				}
				else
				if (m_bORed || m_bORedFirst)
				{
					if (m_bORedFirst)
					{
						m_bORedFirst = false;
					}
					else 
					{
						sprintf(sLine, "return true;", sValue.c_str());	WRITE_LINE(rafOut, sLine);
						sprintf(sLine, "}\r\nelse");					WRITE_LINE(rafOut, sLine);
					}

					sprintf(sLine, "if(%s()) {", sValue.c_str());		WRITE_LINE(rafOut, sLine);
				}
				else 
				{
					sprintf(sLine, "if(!%s())\r\nreturn false;", sValue.c_str());	WRITE_LINE(rafOut, sLine);
				}
			}
		}
		break;
		case TokenType::Type::TK_BNFCODE:
		{
			sprintf(sLine, "%s", sValue.c_str());	WRITE_LINE(rafOut, sLine);
		}
		break;
		case TokenType::Type::TK_STRING:
		case TokenType::Type::TK_IDENTIFIER:
			if (m_bOptional)
			{
				if (TokenType::Type::TK_INVALID == TokenType::fromString(sValue))
					sprintf(sLine, "if(!GrammerUtils::match(\"%s\", OPTIONAL)) {\r\n", sValue.c_str());
				else
					sprintf(sLine, "if(!GrammerUtils::match(TokenType::Type::%s, OPTIONAL)) {\r\n", sValue.c_str());
																							WRITE_LINE(rafOut, sLine);
				sprintf(sLine, "}\r\n");													WRITE_LINE(rafOut, sLine);
				sprintf(sLine, "else {\r\n");												WRITE_LINE(rafOut, sLine);

				m_bOptional = false;
			}
			else
			if (m_bWhile)
			{
				if (TokenType::Type::TK_INVALID == TokenType::fromString(sValue))
					sprintf(sLine, "if(GrammerUtils::match(\"%s\", OPTIONAL)) {\r\n", sValue.c_str());
				else
					sprintf(sLine, "if(GrammerUtils::match(TokenType::Type::%s, OPTIONAL)) {\r\n", sValue.c_str());
				WRITE_LINE(rafOut, sLine);
				m_bWhile = false;
			}
			else
			if (m_bORed || m_bORedFirst)
			{
				if (m_bORedFirst)
				{
					m_bORedFirst = false;
				}
				else 
				{
					sprintf(sLine, "return true;");		WRITE_LINE(rafOut, sLine);
					sprintf(sLine, "}\r\nelse");		WRITE_LINE(rafOut, sLine);
				}

				if (TokenType::Type::TK_INVALID == TokenType::fromString(sValue))
					sprintf(sLine, "if(GrammerUtils::match(\"%s\", OPTIONAL)) {", sValue.c_str());
				else
					sprintf(sLine, "if(GrammerUtils::match(TokenType::Type::%s, OPTIONAL)) {", sValue.c_str());
				WRITE_LINE(rafOut, sLine);
			}
			else 
			{
				if (TokenType::Type::TK_INVALID == TokenType::fromString(sValue))
					sprintf(sLine, "if(!GrammerUtils::match(\"%s\", MANDATORY))\r\nreturn false;", sValue.c_str());
				else
					sprintf(sLine, "if(!GrammerUtils::match(TokenType::Type::%s, MANDATORY))\r\nreturn false;", sValue.c_str());
				WRITE_LINE(rafOut, sLine);
			}
		break;
		case TokenType::Type::TK_CHARACTER:
		{
			if (m_bOptional)
			{
				sprintf(sLine, "if(!GrammerUtils::match(\'%c\', OPTIONAL)) {\r\n", sValue[0]);		WRITE_LINE(rafOut, sLine);
				sprintf(sLine, "}\r\n");															WRITE_LINE(rafOut, sLine);
				sprintf(sLine, "else {\r\n");														WRITE_LINE(rafOut, sLine);

				m_bOptional = false;
			}
			else
			if (m_bWhile)
			{
				sprintf(sLine, "if(GrammerUtils::match(\'%c\', OPTIONAL)) {\r\n", sValue[0]);		WRITE_LINE(rafOut, sLine);
				m_bWhile = false;
			}
			else
			if (m_bORed || m_bORedFirst)
			{
				if (m_bORedFirst)
				{
					m_bORedFirst = false;
				}
				else 
				{
					sprintf(sLine, "return true;");		WRITE_LINE(rafOut, sLine);
					sprintf(sLine, "}\r\nelse");		WRITE_LINE(rafOut, sLine);
				}

				sprintf(sLine, "if(GrammerUtils::match(\'%c\', OPTIONAL)) {", sValue[0]);	WRITE_LINE(rafOut, sLine);
			}
			else 
			{
				sprintf(sLine, "if(!GrammerUtils::match(\'%c\', MANDATORY))\r\nreturn false;", sValue[0]);	WRITE_LINE(rafOut, sLine);
			}
		}
		break;
		case TokenType::Type::TK_LBRACE:
			WRITE_LINE(rafOut, "while(true) {");
			m_bWhile = true;
		break;
		case TokenType::Type::TK_SEMICOL:
			WRITE_LINE(rafOut, "return true;\r\n");
			WRITE_LINE(rafOut, "}\r\n");

			m_eGrammerState = EGrammerState::WRITING_FUNCTION_START;
		break;
		case TokenType::Type::TK_LPAREN:
			m_bORedFirst = true;
		break;
		case TokenType::Type::TK_RBRACE:
			WRITE_LINE(rafOut, "}");
			WRITE_LINE(rafOut, "else\r\nbreak;");
			WRITE_LINE(rafOut, "}\r\n");
			m_bWhile = false;
		break;
		case TokenType::Type::TK_RPAREN:
			WRITE_LINE(rafOut, "return true;\r\n}");
			WRITE_LINE(rafOut, "else\r\nreturn false;\r\n");
			m_bORed = false;
		break;
		case TokenType::Type::TK_BITWISEOR:
			m_bORed = true;
		break;
		case TokenType::Type::TK_LSQBRACKET:
			m_bOptional = true;
		break;
		case TokenType::Type::TK_RSQBRACKET:
			WRITE_LINE(rafOut, "}\r\n");
			m_bOptional = false;
		break;
	}
}

BNFToCpp::~BNFToCpp()
{
}