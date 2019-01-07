#pragma once

#include <string>
#include <map>
#include "RandomAccessFile.h"
#include "StringTokenizer.h"
#include "Token.h"

#define OPTIONAL			0
#define MANDATORY			1

#define MAX_BYTECODE_SIZE	8704

class ByteArrayOutputStream;
class ByteArrayInputStream;

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
		static std::vector<std::string>				m_vTypes;
		static std::vector<std::string>				m_vUserDefinedTypes;

		static int									iTabCount;
		static void									printAST(Tree* pNode, bool bPrintTabs = true);
		static void									printTabs();

		static void									generateCode(Tree* pRootNode);
		static void									populateStrings(Tree* pParentNode, std::vector<std::string>& sVector);
		static void									addString(std::string& sText, std::vector<std::string>& vVector);

		static int									getStringPosition(const char* sString);

		static void									printHeaders(RandomAccessFile* pRaf, std::vector<std::string>& vStrings);
		static void									printAssembly(int8_t* iByteCode, std::vector<std::string>& vStrings);

		static Tree*								createNodeOfType(ASTNodeType eASTNodeType, const char* sText = "");
	private:
		static void									handleFunctionDef(Tree* pNode);
		static void									handleFunctionStart(Tree* pNode);
		static void									handleFunctionEnd(Tree* pNode);
		static void									handleFunctionCall(Tree* pNode);

		static void									handleStructDef(Tree* pNode);
		static void									handleStructConstructorOrDestructor(FunctionInfo* pNode);
		static void									addASTForStructMemberVariableConstruction(FunctionInfo* pFunctionInfo);
		static void									addASTForStructMemberVariableDestruction(FunctionInfo* pFunctionInfo);
		static Tree*								createASTForArrayType(Tree* pASTArrayTypeNode);
		static Tree*								createASTForPointerType(Tree* pASTArrayTypeSrc);
		static Tree*								createASTForType(Tree* pASTArrayTypeSrc);
		static Tree*								createFreeASTForArrayType(Tree* pASTArrayTypeSrc);
		static Tree*								createFunctionWithNoArguments(const char* sFunctionName, const char* sReturnType);
		static Tree*								createFunctionCallWithNoArguments(const char* sFunctionName);

		static void									handlePreFixExpression(Tree* pNode);
		static void									handleExpression(Tree* pNode);
		static void									handlePostFixExpression(Tree* pNode);

		static void									handleCharacter(Tree* pNode);
		static void									handleInteger(Tree* pNode);
		static void									handleIdentifier(Tree* pNode);
		static void									handleString(Tree* pNode);
		static void									handlePrimitiveInt(Tree* pNode);
		static void									handleTypeArray(Tree* pNode);
		static void									handlePrimitivePtrEpilogue(Tree* pNode);
		static void									handleAssign(Tree* pNode);
		static void									handleReturnStatement(Tree* pNode);
		static void									handleIfWhile(Tree* pNode);
		static void									handleIfWhile_Prologue(Tree* pNode, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole);
		static void									handleIf_Epilogue(Tree* pNode, int& i_ElseEnd_JMP_Offset, int& i_IfWhile_JCondition_Hole);
		static void									handleWhile_Epilogue(Tree* pNode, int& i_While_Loop_Hole, int& i_IfWhile_JCondition_Hole);
		static void									handleSwitch(Tree* pNode);
		static void									handleSwitchArgument(Tree* pNode);
		static void									handleSwitchCasePrologue(Tree* pNode, std::vector<uint32_t>& vCaseStartOffsets);
		static void									handleSwitchCases(Tree* pNode, std::vector<uint32_t>& vCaseStartOffsets, std::vector<uint32_t>& vCaseBreakJmpHoles);
		static void									handleSwitchCaseEpilogues(Tree* pNode, std::vector<uint32_t>& vCaseBreakJmpHoles);
		static void									handleMalloc(Tree* pNode);
		static void									handleFree(Tree* pNode);
		static void									handleStatics(Tree* pNode);
		static void									handleTypeStructs(Tree* pNode);

		static void									handleStatements(Tree* pNode);

		static void									populateCode(Tree* pNode);

		static int32_t								sizeOf(std::string sType);
		static int32_t								castValueFor(std::string sType);
		static void									cast(int32_t iCastValue);
		static void									storeValueAtPosForVariable(int32_t iPos, const char* sType, const char* sVariableName);

		static void									emit(OPCODE eOPCODE, int iOperand);
		static void									emit(OPCODE eOPCODE, int iOperand1, int iOperand2);

		static void									emitByte(int8_t iCode);
		static void									emitInt(int32_t iCode);

		static void									emitIntAtPos(int32_t iCode, uint32_t iOffset);

		static FunctionInfo*						getFunctionInfo(Tree* pNode);
		static StructInfo*							getStructOfObject(std::string sObjectName);

		static std::vector<std::string>				m_vStrings;

		static std::map<std::string, FunctionInfo*>	m_MapGlobalFunctions;
		static std::map<std::string, StructInfo*>	m_MapGlobalStructs;

		static int8_t								m_iByteCode[MAX_BYTECODE_SIZE];

		static ByteArrayOutputStream*				m_pBAOS;
		static ByteArrayInputStream*				m_pBAIS;
	public:
		static FunctionInfo*						m_pCurrentFunction;
		static StructInfo*							m_pCurrentStruct;
};