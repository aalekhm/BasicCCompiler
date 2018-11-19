#include "TinyCReader.h"
#include "GrammerUtils.h"

TinyCReader::TinyCReader()
{
	GrammerUtils::init();
}

void TinyCReader::read(const char* sFile)
{
	//////////////// THIS PIECE OF CODE WILL REMAIN COMMON FOR ALL //////////////////
	if (GrammerUtils::read(sFile))
	{
		addKeywords();
		GrammerUtils::getNextToken();

		m_pASTRootNode = new Tree();
		{
			m_pASTRootNode->m_eASTNodeType = ASTNodeType::ASTNode_ROOT;
			m_pASTCurrentNode = m_pASTRootNode;
		}

		def();

		GrammerUtils::generateCode(m_pASTRootNode);
	}
	/////////////////////////////////////////////////////////////////////////////////
}

std::string TinyCReader::popOperator()
{
	if(!m_vOperatorStack.empty())
	{
		std::string sTop = m_vOperatorStack.top();
		m_vOperatorStack.pop();
		
		return sTop;
	}
	
	return "";
}

ENUM_OP_PRECEDENCE TinyCReader::opFromString(std::string sOperator)
{
	ENUM_OP_PRECEDENCE eOperator = ENUM_OP_PRECEDENCE::OP_INVALID;

	if (sOperator == "||")
		eOperator = ENUM_OP_PRECEDENCE::OP_LOGICALOR;
	else
	if (sOperator == "&&")
		eOperator = ENUM_OP_PRECEDENCE::OP_LOGICALAND;
	else
	if (sOperator == "==")
		eOperator = ENUM_OP_PRECEDENCE::OP_EQ;
	else
	if (sOperator == "!=")
		eOperator = ENUM_OP_PRECEDENCE::OP_NEQ;
	else
	if (sOperator == "<")
		eOperator = ENUM_OP_PRECEDENCE::OP_LT;
	else
	if (sOperator == "<=")
		eOperator = ENUM_OP_PRECEDENCE::OP_LTEQ;
	else
	if (sOperator == ">")
		eOperator = ENUM_OP_PRECEDENCE::OP_GT;
	else
	if (sOperator == ">=")
		eOperator = ENUM_OP_PRECEDENCE::OP_GTEQ;
	else
	if (sOperator == "+")
		eOperator = ENUM_OP_PRECEDENCE::OP_ADD;
	else
	if (sOperator == "-")
		eOperator = ENUM_OP_PRECEDENCE::OP_SUB;
	else
	if (sOperator == "*")
		eOperator = ENUM_OP_PRECEDENCE::OP_MUL;
	else
	if (sOperator == "/")
		eOperator = ENUM_OP_PRECEDENCE::OP_DIV;
	else
	if (sOperator == "")
		eOperator = ENUM_OP_PRECEDENCE::OP_MOD;
	else
	if (sOperator == "(")
		eOperator = ENUM_OP_PRECEDENCE::OP_LPAREN;
	else
	if (sOperator == ")")
		eOperator = ENUM_OP_PRECEDENCE::OP_RPAREN;
	else
	if (sOperator == "!")
		eOperator = ENUM_OP_PRECEDENCE::OP_NOT;
	else
	if (sOperator == "%")
		eOperator = ENUM_OP_PRECEDENCE::OP_MOD;

	return eOperator;
}

void TinyCReader::checkOpPrecedenceAndPush(std::string sCurrentOperator)
{
	if(m_vOperatorStack.empty()) m_vOperatorStack.push(sCurrentOperator);
	else
	{
		int eCurrOp = (int)opFromString(sCurrentOperator);
		
		std::string sTopOfStack = m_vOperatorStack.top();
		int eTopOfStack = (int)opFromString(sTopOfStack);
		
		if(eCurrOp > eTopOfStack)
			m_vOperatorStack.push(sCurrentOperator);
		else
		{
			popOperator();
			m_vPostFix.push_back(sTopOfStack);
			m_vOperatorStack.push(sCurrentOperator);
		}
	}
}

Tree* TinyCReader::createPostFixExpr()
{
	if (!m_vOperatorStack.empty())
	{
		while (!m_vOperatorStack.empty())
		{
			std::string sTop = m_vOperatorStack.top();
			m_vPostFix.push_back(sTop);

			m_vOperatorStack.pop();
		}
	}

	std::string sPostFixExpr = "";
	
	int iSize = m_vPostFix.size();
	for(int i = 1; i <= iSize; i++)
	{
		sPostFixExpr += m_vPostFix[i-1];
		if(i != iSize)
			sPostFixExpr += ", ";
	}

	m_vPostFix.clear();

	Tree* pLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, sPostFixExpr.c_str());
	return pLeaf;
}

