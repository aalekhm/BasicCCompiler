#include "StringTokenizer.h"

StringTokenizer* StringTokenizer::create(const char* sData, bool bIgnoreBNFNonTerminals)
{
	StringTokenizer* pStringTokenizer = new StringTokenizer(sData, bIgnoreBNFNonTerminals);
	return pStringTokenizer;
}

StringTokenizer::StringTokenizer(const char* sData, bool bIgnoreBNFNonTerminals)
{
	init(sData);
	m_bIgnoreBNFNonTerminals = bIgnoreBNFNonTerminals;
}

void StringTokenizer::init(const char* sData)
{
	m_sData = "";
	if (sData != nullptr)
		m_sData = sData;
	m_iDataLength = m_sData.length();

	m_iCurrPos = m_iSavedPos = 0;
	m_iCurrLine = m_iSavedLine = 0;
	m_iCurrColumn = m_iSavedColumn = 0;

	m_bIgnoreBNFNonTerminals = true;

	m_vTokens.clear();
	m_TokensIterator = m_vTokens.end();
}

void StringTokenizer::reset()
{
	init(m_sData.c_str());
}

void StringTokenizer::setData(const char* sData, bool bIgnoreBNFNonTerminals)
{
	init(sData);
	m_bIgnoreBNFNonTerminals = bIgnoreBNFNonTerminals;
}

bool StringTokenizer::hasMoreTokens()
{
	return (m_TokensIterator != m_vTokens.end());
}

Token StringTokenizer::nextToken()
{
	Token tok = *m_TokensIterator;
	m_TokensIterator++;

	return tok;
}

Token StringTokenizer::prevToken()
{
	Token tok(TokenType_::Type::TK_INVALID, "", -1, -1);
	if (m_TokensIterator != m_vTokens.begin())
	{
		int iCount = 1;
		tok = *(m_TokensIterator - iCount++);
		TokenType_::Type eTokenType = tok.m_eTokenType;
		while (	eTokenType == TokenType_::Type::TK_WHITESPACE 
				||
				eTokenType == TokenType_::Type::TK_EOL
				||
				eTokenType == TokenType_::Type::TK_EOI 
				|| 
				eTokenType == TokenType_::Type::TK_COMMA
		) {
			tok = *(m_TokensIterator - iCount++);
			eTokenType = tok.m_eTokenType;
		}
	}

	return tok;
}

void StringTokenizer::ignoreBNFNonTerminals(bool bIgnore)
{
	m_bIgnoreBNFNonTerminals = bIgnore;
}

void StringTokenizer::tokenize()
{
	while (true)
	{
		Token tok = getNextToken();

		//if( NOT (tok.getType() == TokenType::Type::TK_EOL || tok.getType() == TokenType::Type::TK_WHITESPACE))
		//	printf("tok = %s ==> %s\n", tok.getText(), TokenType::toString(tok.getType()).c_str());

		m_vTokens.push_back(tok);

		if (tok.getType() == TokenType_::Type::TK_EOI)
			break;
	};

	m_TokensIterator = m_vTokens.begin();
}

Token StringTokenizer::getNextToken()
{
	initRead();
	while (true)
	{
		char ch = peek(0);
		return readDefault(ch);
	}
}

void StringTokenizer::initRead()
{
	m_iSavedPos = m_iCurrPos;
	m_iSavedLine = m_iCurrLine;
	m_iSavedColumn = m_iCurrColumn;
}

char StringTokenizer::peek(int iOffset)
{
	int iPeekOffset = m_iCurrPos + iOffset;
	if (iPeekOffset > m_iDataLength)
		return END_OF_INPUT;
	else
		return (char)m_sData[iPeekOffset];
}

void StringTokenizer::consume(int iOffset)
{
	m_iCurrPos += iOffset;
	m_iCurrColumn += iOffset;
}

Token StringTokenizer::createToken(TokenType_::Type eTokenType)
{
	std::string sText = m_sData.substr(m_iSavedPos, m_iCurrPos - m_iSavedPos);
	return Token(eTokenType, sText, m_iSavedLine, m_iSavedColumn);
}

Token StringTokenizer::readNumber()
{
	initRead();
	bool bHasDot = false;

	// Read the 1st digit
	consume(1);

	while (true)
	{
		char ch = peek(0);
		if (::isdigit(ch))
			consume(1);
		else
		if (ch == '.' && NOT bHasDot)
		{
			bHasDot = true;
			consume(1);
		}
		else
			break;
	}

	return createToken( bHasDot ? TokenType_::Type::TK_FLOAT : TokenType_::Type::TK_INTEGER);
}

Token StringTokenizer::readEOL()
{
	char ch = peek(0);
	if (ch == LINE_FEED)
		consume(1);

	m_iCurrLine++;
	m_iCurrColumn = 0;

	return createToken(TokenType_::Type::TK_EOL);
}

