#pragma once

#include <stack>
#include <vector>
#include "GrammerUtils.h"

class TinyCReader {
	public:
		TinyCReader();

		void addKeywords() {
			GrammerUtils::m_vKeywords.push_back("if");
			GrammerUtils::m_vKeywords.push_back("else");
			GrammerUtils::m_vKeywords.push_back("while");
			GrammerUtils::m_vKeywords.push_back("print");
			GrammerUtils::m_vKeywords.push_back("putc");
			GrammerUtils::m_vKeywords.push_back("||");
			GrammerUtils::m_vKeywords.push_back("&&");
			GrammerUtils::m_vKeywords.push_back("==");
			GrammerUtils::m_vKeywords.push_back("!=");
			GrammerUtils::m_vKeywords.push_back("<=");
			GrammerUtils::m_vKeywords.push_back(">=");
		}

		bool def();
		bool stmt_list();
		bool stmt();
		bool ifelse();
		bool _if();
		bool _else();
		bool _while();
		bool print();
		bool print_list();
		bool print_list_0();
		bool putc();
		bool putcList();
		bool bracesstmtlist();
		bool assignment();
		bool paren_expr();
		bool expr();
		bool logicalor_expr();
		bool and_expr();
		bool logicaland_expr();
		bool equality_expr();
		bool equality_expr_optional();
		bool equality();
		bool relational_expr();
		bool relational_expr_optional();
		bool relational();
		bool addition_expr();
		bool addition_expr_optional();
		bool add_sub();
		bool multiplication_expr();
		bool multiplication_expr_optional();
		bool mul_div_mod();
		bool primary();
		bool defaults();
		bool operands();
		bool unary_oper();

		
		void 						read(const char* sFile);
		std::string					popOperator();
		ENUM_OP_PRECEDENCE 			opFromString(std::string sOperator);
		void						checkOpPrecedenceAndPush(std::string sCurrentOperator);		
		
		Tree*						makeLeaf(ASTNodeType eASTNodeType, const char* sText);
		Tree*						makeNode(ASTNodeType eASTNodeType, Tree* pLeft, Tree* pRight);
		Tree*						createPostFixExpr();
	protected:
		
	private:
		std::stack<std::string>		m_vOperatorStack;
		std::vector<std::string>	m_vPostFix;
		
		Tree*						m_pASTRootNode;
		Tree*						m_pASTCurrentNode;

};