Tree* TinyCReader::makeLeaf(ASTNodeType eASTNodeType, const char* sText)
{
	Tree* pLeaf = new Tree();
	{
		pLeaf->m_eASTNodeType = eASTNodeType;
		pLeaf->m_sText = sText;
	}

	return pLeaf;
}

Tree* TinyCReader::makeNode(ASTNodeType eASTNodeType, Tree* pLeft, Tree* pRight)
{
	Tree* pNode = new Tree();
	{
		pNode->m_eASTNodeType = eASTNodeType;
		pNode->m_pLeftNode = pLeft;
		pNode->m_pRightNode = pRight;
	}

	return pNode;
}


bool TinyCReader::def() {
if(!stmt_list())
return false;
return true;

}

bool TinyCReader::stmt_list() {
while(true) {
if(stmt()) {
}
else
break;
}

return true;

}

bool TinyCReader::stmt() {
if(assignment()) {
return true;
}
else
if(ifelse()) {
return true;
}
else
if(_while()) {
return true;
}
else
if(print()) {
return true;
}
else
if(putc()) {
return true;
}
else
if(bracesstmtlist()) {
return true;
}
else
return false;

return true;

}

bool TinyCReader::ifelse() {
if(!_if())
return false;
return true;

}

bool TinyCReader::_if() {
if(!GrammerUtils::match("if", MANDATORY))
return false;

															Tree* pIfNode = makeLeaf(ASTNodeType::ASTNode_IF, GrammerUtils::m_pPrevToken.getText());
															Tree* pTemp = nullptr;
															{
																pIfNode->m_pParentNode = m_pASTCurrentNode;
		
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pIfNode;
															}
														
if(!GrammerUtils::match('(', MANDATORY))
return false;
if(!expr())
return false;

															Tree* pExpressionLeftLeaf = createPostFixExpr();
															{
																m_pASTCurrentNode->m_pLeftNode = pExpressionLeftLeaf;
															}
														
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match('{', OPTIONAL)) {

}

else {

}

if(!stmt_list())
return false;
if(!GrammerUtils::match('}', OPTIONAL)) {

}

else {

}

if(!_else()) {
}
else {
}


															pTemp->addChild(pIfNode);
															{
																m_pASTCurrentNode = pTemp;
															}
														
return true;

}

bool TinyCReader::_else() {
if(!GrammerUtils::match("else", MANDATORY))
return false;

															Tree* pElseNode = makeLeaf(ASTNodeType::ASTNode_ELSE, GrammerUtils::m_pPrevToken.getText());
															Tree* pIfNode = nullptr;
															{
																pIfNode = m_pASTCurrentNode;
																m_pASTCurrentNode = pElseNode;
															}
														
if(!GrammerUtils::match('{', OPTIONAL)) {

}

else {

}

if(!stmt_list())
return false;
if(!GrammerUtils::match('}', OPTIONAL)) {

}

else {

}


															pIfNode->m_pRightNode = pElseNode;
															{
																m_pASTCurrentNode = pIfNode;
															}
														
return true;

}

bool TinyCReader::_while() {
if(!GrammerUtils::match("while", MANDATORY))
return false;

															Tree* pWhileNode = makeLeaf(ASTNodeType::ASTNode_WHILE, GrammerUtils::m_pPrevToken.getText());
															Tree* pTemp = nullptr;
															{
																pWhileNode->m_pParentNode = m_pASTCurrentNode;

																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pWhileNode;
															}
														
if(!GrammerUtils::match('(', MANDATORY))
return false;
if(!expr())
return false;

															Tree* pExpressionLeftLeaf = createPostFixExpr();
															{
																m_pASTCurrentNode->m_pLeftNode = pExpressionLeftLeaf;
															}
														
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match('{', OPTIONAL)) {

}

else {

}

if(!stmt_list())
return false;
if(!GrammerUtils::match('}', OPTIONAL)) {

}

else {

}


															pTemp->addChild(pWhileNode);
															{
																m_pASTCurrentNode = pTemp;
															}
														
return true;

}

bool TinyCReader::print() {
if(!GrammerUtils::match("print", MANDATORY))
return false;
if(!GrammerUtils::match('(', MANDATORY))
return false;

															Tree* pPrintNode = makeLeaf(ASTNodeType::ASTNode_PRINT, GrammerUtils::m_pPrevToken.getText());
															Tree* pTemp = nullptr;
															{
																pPrintNode->m_pParentNode = m_pASTCurrentNode;
														
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pPrintNode;
															}
														
if(!print_list())
return false;
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match(';', MANDATORY))
return false;

															pTemp->addChild(pPrintNode);
															{
																m_pASTCurrentNode = pTemp;
															}
														
