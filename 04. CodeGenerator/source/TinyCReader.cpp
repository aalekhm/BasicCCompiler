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
	if (sOperator == "(")
		eOperator = ENUM_OP_PRECEDENCE::OP_LPAREN;
	else
	if (sOperator == ")")
		eOperator = ENUM_OP_PRECEDENCE::OP_RPAREN;
	else
	if (sOperator == "%")
		eOperator = ENUM_OP_PRECEDENCE::OP_MOD;
	else
	if (sOperator == "!")
		eOperator = ENUM_OP_PRECEDENCE::OP_NOT;
	else
	if (sOperator == "NEGATE")
		eOperator = ENUM_OP_PRECEDENCE::OP_NEGATE;

	return eOperator;
}

void TinyCReader::checkOpPrecedenceAndPush(std::string sCurrentOperator)
{
	if(m_vOperatorStack.empty()) m_vOperatorStack.push(sCurrentOperator);
	else
	{
		if (sCurrentOperator == ")")
		{
			while (NOT m_vOperatorStack.empty())
			{
				std::string sOp = m_vOperatorStack.top();
				popOperator();

				if (sOp != "(")
				{	
					m_vPostFix.push_back(sOp);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			if (sCurrentOperator == "(")
			{
				m_vOperatorStack.push(sCurrentOperator);
			}
			else
			{
				int eCurrOp = (int)opFromString(sCurrentOperator);

				std::string sTopOfStack = m_vOperatorStack.top();
				int eTopOfStack = (int)opFromString(sTopOfStack);

				if (eCurrOp > eTopOfStack)
					m_vOperatorStack.push(sCurrentOperator);
				else
				{
					popOperator();
					m_vPostFix.push_back(sTopOfStack);
					m_vOperatorStack.push(sCurrentOperator);
				}
			}
		}
	}
}

Tree* TinyCReader::createPostFixExpr(Tree* pLeaf /* = nullptr*/)
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

	if (pLeaf == nullptr)
		pLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, sPostFixExpr.c_str());
	else
		pLeaf->m_sText = sPostFixExpr;

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
															
if(!GrammerUtils::match(TokenType::Type::TK_FUNCTIONCALL, MANDATORY))
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
if(!GrammerUtils::match(';', OPTIONAL)) {

}

else {

}

}
else
break;
}

return true;

}

