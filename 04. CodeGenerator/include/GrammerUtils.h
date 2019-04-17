#pragma once

#include <windows.h>
#include <string>
#include <map>
#include "RandomAccessFile.h"
#include "StringTokenizer.h"
#include "Token.h"

#define OPTIONAL_			0
#define MANDATORY_			1

#define MAX_BYTECODE_SIZE	14 * 1024

class ByteArrayOutputStream;
class ByteArrayInputStream;

class GrammerUtils
{
	public:
		static Token								m_pToken;
		static Token								getNextToken();
		static Token								m_pPrevToken;
		static Token								m_pSavedToken;

		static bool									match(TokenType_::Type eTokenType, int TYPE);
		static bool									match(std::string sText, int TYPE);
		static bool									match(char ch, int TYPE);

		static void									init();
		static bool									read(const char* sFile);

		static bool									isOneOfTheKeywords(std::string sKeyword);

		static StringTokenizer*						m_pStrTok;
		static std::vector<std::string>				m_vKeywords;
		static std::vector<std::string>				m_vTypes;
		static std::vector<std::string>				m_vUserDefinedTypes;
		static std::vector<std::string>				m_vUserDefinedInterfaces;
		static std::vector<std::string>				m_vUserDefinedFunctions;
		static std::vector<std::string>				m_vUserDefinedVariables;

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
		static void									handleSystemFunctionCall(Tree* pNode);

		static void									handleInterfaceDef(Tree* pNode);
		static void									handleInterfaceEnd(Tree* pNode);
		static void									handleStructDef(Tree* pNode);
		static void									handleStructConstructorOrDestructor(FunctionInfo* pNode);
		static void									addASTForStructMemberVariableConstruction(FunctionInfo* pFunctionInfo);
		static void									addASTForStructMemberVariableDestruction(FunctionInfo* pFunctionInfo);
		static void									handleStructEnd(Tree* pNode);
		static void									handleStructMemberAccess(Tree* pNode);
		static void									handleStructMemberAccessDeref(Tree* pNode);

		static Tree*								createASTForArrayType(Tree* pASTArrayTypeNode);
		static Tree*								createASTForPointerType(Tree* pASTArrayTypeSrc);
		static Tree*								createASTForType(Tree* pASTArrayTypeSrc);
		static Tree*								createFreeASTForArrayType(Tree* pASTArrayTypeSrc);
		static Tree*								createFunctionWithNoArguments(const char* sFunctionName, const char* sReturnType);
		static Tree*								createFunctionCallWithNoArguments(const char* sFunctionName);
		static Tree*								createThisFunctionCallWithoutArguments(const char* sFunctionName);
		static Tree*								createPrintType(const char* sString);

		static void									handlePreFixExpression(Tree* pNode);
		static void									handleExpression(Tree* pNode);
		static void									handlePostFixExpression(Tree* pNode);

		static void									handleCharacter(Tree* pNode);
		static void									handleInteger(Tree* pNode);
		static void									handleIdentifier(Tree* pNode);
		static void									handleString(Tree* pNode);
		static void									handlePrimitiveInt(Tree* pNode);
		static void									handleTypeArray(Tree* pNode);
		static void									allocateTypeArray(Tree* pNode);
		static void									initializeTypeArray(Tree* pNode);
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
		static void									handleMemSet(Tree* pNode);
		static void									handleMemCpy(Tree* pNode);
		static void									handleMemCmp(Tree* pNode);
		static void									handleMemChr(Tree* pNode);
		static void									handleSizeOf(Tree* pNode);
		static void									handleFree(Tree* pNode);
		static void									handleStatics(Tree* pNode);
		static void									handleTypeStructs(Tree* pNode);

		static void									handleStatements(Tree* pNode);

		static void									populateCode(Tree* pNode);

		static int32_t								sizeOf(std::string sType);
		static int32_t								castValueFor(std::string sType);
		static void									cast(std::string sType);
		static void									cast(int32_t iCastValue);
		static PRIMIIVETYPE							getTypeByString(std::string sType);
		static void									storeValueAtPosForVariable(int32_t iPos, const char* sType, Tree* pNode);

		static void									emit(OPCODE eOPCODE, int iOperand);
		static void									emitF(OPCODE eOPCODE, float fOperand);
		static void									emit(OPCODE eOPCODE, int iOperand1, int iOperand2);

		static void									emitByte(int8_t iCode);
		static void									emitInt(int32_t iCode);
		static void									emitFloat(float fCode);

		static void									emitIntAtPos(int32_t iCode, uint32_t iOffset);

		static FunctionInfo*						getFunctionInfo(Tree* pNode);
		static StructInfo*							getStructByName(std::string sObjectName);
		static void									createStructVTable(StructInfo* pCurrentStruct);
		static void									addVirtualFunctionsFromParentStruct(StructInfo* pCurrentStruct, std::vector<void*>& vVirtualFunctions);
		static void									emitStructVTable(StructInfo* pCurrentStruct);
		static void									storeVTablePointerInObjectOnHeap(int16_t iShortPosition, StructInfo* pStructInfo);

		static int32_t								getMemberPositionInStructHierarchy(std::string sMemberVariableName, StructInfo* pStructInfo);
		static std::string							getMemberTypeInStructHierarchy(std::string sMemberVariableName, StructInfo* pStructInfo);
		static FunctionInfo*						getFunctionByNameInStruct(std::string sFunctionName, StructInfo* pStructInfo);

		static bool									isStructObedient(StructInfo* pStructInfo);

		static std::vector<std::string>				m_vStrings;

		static int8_t								m_iByteCode[MAX_BYTECODE_SIZE];

		static ByteArrayOutputStream*				m_pBAOS;
		static ByteArrayInputStream*				m_pBAIS;

		static void*								m_HColor;
	public:
		static std::map<std::string, FunctionInfo*>	m_MapGlobalFunctions;
		static std::map<std::string, StructInfo*>	m_MapGlobalStructs;
		static std::map<std::string, InterfaceInfo*>m_MapGlobalInterfaces;

		static bool									isABuiltInType(const char* cStr);
		static bool									isAKeyword(const char* cStr);
		static bool									isAUserDefinedType(const char* cStr);
		static bool									isAUserDefinedInterface(const char* cStr);
		static bool									isAUserDefinedFunction(const char* cStr);
		static bool									isAUserDefinedVariable(const char* cStr);

		static FunctionInfo*						getGlobalFunctionByName(std::string sFuncName);
		static Tree*								getSystemFunctionByName(std::string sSystemFuncName);
		static std::string							getSystemFunctionReturnType(std::string sSystemFuncName);

		static FunctionInfo*						m_pCurrentFunction;
		static StructInfo*							m_pCurrentStruct;
		static InterfaceInfo*						m_pCurrentInterface;

		static std::map<std::string, Tree*>			m_MapSystemFunctions;
};