return true;

}

bool TinyCReader::print_list() {
while(true) {
if(print_list_0()) {
}
else
break;
}

return true;

}

bool TinyCReader::print_list_0() {
if(GrammerUtils::match(TokenType::Type::TK_STRING, OPTIONAL)) {

															Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, GrammerUtils::m_pPrevToken.getText());
															{
																m_pASTCurrentNode->addChild(pStringNode);
															}
														
if(!GrammerUtils::match(',', OPTIONAL)) {

}

else {

}

return true;
}
else
if(expr()) {

															Tree* pExpressionLeftLeaf = createPostFixExpr();
															{
																m_pASTCurrentNode->addChild(pExpressionLeftLeaf);
															}
														
if(!GrammerUtils::match(',', OPTIONAL)) {

}

else {

}

return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

																Tree* pCharacterNode = makeLeaf(ASTNodeType::ASTNode_CHARACTER, GrammerUtils::m_pPrevToken.getText());
																{
																	m_pASTCurrentNode->addChild(pCharacterNode);
																}
															
return true;
}
else
return false;

return true;

}

bool TinyCReader::putc() {
if(!GrammerUtils::match("putc", MANDATORY))
return false;

																Tree* pPutCNode = makeLeaf(ASTNodeType::ASTNode_PUTC, GrammerUtils::m_pPrevToken.getText());
																Tree* pTemp = nullptr;
																{
																	pPutCNode->m_pParentNode = m_pASTCurrentNode;
															
																	pTemp = m_pASTCurrentNode;
																	m_pASTCurrentNode = pPutCNode;
																}
															
if(!GrammerUtils::match('(', MANDATORY))
return false;
if(!putcList())
return false;
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match(';', MANDATORY))
return false;

																pTemp->addChild(pPutCNode);
																{
																	m_pASTCurrentNode = pTemp;
																}
															
return true;

}

bool TinyCReader::putcList() {
if(GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

																Tree* pIdentifierNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, GrammerUtils::m_pPrevToken.getText());
																m_pASTCurrentNode->addChild(pIdentifierNode);
															
return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

																Tree* pIntegerNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, GrammerUtils::m_pPrevToken.getText());
																m_pASTCurrentNode->addChild(pIntegerNode);
															
return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

																Tree* pCharacterNode = makeLeaf(ASTNodeType::ASTNode_CHARACTER, GrammerUtils::m_pPrevToken.getText());
																m_pASTCurrentNode->addChild(pCharacterNode);
															
return true;
}
else
return false;

return true;

}

bool TinyCReader::bracesstmtlist() {
if(!GrammerUtils::match('{', MANDATORY))
return false;
if(!stmt_list())
return false;
if(!GrammerUtils::match('}', MANDATORY))
return false;
return true;

}

bool TinyCReader::assignment() {
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;
 
															Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, "");
															Tree* pTemp = nullptr;
															{
																pAssignmentNode->m_pParentNode = m_pASTCurrentNode;

																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pAssignmentNode;
															}
															Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, GrammerUtils::m_pPrevToken.getText());
															{
																pAssignmentNode->m_pRightNode = pIdentifierLeaf;
															}
														
if(!GrammerUtils::match('=', MANDATORY))
return false;
if(!expr())
return false;

															Tree* pExpressionLeftLeaf = createPostFixExpr();
															{
																m_pASTCurrentNode->m_pLeftNode = pExpressionLeftLeaf;
															}
														
if(!GrammerUtils::match(';', MANDATORY))
return false;

															pTemp->addChild(pAssignmentNode);
															{
																m_pASTCurrentNode = pTemp;
															}
														
return true;

}

bool TinyCReader::paren_expr() {
if(!unary_oper()) {
}
else {
}

if(!GrammerUtils::match('(', MANDATORY))
return false;
if(!expr())
return false;
if(!GrammerUtils::match(')', MANDATORY))
return false;
return true;

}

bool TinyCReader::expr() {
if(!and_expr())
return false;
while(true) {
if(logicalor_expr()) {
}
else
break;
}

return true;

}

bool TinyCReader::logicalor_expr() {
if(!GrammerUtils::match("||", MANDATORY))
return false;
 
														checkOpPrecedenceAndPush("||");
													
if(!and_expr())
return false;
return true;

}

