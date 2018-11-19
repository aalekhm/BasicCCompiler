#pragma once

#include <string>
#include <map>
#include "RandomAccessFile.h"
#include "StringTokenizer.h"
#include "Token.h"

#define OPTIONAL			0
#define MANDATORY			1

#define MAX_BYTECODE_SIZE	2048

class GrammerUtils
{
	public:
		static Token								m_pToken;
		static Token								getNextToken();
		static Token								m_pPrevToken;

		static bool									match(TokenType::Type eTokenType, int TYPE);
		static bool									match(std::string sText, int TYPE);
		static bool									match(char ch, int TYPE);

		static void									init();
		static bool									read(const char* sFile);

		static bool									isOneOfTheKeywords(std::string sKeyword);

		static StringTokenizer*						m_pStrTok;
		static std::vector<std::string>				m_vKeywords;

		static int									iTabCount;
		static void									printAST(Tree* pNode);
		static void									printTabs();

		static void									generateCode(Tree* pRootNode);
		static void									populateStrings(Tree* pParentNode, std::vector<std::string>& sVector);
		static void									addString(std::string& sText, std::vector<std::string>& vVector);

		static int									getVariablePosition(const char* sIdentifier);
		static int									getStringPosition(const char* sString);

		static void									printHeaders(RandomAccessFile* pRaf, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings);
		static void									printAssembly(int* iByteCode, int iOffset, std::vector<std::string>& vVariables, std::vector<std::string>& vStrings);

		static std::vector<std::string>				m_vVariables;
		static std::vector<std::string>				m_vStrings;

		static std::map<std::string, FunctionInfo*>	m_MapFunctionInfos;
		static FunctionInfo*						m_pCurrentFunction;
		
		static int									m_iByteCode[MAX_BYTECODE_SIZE];
		static int									m_iJumpHole;
	private:
		static void									handleFunctionDef(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleFunctionStart(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleFunctionEnd(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleFunctionCall(Tree* pNode, int* pByteCode, int& iOffset);

		static void									handlePreFixExpression(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleExpression(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handlePostFixExpression(Tree* pNode, int* pByteCode, int& iOffset);

		static void									handleCharacter(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleInteger(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleIdentifier(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleString(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handlePrimitiveInt(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleAssign(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleReturnStatement(Tree* pNode, int* pByteCode, int& iOffset);
		static void									handleIfWhile_Prologue(Tree* pNode, int* pByteCode, int& iOffset, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole);
		static void									handleIf_Epilogue(Tree* pNode, int* pByteCode, int& iOffset, int& i_ElseEnd_JMP_Offset, int& i_IfWhile_JCondition_Hole);
		static void									handleWhile_Epilogue(Tree* pNode, int* pByteCode, int& iOffset, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole);

		static void									handleStatements(Tree* pNode, int* pByteCode, int& iOffset);

		static void									populateCode(Tree* pNode, int* pByteCode, int& iOffset);
		static void									emit(int iCode, int* pByteCode, int iOffset);

		static void									emit(OPCODE eOPCODE, int iOperand, int* pByteCode, int& iOffset);
		static void									emit(OPCODE eOPCODE, int iOperand1, int iOperand2, int* pByteCode, int& iOffset);
};