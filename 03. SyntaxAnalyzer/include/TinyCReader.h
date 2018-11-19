#pragma once

#include <stack>
#include <vector>
#include "GrammerUtils.h"
#include <assert.h>

class TinyCReader {
	public:
		TinyCReader();

		void addKeywords() {
			GrammerUtils::m_vKeywords.push_back("static");
			GrammerUtils::m_vKeywords.push_back("void");
			GrammerUtils::m_vKeywords.push_back("int8_t");
			GrammerUtils::m_vKeywords.push_back("int16_t");
			GrammerUtils::m_vKeywords.push_back("int32_t");
			GrammerUtils::m_vKeywords.push_back("int32_t");
			GrammerUtils::m_vKeywords.push_back("void");
			GrammerUtils::m_vKeywords.push_back("int8_t");
			GrammerUtils::m_vKeywords.push_back("int16_t");
			GrammerUtils::m_vKeywords.push_back("int32_t");
			GrammerUtils::m_vKeywords.push_back("string");
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
			GrammerUtils::m_vKeywords.push_back("string");
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
		bool functionList();
		bool staticDeclarations();
		bool primPointerTypes();
		bool staticVoidPtr();
		bool functionDef();
		bool returnType();
		bool functionArgumentDefList();
		bool functionArgumentDefListMore();
		bool primitiveType();
		bool primitiveTypeInt();
		bool primitiveTypeString();
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
		bool putcList();
		bool bracesstmtlist();
		bool assignmentNewVariable();
		bool newPtrOrInt();
		bool primPtrOrInt();
		bool primPtr();
		bool primInt();
		bool newString();
		bool ptrAssign();
		bool malloc();
		bool assignmentDerefArray();
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
		
		Tree*						makeLeaf(ASTNodeType eASTNodeType, const char* sText);
		Tree*						makeNode(ASTNodeType eASTNodeType, Tree* pLeft, Tree* pRight);
		Tree*						createPostFixExpr(Tree* pLeaf = nullptr);
		
		void						updateBlockString(std::string sIdent);
		void						removeLastFromBlockString();
		std::string					getBlockString();
		std::string					getFullyQualifiedNameForVariable(Tree* pNode, std::string sVariable);
	protected:
		
	private:
		std::stack<std::string>		m_vOperatorStack;
		std::vector<std::string>	m_vPostFix;
		
		Tree*						m_pASTRootNode;
		Tree*						m_pASTCurrentNode;
		
		std::vector<std::string>	m_sBlockStringList;

};