bool TinyCReader::stmt() {
if(functionCall()) {
return true;
}
else
if(assignmentRHS()) {
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
if(returnStatement()) {
return true;
}
else
return false;

return true;

}

bool TinyCReader::returnStatement() {
if(!GrammerUtils::match("return", MANDATORY))
return false;

														updateBlockString("return");
														
														Tree* pReturnStmtNode = makeLeaf(ASTNodeType::ASTNode_RETURNSTMT, "return");
														{
															pReturnStmtNode->m_pParentNode = m_pASTCurrentNode;
														}
														
														Tree* pTemp = nullptr;
														Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
														{
															pExpressionLeftLeaf->m_pParentNode = pReturnStmtNode;
															
															pTemp = m_pASTCurrentNode;
															m_pASTCurrentNode = pExpressionLeftLeaf;
														}
													
if(!expr())
return false;

														removeLastFromBlockString();
														
														m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
														pReturnStmtNode->addChild(pExpressionLeftLeaf);
														
														m_pASTCurrentNode = pTemp;
														m_pASTCurrentNode->addChild(pReturnStmtNode);
													
return true;

}

bool TinyCReader::functionCall() {
if(!GrammerUtils::match(TokenType::Type::TK_FUNCTIONCALL, MANDATORY))
return false;

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
if(!GrammerUtils::match('(', MANDATORY))
return false;

															updateBlockString("if");

															Tree* pIfNode = makeLeaf(ASTNodeType::ASTNode_IF, "if");
															{
																pIfNode->m_pParentNode = m_pASTCurrentNode;
															}

															Tree* pTemp = nullptr;
															Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
															{
																pIfNode->m_pLeftNode = pExpressionLeftLeaf;
																pExpressionLeftLeaf->m_pParentNode = pIfNode;
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pExpressionLeftLeaf;
															}
														
if(!expr())
return false;

															m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
															m_pASTCurrentNode = pIfNode;
														
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


															removeLastFromBlockString();
														
if(!_else()) {
}
else {
}


															m_pASTCurrentNode = pTemp;
															m_pASTCurrentNode->addChild(pIfNode);
														
return true;

}

bool TinyCReader::_else() {
if(!GrammerUtils::match("else", MANDATORY))
return false;

															updateBlockString("else");
															
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
if(!GrammerUtils::match('(', MANDATORY))
return false;

															updateBlockString("while");
															
															Tree* pWhileNode = makeLeaf(ASTNodeType::ASTNode_WHILE, "while");
															{
																pWhileNode->m_pParentNode = m_pASTCurrentNode;
															}
															
															Tree* pTemp = nullptr;
															Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
															{
																pWhileNode->m_pLeftNode = pExpressionLeftLeaf;
																pExpressionLeftLeaf->m_pParentNode = pWhileNode;
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pExpressionLeftLeaf;
															}
														
if(!expr())
return false;

															m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
															m_pASTCurrentNode = pWhileNode;
														
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


															removeLastFromBlockString();
															
															m_pASTCurrentNode = pTemp;
															m_pASTCurrentNode->addChild(pWhileNode);
														
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

																m_pASTCurrentNode = pTemp;
															
return true;

}

bool TinyCReader::putcList() {
if(GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

																std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
																assert(!sFullyQualifiedVariableName.empty());

																Tree* pIdentifierNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
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
																Tree* pTemp = nullptr;
																{
																	pPrimIntNode->m_sAdditionalInfo.append(sVariableName);
																	pPrimIntNode->m_pParentNode = m_pASTCurrentNode;
																}
																
																Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
																{
																	pPrimIntNode->m_pLeftNode = pExpressionLeftLeaf;
																	pExpressionLeftLeaf->m_pParentNode = pPrimIntNode;
																	
																	pTemp = m_pASTCurrentNode;
																	m_pASTCurrentNode = pExpressionLeftLeaf;
																}

															
if(!expr())
return false;

																m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
															
																m_pASTCurrentNode = pTemp;
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
															

																m_pASTCurrentNode->addChild(pPrimStringNode);
															
return true;

}

bool TinyCReader::assignmentRHS() {
if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

											std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
										
if(!GrammerUtils::match('=', MANDATORY))
return false;

															std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
															assert(!sFullyQualifiedVariableName.empty());

															Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sFullyQualifiedVariableName.c_str());
															Tree* pTemp = nullptr;
															{
																pAssignmentNode->m_pParentNode = m_pASTCurrentNode;
															}
															
															Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
															{
																pIdentifierLeaf->m_sAdditionalInfo = sVariableName;
																pAssignmentNode->m_pRightNode = pIdentifierLeaf;
															}
															
															Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
															{
																pAssignmentNode->m_pLeftNode = pExpressionLeftLeaf;
																pExpressionLeftLeaf->m_pParentNode = pAssignmentNode;
																
																pTemp = m_pASTCurrentNode;
																m_pASTCurrentNode = pExpressionLeftLeaf;
															}
														
if(!expr())
return false;

															m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
															
															m_pASTCurrentNode = pTemp;
															m_pASTCurrentNode->addChild(pAssignmentNode);
														
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
if(unary_expr()) {
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
										
if(functionCall()) {

																// The idea here is to create a temporary variable of the type returned by the function
																// & add it before the expression statement.
																// The temporary variable is then inserted in the expression.
																// Eg:
																// 		int iRet = 10;
																//		iRet = 10 + retFunc(); // where retFunc return type is "int".
																//		This will create a dummy code as follows:
																//			int iRet = 10;
																//			int iRet_retFunc = retFunc;	// This line of code will be inserted by the following piece of code.
																//			iRet = 10 + iRet_retFunc;
																
																Tree* pExpressionNode = m_pASTCurrentNode;
																Tree* pAssignNode = pExpressionNode->m_pParentNode;
																Tree* pBlockNode = pAssignNode->m_pParentNode;
																Tree* pFunctionCallNode = pExpressionNode->getLastStatement();
																{
																	pFunctionCallNode->removeFromParent();
																}
																assert(pFunctionCallNode != nullptr);
																{
																	std::string sFuncName = pFunctionCallNode->m_sText;
																	std::string sFullyQualifiedTempVariableName;
																	sFullyQualifiedTempVariableName.append(pAssignNode->m_sText);
																	sFullyQualifiedTempVariableName.append("_");
																	sFullyQualifiedTempVariableName.append(sFuncName);
																	
																	Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_PRIMITIVETYPEINT, sFullyQualifiedTempVariableName.c_str());
																	{
																		pPrimIntNode->m_sAdditionalInfo.append(sFullyQualifiedTempVariableName);
																		pBlockNode->addChild(pPrimIntNode);
																	}
																	
																	Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
																	{
																		pPrimIntNode->m_pLeftNode = pExpressionLeftLeaf;
																		pExpressionLeftLeaf->m_pParentNode = pPrimIntNode;
																		pExpressionLeftLeaf->addChild(pFunctionCallNode);
																	}
																	
																	m_vPostFix.push_back(sFullyQualifiedTempVariableName);
																}
															
return true;
}
else
if(tk_identifier()) {
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

bool TinyCReader::tk_identifier() {
if(!preFixOper()) {
}
else {
}

if(!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
return false;

																std::string sOperand = GrammerUtils::m_pPrevToken.getText();
																std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sOperand);
																assert(!sFullyQualifiedVariableName.empty());
																if(!sFullyQualifiedVariableName.empty())
																{
																	sOperand = sFullyQualifiedVariableName;
																}
																
																m_vPostFix.push_back(sOperand);
															
if(!postFixOper()) {
}
else {
}

return true;

}

bool TinyCReader::unary_expr() {
if(!unary_oper())
return false;
if(!expr())
return false;
return true;

}

bool TinyCReader::unary_oper() {
if(GrammerUtils::match('-', OPTIONAL)) {
 
																checkOpPrecedenceAndPush("NEGATE");
															
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

bool TinyCReader::preFixOper() {
if(GrammerUtils::match("--", OPTIONAL)) {
 
															if(m_pASTCurrentNode->m_pLeftNode == nullptr)
															{
																Tree* pPreFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_PREFIX, "");
																m_pASTCurrentNode->m_pLeftNode = pPreFixNode;
															}
															
															std::string sVariableName = GrammerUtils::m_pToken.m_sText;
															std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
															Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREDECR, sFullyQualifiedVariableName.c_str());
															{
																m_pASTCurrentNode->m_pLeftNode->addChild(pPreDecrNode);
															}
														
return true;
}
else
if(GrammerUtils::match("++", OPTIONAL)) {
 
															if(m_pASTCurrentNode->m_pLeftNode == nullptr)
															{
																Tree* pPreFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_PREFIX, "");
																m_pASTCurrentNode->m_pLeftNode = pPreFixNode;
															}

															std::string sVariableName = GrammerUtils::m_pToken.m_sText;
															std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
															Tree* pPreIncrNode = makeLeaf(ASTNodeType::ASTNode_PREINCR, sFullyQualifiedVariableName.c_str());
															{
																m_pASTCurrentNode->m_pLeftNode->addChild(pPreIncrNode);
															}
														
return true;
}
else
return false;

return true;

}

bool TinyCReader::postFixOper() {
 
										std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
									
if(GrammerUtils::match("--", OPTIONAL)) {
 
															if(m_pASTCurrentNode->m_pRightNode == nullptr)
															{
																Tree* pPostFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_POSTFIX, "");
																m_pASTCurrentNode->m_pRightNode = pPostFixNode;
															}

															std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
															Tree* pPostDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTDECR, sFullyQualifiedVariableName.c_str());
															{
																m_pASTCurrentNode->m_pRightNode->addChild(pPostDecrNode);
															}
														
return true;
}
else
if(GrammerUtils::match("++", OPTIONAL)) {
 
															if(m_pASTCurrentNode->m_pRightNode == nullptr)
															{
																Tree* pPostFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_POSTFIX, "");
																m_pASTCurrentNode->m_pRightNode = pPostFixNode;
															}

															std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
															Tree* pPostIncrNode = makeLeaf(ASTNodeType::ASTNode_POSTINCR, sFullyQualifiedVariableName.c_str());
															{
																m_pASTCurrentNode->m_pRightNode->addChild(pPostIncrNode);
															}
														
return true;
}
else
return false;

return true;

}

bool TinyCReader::paren_expr() {
if(!GrammerUtils::match('(', MANDATORY))
return false;

															checkOpPrecedenceAndPush("(");
														
if(!expr())
return false;
if(!GrammerUtils::match(')', MANDATORY))
return false;

															checkOpPrecedenceAndPush(")");
														
return true;

}

