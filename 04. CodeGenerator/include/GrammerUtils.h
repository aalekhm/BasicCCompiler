#pragma once

#include <string>
#include "RandomAccessFile.h"
#include "StringTokenizer.h"
#include "Token.h"

#define OPTIONAL	0
#define MANDATORY	1

enum class OPCODE
{
	NOP = 0,
	FETCH,
	STORE,
	PUSH,
	POP,
	MUL,
	DIV,
	MOD,
	ADD,
	SUB,
	JMP_LT,
	JMP_LTEQ,
	JMP_GT,
	JMP_GTEQ,
	JMP_EQ,
	JMP_NEQ,
	LOGICALOR,
	LOGICALAND,
	_NOT,
	JMP,
	JZ,
	JNZ,
	PRTS,
	PRTC,
	PRTI,
	HLT,
};

#define MAX_BYTECODE_SIZE	1024

class GrammerUtils
{
	public:
		static Token					m_pToken;
		static Token					getNextToken();
		static Token					m_pPrevToken;

		static bool						match(TokenType::Type eTokenType, int TYPE);
		static bool						match(std::string sText, int TYPE);
		static bool						match(char ch, int TYPE);

		static void						init();
		static bool						read(const char* sFile);

		static bool						isOneOfTheKeywords(std::string sKeyword);

		static StringTokenizer*			m_pStrTok;
		static std::vector<std::string>	m_vKeywords;

		static int						iTabCount;
		static void						printAST(Tree* pNode);
		static void						printTabs();

		static void						generateCode(Tree* pRootNode);
		static void						populateVariables(Tree* pParentNode, std::vector<std::string>& sVector);
		static void						populateStrings(Tree* pParentNode, std::vector<std::string>& sVector);
		static void						addString(std::string& sText, std::vector<std::string>& vVector);
		static void						populateCode(Tree* pNode, int* pByteCode, int& iOffset);
		static void						emit(int iCode, int* pByteCode, int iOffset);

		static int						getVariablePosition(const char* sIdentifier);
		static int						getStringPosition(const char* sString);

		static void						printHeaders(RandomAccessFile* pRaf, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings);
		static void						printAssembly(int* iByteCode, int iOffset, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings);

		static std::vector<std::string>	m_vVariables;
		static std::vector<std::string>	m_vStrings;

		static int						m_iByteCode[MAX_BYTECODE_SIZE];
		static int						m_iJumpHole;
;	private:
};