Token StringTokenizer::readString()
{
	consume(1);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (ch == '"')
			break;
		if (ch == CARRIAGE_RETURN)
		{
			consume(1);
			if (peek(0) == LINE_FEED)
				consume(1);

			m_iCurrLine++;
			m_iCurrColumn = 1;
		}
		else
		if (ch == LINE_FEED)
		{
			consume(1);
			m_iCurrLine++;
			m_iCurrColumn = 1;
		}
		else
			consume(1);
	}

	Token tok = createToken(TokenType_::Type::TK_STRING);
	consume(1);

	return tok;
}

Token StringTokenizer::readCharacter()
{
	consume(1);
	initRead();

	char ch = peek(0);
	consume(1);

	Token tok = createToken(TokenType_::Type::TK_CHARACTER);
	consume(1);

	return tok;
}

Token StringTokenizer::readSingleLineComment()
{
	consume(2);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (ch == CARRIAGE_RETURN || ch == LINE_FEED)
			break;
		else
			consume(1);
	}

	return createToken(TokenType_::Type::TK_SINGLELINECOMMENT);
}

Token StringTokenizer::readMultiLineComment()
{
	consume(2);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (ch == '*' && peek(1) == '/')
		{
			break;
		}
		else
			consume(1);
	}
	
	Token tok = createToken(TokenType_::Type::TK_MULTILINECOMMENT);
	consume(2);

	return tok;
}

Token StringTokenizer::readDefault(char ch0)
{
	char ch1 = peek(1);
	char ch2 = peek(2);

	if(ch0 == END_OF_INPUT)																		
																			{ return createToken(TokenType_::Type::TK_EOI); }
	else if(ch0 == 'w' && ch1 == 'h' && ch2 == 'i' && peek(3) == 'l' && peek(4) == 'e' && peek(5) == '(')
																			{ initRead(); consume(5); return createToken(TokenType_::Type::TK_WHILE); }
	else if(ch0 == 'f' && ch1 == 'o' && ch2 == 'r' && peek(3) == '(')		{ initRead(); consume(3); return createToken(TokenType_::Type::TK_FOR); }
	else if(ch0 == 'i' && ch1 == 'f' && ch2 == '(')							{ initRead(); consume(2); return createToken(TokenType_::Type::TK_IF); }
	else if(ch0 == 'e' && ch1 == 'l' && ch2 == 's' && peek(3) == 'e')		{ initRead(); consume(4); return createToken(TokenType_::Type::TK_ELSE); }
	else if(ch0 == WHITESPACE_SPACE || ch0 == WHITESPACE_TAB)				{ initRead(); consume(1); return createToken(TokenType_::Type::TK_WHITESPACE); }
	else if(ch0 == 'N' && ch1 == 'E' && ch2 == 'G' && peek(3) == 'A' && peek(4) == 'T' && peek(5) == 'E')
																			{ initRead(); consume(6); return createToken(TokenType_::Type::TK_NEGATE); }

	else if(ch0 == CARRIAGE_RETURN && ch1 == LINE_FEED)						{ initRead(); consume(2); return readEOL(); }
	else if(ch0 == LINE_FEED)												{ return readEOL(); }
	else if(isdigit(ch0) || (ch0 == '-' && isdigit(ch1)))					{ return readNumber(); }
	else if(ch0 == '/' && ch1 == '/')										{ return readSingleLineComment(); }
	else if(ch0 == '/' && ch1 == '*')										{ return readMultiLineComment(); }
	else if(isBNFCode(true) && NOT m_bIgnoreBNFNonTerminals)				{ return readBNFCode(); }
	else if(ch0 == '"')														{ return readString(); }
	else if(ch0 == '\'' && ch2 == '\'')										{ return readCharacter(); }

	else if(ch0 == '(')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_LPAREN); }
	else if(ch0 == ')')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_RPAREN); }
	else if(ch0 == '{')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_LBRACE); }
	else if(ch0 == '}')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_RBRACE); }
	else if(ch0 == '[')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_LSQBRACKET); }
	else if(ch0 == ']')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_RSQBRACKET); }

	else if(ch0 == ';')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_SEMICOL);}
	else if(ch0 == ',')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_COMMA); }

	else if(ch0 == '*' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_MULEQ); }
	else if(ch0 == '*')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_MUL); }
	else if(ch0 == '/' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_DIVEQ); }
	else if(ch0 == '/')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_DIV); }
	else if(ch0 == '%')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_MOD); }
	else if(ch0 == '+' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_ADDEQ); }
	else if(ch0 == '-' && ch1 == '-' && (isalpha(ch2) || ch2 == '_'))
																			{ return readPrefixIncrDecr(false); }
	else if(ch0 == '+' && ch1 == '+' && (isalpha(ch2) || ch2 == '_'))
																			{ return readPrefixIncrDecr(true); }
	else if(ch0 == '@')
																			{ return readPointerDeref(); }
	else if(isalpha(ch0) || ch0 == '_' || ch0 == '#')						{ return readIdentifier(); }

	else if(ch0 == '+')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_ADD); }
	else if(ch0 == '-' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_SUBEQ); }
	else if(ch0 == '-')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_SUB); }
	else if(ch0 == '<' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_LTEQ); }

	else if(ch0 == '<' && NOT m_bIgnoreBNFNonTerminals)						{ return readBNFNonTerminal(); }
	
	else if(ch0 == '>' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_GTEQ); }
	else if(ch0 == '=' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_EQ); }
	else if(ch0 == '=')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_ASSIGN); }
	else if(ch0 == '!' && ch1 == '=')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_NEQ); }
	else if(ch0 == '!')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_NOT); }
	else if(ch0 == '&' && ch1 == '&')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_LOGICALAND); }
	else if(ch0 == '&')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_BITWISEAND); }
	else if(ch0 == '|' && ch1 == '|')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_LOGICALOR); }
	else if(ch0 == '|')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_BITWISEOR); }
	else if(ch0 == '^')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_BITWISEXOR); }
	else if(ch0 == '~')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_BITWISENOT); }
	else if(ch0 == '<' && ch1 == '<')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_BITWISELEFTSHIFT); }
	else if(ch0 == '<')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_LT); }
	else if(ch0 == '>' && ch1 == '>')										{ initRead(); consume(2); return createToken(TokenType_::Type::TK_BITWISERIGHTSHIFT); }
	else if(ch0 == '>')														{ initRead(); consume(1); return createToken(TokenType_::Type::TK_GT); }
	
	else if(ch0 == ':' && ch1 == ':' && ch2 == '=' && NOT m_bIgnoreBNFNonTerminals)
																			{ initRead(); consume(3); return createToken(TokenType_::Type::TK_BNFASSIGNMENT); }
	else if(ch0 == '$' && ch1 == '_' && (isalpha(ch2) || ch2 == '_' || ch2 == '#'))
																			{ return readSystemIdentifier(); }
	else																	{ initRead(); consume(1); return createToken(TokenType_::Type::TK_UNKNOWN); }
}

