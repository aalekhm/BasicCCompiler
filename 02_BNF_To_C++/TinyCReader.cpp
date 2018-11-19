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

void TinyCReader::updateBlockString(std::string sIdent)
{
	if (NOT sIdent.empty())
	{
		m_sBlockStringList.push_back(sIdent);
	}
}

std::string TinyCReader::getBlockString()
{
	std::string sReturn;
	for (std::string sStr : m_sBlockStringList)
	{
		sReturn.append(sStr);
		sReturn.append("_");
	}

	return sReturn;
}

void TinyCReader::removeLastFromBlockString()
{
	if (m_sBlockStringList.size() > 0)
	{
		m_sBlockStringList.erase(m_sBlockStringList.end()-1);
	}
}

std::string	TinyCReader::getFullyQualifiedNameForVariable(Tree* pNode, std::string sVariable)
{
	std::string sFullyQualifiedName;

	for (Tree* pChild : pNode->m_vStatements)
	{
		if (	pChild->m_eASTNodeType == ASTNodeType::ASTNode_PRIMITIVETYPEINT
				||
				pChild->m_eASTNodeType == ASTNodeType::ASTNode_PRIMITIVETYPESTRING
		) {
			if (pChild->m_sAdditionalInfo == sVariable)
			{
				sFullyQualifiedName = pChild->m_sText;
				break;
			}
		}
	}

	if (sFullyQualifiedName.empty())
	{
		if (pNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONDEF)
		{
			sFullyQualifiedName = getFullyQualifiedNameForVariable(pNode->m_pRightNode, sVariable); // Search in the 'Arg list'.
		}
		else
		{
			sFullyQualifiedName = getFullyQualifiedNameForVariable(pNode->m_pParentNode, sVariable);
		}
	}

	return sFullyQualifiedName;
}


bool TinyCReader::def() {
if(!functionList())
return false;
return true;

}

bool TinyCReader::functionList() {
while(true) {
if(functionDef()) {
}
else
break;
}

return true;

}

bool TinyCReader::functionDef() {
if(!returnType())
return false;

																std::string sReturnType = GrammerUtils::m_pPrevToken.getText();
															
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

																std::string sFunctionName = GrammerUtils::m_pPrevToken.getText();
																updateBlockString(sFunctionName);
																
																Tree* pFunctionDefNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONDEF, sFunctionName.c_str());
																Tree* pReturnTypeNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONRETURNTYPE, sReturnType.c_str());
																Tree* pArgListNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONARGLIST, "");
																{
																	pReturnTypeNode->m_pParentNode = pFunctionDefNode;
																	pArgListNode->m_pParentNode = pFunctionDefNode;
																	
																	m_pASTCurrentNode->addChild(pFunctionDefNode);
																	
																	pFunctionDefNode->m_pLeftNode = pReturnTypeNode;
																	pFunctionDefNode->m_pRightNode = pArgListNode;
																}
															
if(!GrammerUtils::match('(', MANDATORY))
return false;

																Tree* pTemp = nullptr;
																{
																	pTemp = m_pASTCurrentNode; // Save Root Node temporarily
																	m_pASTCurrentNode = pArgListNode;
																}
															
if(!functionArgumentDefList()) {
}
else {
}


																			m_pASTCurrentNode = pTemp;
																		
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match('{', MANDATORY))
return false;

																pTemp = m_pASTCurrentNode; // Save Root Node temporarily
																m_pASTCurrentNode = pFunctionDefNode;
																
																Tree* pFuncStartNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONSTART, "");
																m_pASTCurrentNode->addChild(pFuncStartNode);
															
if(!stmt_list())
return false;
if(!GrammerUtils::match('}', MANDATORY))
return false;

																removeLastFromBlockString();
																
																Tree* pFuncEndNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONEND, "");
																m_pASTCurrentNode->addChild(pFuncEndNode);

																m_pASTCurrentNode = pTemp;
															
return true;

}

bool TinyCReader::returnType() {
if(GrammerUtils::match("int", OPTIONAL)) {
return true;
}
else
if(GrammerUtils::match("void", OPTIONAL)) {
return true;
}
else
if(GrammerUtils::match("string", OPTIONAL)) {
return true;
}
else
return false;

return true;

}

bool TinyCReader::functionArgumentDefList() {
while(true) {
if(functionArgumentDefListMore()) {
}
else
break;
}

return true;

}

bool TinyCReader::functionArgumentDefListMore() {
if(primitiveTypeInt()) {
if(!GrammerUtils::match(',', OPTIONAL)) {

}

else {

}

return true;
}
else
if(primitiveTypeString()) {
if(!GrammerUtils::match(',', OPTIONAL)) {

}

else {

}

return true;
}
else
return false;

return true;

}