bool TinyCReader::and_expr() {
if(!equality_expr())
return false;
while(true) {
if(logicaland_expr()) {
}
else
break;
}

return true;

}

bool TinyCReader::logicaland_expr() {
if(!GrammerUtils::match("&&", MANDATORY))
return false;
 
														checkOpPrecedenceAndPush("&&");
													
if(!equality_expr())
return false;
return true;

}

bool TinyCReader::equality_expr() {
if(!relational_expr())
return false;
if(!equality_expr_optional()) {
}
else {
}

return true;

}

bool TinyCReader::equality_expr_optional() {
if(!equality())
return false;
if(!relational_expr())
return false;
return true;

}

bool TinyCReader::equality() {
if(GrammerUtils::match("==", OPTIONAL)) {
 
														checkOpPrecedenceAndPush("==");
													
return true;
}
else
if(GrammerUtils::match("!=", OPTIONAL)) {
 
														checkOpPrecedenceAndPush("!=");
													
return true;
}
else
return false;

return true;

}

bool TinyCReader::relational_expr() {
if(!addition_expr())
return false;
if(!relational_expr_optional()) {
}
else {
}

return true;

}

bool TinyCReader::relational_expr_optional() {
if(!relational())
return false;
if(!addition_expr())
return false;
return true;

}

bool TinyCReader::relational() {
if(GrammerUtils::match('<', OPTIONAL)) {
 
														checkOpPrecedenceAndPush("<");
													
return true;
}
else
if(GrammerUtils::match("<=", OPTIONAL)) {
 
														checkOpPrecedenceAndPush("<=");
													
return true;
}
else
if(GrammerUtils::match('>', OPTIONAL)) {
 
														checkOpPrecedenceAndPush(">");
													
return true;
}
else
if(GrammerUtils::match(">=", OPTIONAL)) {
 
														checkOpPrecedenceAndPush(">=");
													
return true;
}
else
return false;

return true;

}

bool TinyCReader::addition_expr() {
if(!multiplication_expr())
return false;
while(true) {
if(addition_expr_optional()) {
}
else
break;
}

return true;

}

bool TinyCReader::addition_expr_optional() {
if(!add_sub())
return false;
if(!multiplication_expr())
return false;
return true;

}

bool TinyCReader::add_sub() {
if(GrammerUtils::match('+', OPTIONAL)) {
 
														checkOpPrecedenceAndPush("+");
													
return true;
}
else
if(GrammerUtils::match('-', OPTIONAL)) {
 
														checkOpPrecedenceAndPush("-");
													
return true;
}
else
return false;

return true;

}

bool TinyCReader::multiplication_expr() {
if(!primary())
return false;
while(true) {
if(multiplication_expr_optional()) {
}
else
break;
}

return true;

}

bool TinyCReader::multiplication_expr_optional() {
if(!mul_div_mod())
return false;
if(!primary())
return false;
return true;

}

bool TinyCReader::mul_div_mod() {
if(GrammerUtils::match('*', OPTIONAL)) {
 
														checkOpPrecedenceAndPush("*");
													
return true;
}
else
if(GrammerUtils::match('/', OPTIONAL)) {
 
														checkOpPrecedenceAndPush("/");
													
return true;
}
else
if(GrammerUtils::match('%', OPTIONAL)) {
 
														checkOpPrecedenceAndPush("%");
													
return true;
}
else
return false;

return true;

}

bool TinyCReader::primary() {
if(!defaults())
return false;
return true;

}

bool TinyCReader::defaults() {
if(operands()) {
return true;
}
else
if(paren_expr()) {
return true;
}
else
return false;

return true;

}

bool TinyCReader::operands() {

											std::string sOperand;
										
if(GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

																sOperand = GrammerUtils::m_pPrevToken.getText();
																m_vPostFix.push_back(sOperand);
															
return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

																sOperand = GrammerUtils::m_pPrevToken.getText();
																m_vPostFix.push_back(sOperand);
															
return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

																sOperand = GrammerUtils::m_pPrevToken.getText();
																char pStr[255] = {0};
																sprintf_s(pStr, "%d", sOperand.c_str()[0]);

																m_vPostFix.push_back(pStr);
															
return true;
}
else
return false;

return true;

}

bool TinyCReader::unary_oper() {
if(GrammerUtils::match('+', OPTIONAL)) {
return true;
}
else
if(GrammerUtils::match('-', OPTIONAL)) {
return true;
}
else
if(GrammerUtils::match('!', OPTIONAL)) {
 
																checkOpPrecedenceAndPush("!");
															
return true;
}
else
return false;

return true;

}
