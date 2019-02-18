#pragma once

#include <stack>
#include <vector>
#include "GrammerUtils.h"
#include <assert.h>

class TinyCReader {
	public:
		TinyCReader();

		void addKeywords() {
			GrammerUtils::m_vKeywords.push_back("struct");
			GrammerUtils::m_vKeywords.push_back("inline");
			GrammerUtils::m_vKeywords.push_back("virtual");
			GrammerUtils::m_vKeywords.push_back("static");
			GrammerUtils::m_vKeywords.push_back("return");
			GrammerUtils::m_vKeywords.push_back("if");
			GrammerUtils::m_vKeywords.push_back("else");
			GrammerUtils::m_vKeywords.push_back("while");
			GrammerUtils::m_vKeywords.push_back("switch");
			GrammerUtils::m_vKeywords.push_back("case");
			GrammerUtils::m_vKeywords.push_back("break");
			GrammerUtils::m_vKeywords.push_back("default");
			GrammerUtils::m_vKeywords.push_back("break");
			GrammerUtils::m_vKeywords.push_back("for");
			GrammerUtils::m_vKeywords.push_back("print");
			GrammerUtils::m_vKeywords.push_back("putc");
			GrammerUtils::m_vKeywords.push_back("memSet");
			GrammerUtils::m_vKeywords.push_back("memCpy");
			GrammerUtils::m_vKeywords.push_back("memCmp");
			GrammerUtils::m_vKeywords.push_back("memChr");
			GrammerUtils::m_vKeywords.push_back("sizeOf");
			GrammerUtils::m_vKeywords.push_back("new");
			GrammerUtils::m_vKeywords.push_back("malloc");
			GrammerUtils::m_vKeywords.push_back("free");
			GrammerUtils::m_vKeywords.push_back("&&");
			GrammerUtils::m_vKeywords.push_back("||");
			GrammerUtils::m_vKeywords.push_back("==");
			GrammerUtils::m_vKeywords.push_back("!=");
			GrammerUtils::m_vKeywords.push_back("<=");
			GrammerUtils::m_vKeywords.push_back(">=");
			GrammerUtils::m_vKeywords.push_back("<<");
			GrammerUtils::m_vKeywords.push_back(">>");
		}

		bool def();
		bool objectList();
		bool structDeclaration();
		bool structObjectList();
		bool structInlineFunction();
		bool staticDeclaration();
		bool primitiveType();
		bool structType();
		bool staticPtr();
		bool functionDef();
		bool functionArgumentDefList();
		bool functionArgumentDefListMore();
		bool primitiveTypeInt();
		bool stmt_list();
		bool stmt();
		bool prePostFixedIncrDecr();
		bool preFixIncrDecr();
		bool postFixIncrDecr();
		bool returnStatement();
		bool functionCall();
		bool functionArgumentList();
		bool functionArgumentItem();
		bool ifelseStatement();
		bool ifStatement();
		bool elseStatement();
		bool whileStatement();
		bool switchStatement();
		bool switchArgument();
		bool oneOrMoreCasesOrDefault();
		bool oneOrMoreSwitchCases();
		bool switchCase();
		bool defaultCase();
		bool forStatement();
		bool oneOrMoreInitExprs();
		bool initExpr();
		bool oneOrMoreLoopExprs();
		bool loopExpr();
		bool print();
		bool print_list();
		bool print_list_0();
		bool putc();
		bool memSet();
		bool memCpy();
		bool memCmp();
		bool memChr();
		bool sizeOf();
		bool putcList();
		bool bracesstmtlist();
		bool newStructPtr();
		bool userDefinedConstructorCall();
		bool newPtrOrArrayOrInt();
		bool primPtrOrArrayOrInt();
		bool primPtr();
		bool primPtrOptionalRHS();
		bool primArrayOrInt();
		bool primArray();
		bool primArrayOptionalRHS();
		bool primType();
		bool primTypeOptionalRHS();
		bool ptrAssign();
		bool malloc();
		bool assignmentDerefArray();
		bool structMemberVariableAssignmentOrFunctionCall();
		bool structMemberVariableLValueOrFunctionCall();
		bool structMemberVariableLValueArrayAccess();
		bool assignmentRHS();
		bool freePtrStatement();
		bool expr();
		bool startExpr();
		bool logicalAndOrExpr();
		bool logicalAndOr();
		bool equalityExpr();
		bool eqNeqExpr();
		bool eqNeq();
		bool relationalExpr();
		bool lteqGteqExpr();
		bool lteqGteq();
		bool addSubExpr();
		bool plusMinusExpr();
		bool plusMinus();
		bool mulDivExpr();
		bool mulDivModExpr();
		bool mulDivMod();
		bool bitwiseExpr();
		bool bitwiseOrAndXorExpr();
		bool bitwiseOrAndXor();
		bool primary();
		bool operands();
		bool tk_identifier();
		bool structMemberAccess();
		bool structMemberVariableOrFunctionCall_RValue();
		bool structMemberVariableArrayInAnExpr();
		bool structMemberFunctionCallInAnExpr();
		bool unary_expr();
		bool unary_oper();
		bool preFixInExpr();
		bool rValueDeref();
		bool postFixIncrDecrInExpr();
		bool paren_expr();

		
		void 						read(const char* sFile);
		std::string					popOperator();
		ENUM_OP_PRECEDENCE 			opFromString(std::string sOperator);
		void						checkOpPrecedenceAndPush(std::string sCurrentOperator);
		
		static Tree*				makeLeaf(ASTNodeType eASTNodeType, const char* sText);
		Tree*						makeNode(ASTNodeType eASTNodeType, Tree* pLeft, Tree* pRight);
		Tree*						createPostFixExpr(Tree* pLeaf = nullptr);
		
		void						updateBlockString(std::string sIdent);
		void						removeLastFromBlockString();
		std::string					getBlockString();
		std::string					getFullyQualifiedNameForVariable(Tree* pNode, std::string sVariable);

		void 						addType(std::string sType);
		bool						isValidType(std::string sType);
		void 						addStructType(std::string sType);
		bool 						isValidStructType(std::string sType);
		Tree* 						hasNodeOfType(Tree* pNode, ASTNodeType eASTNodeType);
		
		void						pushLocalHeapVar(std::string sVariableName);
		void						startBlockMarker();
		void						endBlockMarker();
		
		E_VARIABLESCOPE 			getCurrentScope();
		std::string 				getCurrentScopeString();
		
		Tree*						getStructNodeByName(std::string sStructName);
		void						handleFunctionCallInExpr();
	protected:
		
	private:
		std::stack<std::string>		m_vOperatorStack;
		std::vector<std::string>	m_vPostFix;
		
		std::stack<std::string>		m_vLocalHeapVarStack;
		
		Tree*						m_pASTRootNode;
		Tree*						m_pASTCurrentNode;
		
		std::vector<std::string>	m_sBlockStringList;
		bool						m_bStructInProcess;
		bool						m_bFunctionInProcess;
		
		std::vector<Tree*>			m_vStructs;

};
