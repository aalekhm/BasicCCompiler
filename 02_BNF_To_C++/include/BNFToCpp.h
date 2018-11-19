#pragma once
#include <vector>
#include "StringTokenizer.h"
#include "RandomAccessFile.h"
#include "NonTerminal.h"

enum class EGrammerState
{
	WRITING_FUNCTION_START,
	WRITING_FUNCTION
};


class BNFToCpp
{
	public:
									BNFToCpp();
		virtual						~BNFToCpp();
		
		void						process(const char* sGrammerFile, const char* sClassName);
		void						nonTerminalize();
	protected:
		bool						tokenize(const char* sGrammerFile);
		void						generateDotH();
		void						generateDotCPP();
		void						onTokenCallback(RandomAccessFile* rafOut, const char* sClassName, Token tok);
	private:
		void						writeFromFile(RandomAccessFile* rafOutDest, const char* sSrcFileName);

		std::vector<Token>			m_vTokens;
		std::vector<NonTerminal*>	m_vNonterminals;
		std::vector<Token>			m_vKeywords;

		std::string					m_sClassName;

		// BNF variables
		bool						m_bWhile, m_bORed, m_bORedFirst, m_bOptional;
		EGrammerState				m_eGrammerState;
};