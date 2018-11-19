#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Token.h"

#define NOT !
#define END_OF_INPUT 0
#define WHITESPACE_SPACE	' '
#define WHITESPACE_TAB		'\t'
#define CARRIAGE_RETURN		'\r'
#define LINE_FEED			'\n'

class StringTokenizer
{
	public:
		static StringTokenizer*		create(const char* sData, bool bIgnoreBNFNonTerminals = true);
		void						setData(const char* sData, bool bIgnoreBNFNonTerminals = true);

		void						ignoreBNFNonTerminals(bool bIgnore = true);	// Make it false to include non-terminals ==> <Non-Terminal>.

		void						tokenize();
		bool						hasMoreTokens();
		Token						nextToken();
		Token						prevToken();
protected:
	private:
									StringTokenizer() = delete;
									StringTokenizer(const char* sData, bool bIgnoreBNFNonTerminals);
									virtual ~StringTokenizer();

		void						init(const char* sData);
		void						reset();
		Token						getNextToken();
		void						initRead();
		char						peek(int iOffset);
		void						consume(int iOffset);
		Token						createToken(TokenType::Type eTokenType);
		Token						readNumber();
		Token						readEOL();
		Token						readString();
		Token						readCharacter();
		Token						readDefault(char ch0);
		Token						readIdentifier();
		Token						readBNFNonTerminal();
		Token						readBNFCode();
		Token						readSingleLineComment();
		Token						readMultiLineComment();

		bool						isBNFCode(bool bStartTag);

		int							m_iCurrPos;
		int							m_iSavedPos;

		int							m_iCurrLine;
		int							m_iSavedLine;

		int							m_iCurrColumn;
		int							m_iSavedColumn;

		std::string					m_sData;
		int							m_iDataLength;

		bool						m_bIgnoreBNFNonTerminals;	// <Non-Terminal>

		std::vector<Token>			m_vTokens;
		std::vector<Token>::const_iterator	m_TokensIterator;

};