bool TinyCReader::primitiveTypeInt() {
if(!GrammerUtils::match("int", MANDATORY))
return false;
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

																std::string sArgName = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedArgName;
																sFullyQualifiedArgName.append(getBlockString());
																sFullyQualifiedArgName.append(sArgName);

																Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_PRIMITIVETYPEINT, sFullyQualifiedArgName.c_str());
																{
																	pPrimIntNode->m_sAdditionalInfo.append(sArgName);
																	
																	m_pASTCurrentNode->addChild(pPrimIntNode);
																	m_pASTCurrentNode->m_sAdditionalInfo.append("I");
																}
															
return true;

}

bool TinyCReader::primitiveTypeString() {
if(!GrammerUtils::match("string", MANDATORY))
return false;
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

																std::string sArgName = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedArgName;
																sFullyQualifiedArgName.append(getBlockString());
																sFullyQualifiedArgName.append(sArgName);
																
																Tree* pPrimStringNode = makeLeaf(ASTNodeType::ASTNode_PRIMITIVETYPESTRING, sFullyQualifiedArgName.c_str());
																{
																	pPrimStringNode->m_sAdditionalInfo.append(sArgName);
																	
																	m_pASTCurrentNode->addChild(pPrimStringNode);
																	m_pASTCurrentNode->m_sAdditionalInfo.append("S");
																}
															
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
if(assignOrFuncCall()) {
return true;
}
else
if(assignmentNewVariable()) {
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

bool TinyCReader::assignOrFuncCall() {
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;
if(assignmentRHS()) {
return true;
}
else
if(functionCall()) {
return true;
}
else
return false;

return true;

}

bool TinyCReader::functionCall() {

											std::string sIdentifier = GrammerUtils::m_pPrevToken.getText();
										
if(!GrammerUtils::match('(', MANDATORY))
return false;

															Tree* pFunctionCallNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONCALL, sIdentifier.c_str());
															Tree* pTemp = nullptr;
															{
																m_pASTCurrentNode->addChild(pFunctionCallNode);
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pFunctionCallNode;
															}
														
if(!functionArgumentList()) {
}
else {
}

if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match(';', MANDATORY))
return false;

															Tree* pFuncCallEndNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONCALLEND, sIdentifier.c_str());
															m_pASTCurrentNode->addChild(pFuncCallEndNode);
															
															m_pASTCurrentNode = pTemp;
														
return true;

}

bool TinyCReader::functionArgumentList() {
while(true) {
if(functionArgumentItem()) {
}
else
break;
}

return true;

}

bool TinyCReader::functionArgumentItem() {
if(GrammerUtils::match(TokenType::Type::TK_STRING, OPTIONAL)) {

															Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, GrammerUtils::m_pPrevToken.getText());
															{
																m_pASTCurrentNode->addChild(pStringNode);
																m_pASTCurrentNode->m_sAdditionalInfo.append("S");
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
																pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
																
																m_pASTCurrentNode->m_sAdditionalInfo.append("I");
															}
														
if(!GrammerUtils::match(',', OPTIONAL)) {

}

else {

}

return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

																Tree* pIntegerNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, GrammerUtils::m_pPrevToken.getText());
																{
																	m_pASTCurrentNode->addChild(pIntegerNode);
																	m_pASTCurrentNode->m_sAdditionalInfo.append("C");
																}
															
return true;
}
else
if(GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

																Tree* pCharacterNode = makeLeaf(ASTNodeType::ASTNode_CHARACTER, GrammerUtils::m_pPrevToken.getText());
																{
																	m_pASTCurrentNode->addChild(pCharacterNode);
																	m_pASTCurrentNode->m_sAdditionalInfo.append("I");
																}
															
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
																m_pASTCurrentNode->addChild(pIfNode);

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
																pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
															}
														
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match('{', OPTIONAL)) {

}

else {

}


															updateBlockString("if");
														
if(!stmt_list())
return false;
if(!GrammerUtils::match('}', OPTIONAL)) {

}

else {

}


															removeLastFromBlockString();
														
if(!_else()) {
}
else {
}


															m_pASTCurrentNode = pTemp;
														
return true;

}

bool TinyCReader::_else() {
if(!GrammerUtils::match("else", MANDATORY))
return false;

															Tree* pElseNode = makeLeaf(ASTNodeType::ASTNode_ELSE, GrammerUtils::m_pPrevToken.getText());
															Tree* pIfNode = nullptr;
															{	
																pIfNode = m_pASTCurrentNode;
																pElseNode->m_pParentNode = pIfNode->m_pParentNode;
																
																m_pASTCurrentNode = pElseNode;
															}
														
if(!GrammerUtils::match('{', OPTIONAL)) {

}

else {

}


															updateBlockString("else");
														
if(!stmt_list())
return false;
if(!GrammerUtils::match('}', OPTIONAL)) {

}

else {

}


															removeLastFromBlockString();
															
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
																m_pASTCurrentNode->addChild(pWhileNode);
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pWhileNode;
															}
														