Token StringTokenizer::readIdentifier()
{
	initRead();
	consume(1);

	while (true)
	{
		char ch = peek(0);
		if (isalpha(ch) || ch == '_' || isdigit(ch))
			consume(1);
		else
		if (ch == '(')
		{
			Token tok = createToken(TokenType_::Type::TK_FUNCTIONCALL);
			return tok;
		}
		else
		if (ch == '-' && peek(1) == '>')
		{
			Token tok = createToken(TokenType_::Type::TK_MEMBERACCESS);
			return tok;
		}
		else
		if (ch == '[')
		{
			Token tok = createToken(TokenType_::Type::TK_DEREFARRAY);
			return tok;
		}
		else
		if (ch == '-' && peek(1) == '-')
		{
			Token tok = createToken(TokenType_::Type::TK_POSTFIXDECR);
			consume(2);

			return tok;
		}
		else
		if (ch == '+' && peek(1) == '+')
		{	
			Token tok = createToken(TokenType_::Type::TK_POSTFIXINCR);
			consume(2);

			return tok;
		}
		else
		if (ch == ':' && peek(1) == ':')
		{	
			Token tok = createToken(TokenType_::Type::TK_STATICMEMBERACCESS);
			return tok;
		}
		else
			break;
	}

	return createToken(TokenType_::Type::TK_IDENTIFIER);
}

Token StringTokenizer::readSystemIdentifier()
{
	consume(2);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (isalpha(ch) || ch == '_' || isdigit(ch))
			consume(1);
		else
		if (ch == '(')
		{
			break;
		}
	}

	Token tok = createToken(TokenType_::Type::TK_SYSTEMFUNCTIONCALL);
	return tok;
}

Token StringTokenizer::readPrefixIncrDecr(bool bIsIncr)
{
	consume(2);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (isalpha(ch) || ch == '_' || isdigit(ch))
			consume(1);
		else
			break;
	}

	return createToken(bIsIncr ? TokenType_::Type::TK_PREFIXINCR : TokenType_::Type::TK_PREFIXDECR);
}

Token StringTokenizer::readPointerDeref()
{
	consume(1);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (isalpha(ch) || ch == '_' || isdigit(ch))
			consume(1);
		else
			break;
	}

	return createToken(TokenType_::Type::TK_DEREF);
}

Token StringTokenizer::readBNFNonTerminal()
{
	consume(1);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (ch != '>')
		{
			consume(1);
		}
		else
			break;
	}

	Token tok = createToken(TokenType_::Type::TK_BNFNONTERMINAL);
	consume(1);

	return tok;
}

bool StringTokenizer::isBNFCode(bool bStartTag)
{
	int iPos = 0;
	return (	peek(iPos++) == '"'
				&&
				peek(iPos++) == '<'
				&&
				(NOT bStartTag ? peek(iPos++) == '/' : true)
				&&
				peek(iPos++) == 'c'
				&&
				peek(iPos++) == 'o'
				&&
				peek(iPos++) == 'd'
				&&
				peek(iPos++) == 'e'
				&&
				peek(iPos++) == '>'
				&&
				peek(iPos++) == '"'
			);
}

Token StringTokenizer::readBNFCode()
{
	consume(8);
	initRead();

	while (true)
	{
		char ch = peek(0);
		if (ch == '"' && isBNFCode(false))
		{
			break;
		}
		else
			consume(1);
	}

	Token tok = createToken(TokenType_::Type::TK_BNFCODE);
	consume(9);

	return tok;
}

StringTokenizer::~StringTokenizer()
{

}