if(!GrammerUtils::match('(', MANDATORY))
return false;
if(!expr())
return false;

															Tree* pExpressionLeftLeaf = createPostFixExpr();
															{
																pWhileNode->m_pLeftNode = pExpressionLeftLeaf;
																pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
															}
														
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match('{', OPTIONAL)) {

}

else {

}


															updateBlockString("while");
														
if(!stmt_list())
return false;
if(!GrammerUtils::match('}', OPTIONAL)) {

}

else {

}


															removeLastFromBlockString();
															m_pASTCurrentNode = pTemp;
														
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
																m_pASTCurrentNode->addChild(pPrintNode);
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pPrintNode;
															}
														
if(!print_list())
return false;
if(!GrammerUtils::match(')', MANDATORY))
return false;
if(!GrammerUtils::match(';', MANDATORY))
return false;

															m_pASTCurrentNode = pTemp;
														
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
																pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
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
																	m_pASTCurrentNode->addChild(pPutCNode);

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

																m_pASTCurrentNode = pTemp;
															
return true;

}

bool TinyCReader::putcList() {
if(GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

																std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
																assert(!sFullyQualifiedVariableName.empty());

																Tree* pIdentifierNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName);
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

																updateBlockString("{");
															
if(!stmt_list())
return false;
if(!GrammerUtils::match('}', MANDATORY))
return false;

																removeLastFromBlockString();
															
return true;

}

bool TinyCReader::assignmentNewVariable() {
if(newInt()) {
return true;
}
else
if(newString()) {
return true;
}
else
return false;

return true;

}

bool TinyCReader::newInt() {
if(!GrammerUtils::match("int", MANDATORY))
return false;
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

																std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedVariableName;
																sFullyQualifiedVariableName.append(getBlockString());
																sFullyQualifiedVariableName.append(sVariableName);
															
if(!GrammerUtils::match('=', MANDATORY))
return false;
 
																Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_PRIMITIVETYPEINT, sFullyQualifiedVariableName.c_str());
																{
																	pPrimIntNode->m_sAdditionalInfo.append(sVariableName);
																}
															
if(!expr())
return false;

																Tree* pExpressionLeftLeaf = createPostFixExpr();
																{
																	pPrimIntNode->m_pLeftNode = pExpressionLeftLeaf;
																	pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
																}
															
if(!GrammerUtils::match(';', MANDATORY))
return false;

																m_pASTCurrentNode->addChild(pPrimIntNode);
															
return true;

}

bool TinyCReader::newString() {
if(!GrammerUtils::match("string", MANDATORY))
return false;
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

																std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedVariableName;
																sFullyQualifiedVariableName.append(getBlockString());
																sFullyQualifiedVariableName.append(sVariableName);
															
if(!GrammerUtils::match('=', MANDATORY))
return false;
 
																Tree* pPrimStringNode = makeLeaf(ASTNodeType::ASTNode_PRIMITIVETYPESTRING, sFullyQualifiedVariableName.c_str());
																{
																	pPrimStringNode->m_sAdditionalInfo.append(sVariableName);
																}
															
if(!expr())
return false;

																Tree* pExpressionLeftLeaf = createPostFixExpr();
																{
																	pPrimStringNode->m_pLeftNode = pExpressionLeftLeaf;
																	pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
																}
															
if(!GrammerUtils::match(';', MANDATORY))
return false;

																m_pASTCurrentNode->addChild(pPrimStringNode);
															
return true;

}

bool TinyCReader::assignmentRHS() {

											std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
										
if(!GrammerUtils::match('=', MANDATORY))
return false;
 
															std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
															assert(!sFullyQualifiedVariableName.empty());

															Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, "");
															Tree* pTemp = nullptr;
															{
																m_pASTCurrentNode->addChild(pAssignmentNode);
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pAssignmentNode;
															}
															
															Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
															{
																pIdentifierLeaf->m_sAdditionalInfo = sVariableName;
																pAssignmentNode->m_pRightNode = pIdentifierLeaf;
															}
														
if(!expr())
return false;

															Tree* pExpressionLeftLeaf = createPostFixExpr();
															{
																m_pASTCurrentNode->m_pLeftNode = pExpressionLeftLeaf;
																pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
															}
														
if(!GrammerUtils::match(';', MANDATORY))
return false;

															m_pASTCurrentNode = pTemp;
														
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
																std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sOperand);
																assert(!sFullyQualifiedVariableName.empty());
																if(!sFullyQualifiedVariableName.empty())
																{
																	sOperand = sFullyQualifiedVariableName;
																}
																
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

