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
	if (!m_vOperatorStack.empty())
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
			if (sOperator == "|")
				eOperator = ENUM_OP_PRECEDENCE::OP_BITWISEOR;
			else
				if (sOperator == "&")
					eOperator = ENUM_OP_PRECEDENCE::OP_BITWISEAND;
				else
					if (sOperator == "^")
						eOperator = ENUM_OP_PRECEDENCE::OP_BITWISEXOR;
					else
						if (sOperator == "~")
							eOperator = ENUM_OP_PRECEDENCE::OP_BITWISENOT;
						else
							if (sOperator == "<<")
								eOperator = ENUM_OP_PRECEDENCE::OP_BITWISELEFTSHIFT;
							else
								if (sOperator == ">>")
									eOperator = ENUM_OP_PRECEDENCE::OP_BITWISERIGHTSHIFT;
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
	if (m_vOperatorStack.empty()) m_vOperatorStack.push(sCurrentOperator);
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
	for (int i = 1; i <= iSize; i++)
	{
		sPostFixExpr += m_vPostFix[i - 1];
		if (i != iSize)
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
		m_sBlockStringList.erase(m_sBlockStringList.end() - 1);
	}
}

std::string	TinyCReader::getFullyQualifiedNameForVariable(Tree* pNode, std::string sVariable)
{
	std::string sFullyQualifiedName;

	for (Tree* pChild : pNode->m_vStatements)
	{
		if (pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPE
			|
			pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPEARRAY
		) {
			if (pChild->m_sAdditionalInfo == sVariable)
			{
				sFullyQualifiedName = pChild->m_sText;
				break;
			}
		}
	}

	for (Tree* pStaticVar : FunctionInfo::m_vStaticVariables)
	{
		if (pStaticVar->m_sText == sVariable)
		{
			sFullyQualifiedName = pStaticVar->m_sText;
			break;
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

void TinyCReader::addType(std::string sType)
{
	GrammerUtils::m_vTypes.push_back(sType);
}

bool TinyCReader::isValidType(std::string sType)
{
	for (std::string sInBuiltType : GrammerUtils::m_vTypes)
	{
		if (sType == sInBuiltType)
		{
			GrammerUtils::m_pPrevToken = GrammerUtils::m_pToken;
			GrammerUtils::getNextToken();

			return true;
		}
	}

	return false;
}


bool TinyCReader::def() {

	addType("int8_t");
	addType("int16_t");
	addType("int32_t");

	while (true) {
		if (functionList()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::functionList() {
	if (staticDeclarations()) {
		return true;
	}
	else
		if (functionDef()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::staticDeclarations() {
	if (!GrammerUtils::match("static", MANDATORY))
		return false;
	if (staticPtr()) {
		return true;
	}
	else
		return false;

	return true;

}

bool TinyCReader::primitiveType() {

	std::string sType = GrammerUtils::m_pToken.getText();
	return isValidType(sType);

	return true;

}

bool TinyCReader::staticPtr() {
	if (!primitiveType())
		return false;

	std::string sPointerType = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match('*', MANDATORY))
		return false;
	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	std::string sStaticVariableName = GrammerUtils::m_pPrevToken.getText();

	ASTNodeType eASTNodeType = ASTNodeType::ASTNode_TYPESTATIC;
	Tree* pStaticPtrNode = makeLeaf(eASTNodeType, sStaticVariableName.c_str());
	pStaticPtrNode->m_bIsPointerType = true;
	pStaticPtrNode->setAdditionalInfo("type", sPointerType);
	m_pASTCurrentNode->addChild(pStaticPtrNode);
	FunctionInfo::addStaticVariable(pStaticPtrNode);

	if (!GrammerUtils::match(';', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::functionDef() {
	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	std::string sReturnType = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match(TokenType::Type::TK_FUNCTIONCALL, MANDATORY))
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

	if (!GrammerUtils::match('(', MANDATORY))
		return false;

	Tree* pTemp = nullptr;
	{
		pTemp = m_pASTCurrentNode; // Save Root Node temporarily
		m_pASTCurrentNode = pArgListNode;
	}

	if (!functionArgumentDefList()) {
	}
	else {
	}


	m_pASTCurrentNode = pTemp;

	if (!GrammerUtils::match(')', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', MANDATORY))
		return false;

	pTemp = m_pASTCurrentNode; // Save Root Node temporarily
	m_pASTCurrentNode = pFunctionDefNode;

	Tree* pFuncStartNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONSTART, "");
	m_pASTCurrentNode->addChild(pFuncStartNode);

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', MANDATORY))
		return false;

	removeLastFromBlockString();

	Tree* pFuncEndNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONEND, "");
	m_pASTCurrentNode->addChild(pFuncEndNode);

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::functionArgumentDefList() {
	while (true) {
		if (functionArgumentDefListMore()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::functionArgumentDefListMore() {
	if (!primitiveTypeInt())
		return false;
	if (!GrammerUtils::match(',', OPTIONAL)) {

	}

	else {

	}

	return true;

}

bool TinyCReader::primitiveTypeInt() {
	if (!primitiveType())
		return false;

	std::string sType = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	std::string sArgName = GrammerUtils::m_pPrevToken.getText();
	std::string sFullyQualifiedArgName;
	sFullyQualifiedArgName.append(getBlockString());
	sFullyQualifiedArgName.append(sArgName);

	ASTNodeType eASTNodeType = ASTNodeType::ASTNode_TYPE;
	Tree* pPrimIntNode = makeLeaf(eASTNodeType, sFullyQualifiedArgName.c_str());
	{
		pPrimIntNode->m_sAdditionalInfo.append(sArgName);
		pPrimIntNode->setAdditionalInfo("type", sType);

		m_pASTCurrentNode->addChild(pPrimIntNode);
		m_pASTCurrentNode->m_sAdditionalInfo.append("I");
	}

	return true;

}

bool TinyCReader::stmt_list() {
	while (true) {
		if (stmt()) {
			if (!GrammerUtils::match(';', OPTIONAL)) {

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
	if (functionCall()) {
		return true;
	}
	else
		if (prePostFixedIncrDecr()) {
			return true;
		}
		else
			if (assignmentDerefArray()) {
				return true;
			}
			else
				if (newPtrOrArrayOrInt()) {
					return true;
				}
				else
					if (assignmentRHS()) {
						return true;
					}
					else
						if (ifelseStatement()) {
							return true;
						}
						else
							if (whileStatement()) {
								return true;
							}
							else
								if (forStatement()) {
									return true;
								}
								else
									if (switchStatement()) {
										return true;
									}
									else
										if (print()) {
											return true;
										}
										else
											if (putc()) {
												return true;
											}
											else
												if (bracesstmtlist()) {
													return true;
												}
												else
													if (returnStatement()) {
														return true;
													}
													else
														if (freePtrStatement()) {
															return true;
														}
														else
															return false;

	return true;

}

bool TinyCReader::prePostFixedIncrDecr() {
	if (preFixIncrDecr()) {
		return true;
	}
	else
		if (postFixIncrDecr()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::preFixIncrDecr() {
	if (GrammerUtils::match(TokenType::Type::TK_PREFIXDECR, OPTIONAL)) {

		std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREDECR, sFullyQualifiedVariableName.c_str());
		{
			m_pASTCurrentNode->addChild(pPreDecrNode);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType::Type::TK_PREFIXINCR, OPTIONAL)) {

			std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREINCR, sFullyQualifiedVariableName.c_str());
			{
				m_pASTCurrentNode->addChild(pPreDecrNode);
			}

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::postFixIncrDecr() {
	if (GrammerUtils::match(TokenType::Type::TK_POSTFIXDECR, OPTIONAL)) {

		std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTDECR, sFullyQualifiedVariableName.c_str());
		{
			m_pASTCurrentNode->addChild(pPreDecrNode);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType::Type::TK_POSTFIXINCR, OPTIONAL)) {

			std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTINCR, sFullyQualifiedVariableName.c_str());
			{
				m_pASTCurrentNode->addChild(pPreDecrNode);
			}

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::returnStatement() {
	if (!GrammerUtils::match("return", MANDATORY))
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

	if (!expr())
		return false;

	removeLastFromBlockString();

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
	pReturnStmtNode->addChild(pExpressionLeftLeaf);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pReturnStmtNode);

	return true;

}

bool TinyCReader::functionCall() {
	if (!GrammerUtils::match(TokenType::Type::TK_FUNCTIONCALL, MANDATORY))
		return false;

	std::string sIdentifier = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match('(', MANDATORY))
		return false;

	Tree* pFunctionCallNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONCALL, sIdentifier.c_str());
	Tree* pTemp = nullptr;
	{
		m_pASTCurrentNode->addChild(pFunctionCallNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pFunctionCallNode;
	}

	if (!functionArgumentList()) {
	}
	else {
	}

	if (!GrammerUtils::match(')', MANDATORY))
		return false;

	Tree* pFuncCallEndNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONCALLEND, sIdentifier.c_str());
	m_pASTCurrentNode->addChild(pFuncCallEndNode);

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::functionArgumentList() {
	while (true) {
		if (functionArgumentItem()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::functionArgumentItem() {
	if (GrammerUtils::match(TokenType::Type::TK_STRING, OPTIONAL)) {

		Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, GrammerUtils::m_pPrevToken.getText());
		{
			m_pASTCurrentNode->addChild(pStringNode);
			m_pASTCurrentNode->m_sAdditionalInfo.append("S");
		}

		if (!GrammerUtils::match(',', OPTIONAL)) {

		}

		else {

		}

		return true;
	}
	else
		if (expr()) {

			Tree* pExpressionLeftLeaf = createPostFixExpr();
			{
				m_pASTCurrentNode->addChild(pExpressionLeftLeaf);
				pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;

				m_pASTCurrentNode->m_sAdditionalInfo.append("I");
			}

			if (!GrammerUtils::match(',', OPTIONAL)) {

			}

			else {

			}

			return true;
		}
		else
			if (GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

				Tree* pIntegerNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, GrammerUtils::m_pPrevToken.getText());
				{
					m_pASTCurrentNode->addChild(pIntegerNode);
					m_pASTCurrentNode->m_sAdditionalInfo.append("C");
				}

				return true;
			}
			else
				if (GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

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

bool TinyCReader::ifelseStatement() {
	if (!ifStatement())
		return false;
	return true;

}

bool TinyCReader::ifStatement() {
	if (!GrammerUtils::match("if", MANDATORY))
		return false;
	if (!GrammerUtils::match('(', MANDATORY))
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

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
	m_pASTCurrentNode = pIfNode;

	if (!GrammerUtils::match(')', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL)) {

	}

	else {

	}


	removeLastFromBlockString();

	if (!elseStatement()) {
	}
	else {
	}


	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pIfNode);

	return true;

}

bool TinyCReader::elseStatement() {
	if (!GrammerUtils::match("else", MANDATORY))
		return false;

	updateBlockString("else");

	Tree* pElseNode = makeLeaf(ASTNodeType::ASTNode_ELSE, GrammerUtils::m_pPrevToken.getText());
	Tree* pIfNode = nullptr;
	{
		pIfNode = m_pASTCurrentNode;
		pElseNode->m_pParentNode = pIfNode->m_pParentNode;

		m_pASTCurrentNode = pElseNode;
	}

	if (!GrammerUtils::match('{', OPTIONAL)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL)) {

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

bool TinyCReader::whileStatement() {
	if (!GrammerUtils::match("while", MANDATORY))
		return false;
	if (!GrammerUtils::match('(', MANDATORY))
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

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
	m_pASTCurrentNode = pWhileNode;

	if (!GrammerUtils::match(')', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL)) {

	}

	else {

	}


	removeLastFromBlockString();

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pWhileNode);

	return true;

}

bool TinyCReader::switchStatement() {
	if (!GrammerUtils::match("switch", MANDATORY))
		return false;
	if (!GrammerUtils::match('(', MANDATORY))
		return false;

	updateBlockString("switch");

	Tree* pTemp = nullptr;
	Tree* pSwitchNode = makeLeaf(ASTNodeType::ASTNode_SWITCH, "switch");
	{
		pSwitchNode->m_pParentNode = m_pASTCurrentNode;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSwitchNode;
	}

	if (!switchArgument())
		return false;
	if (!GrammerUtils::match(')', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', MANDATORY))
		return false;
	if (!oneOrMoreCasesOrDefault())
		return false;
	if (!GrammerUtils::match('}', MANDATORY))
		return false;

	removeLastFromBlockString();

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pSwitchNode);

	return true;

}

bool TinyCReader::switchArgument() {
	if (GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

		std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		assert(!sFullyQualifiedVariableName.empty());

		Tree* pSwitchArgumentNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
		m_pASTCurrentNode->m_pLeftNode = pSwitchArgumentNode;

		return true;
	}
	else
		if (GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

			Tree* pSwitchArgumentNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, GrammerUtils::m_pPrevToken.getText());
			m_pASTCurrentNode->m_pLeftNode = pSwitchArgumentNode;

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::oneOrMoreCasesOrDefault() {
	if (!oneOrMoreSwitchCases()) {
	}
	else {
	}

	if (!defaultCase()) {
	}
	else {
	}

	return true;

}

bool TinyCReader::oneOrMoreSwitchCases() {
	while (true) {
		if (switchCase()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::switchCase() {
	if (!GrammerUtils::match("case", MANDATORY))
		return false;
	if (!GrammerUtils::match(TokenType::Type::TK_INTEGER, MANDATORY))
		return false;

	updateBlockString("switchcase");

	Tree* pTemp = nullptr;
	Tree* pSwitchCaseNode = makeLeaf(ASTNodeType::ASTNode_SWITCHCASE, GrammerUtils::m_pPrevToken.getText());
	m_pASTCurrentNode->addChild(pSwitchCaseNode);
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSwitchCaseNode;
	}

	if (!GrammerUtils::match(':', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL)) {

	}

	else {

	}

	if (!GrammerUtils::match("break", OPTIONAL)) {

	}

	else {


		Tree* pSwitchBreakNode = makeLeaf(ASTNodeType::ASTNode_SWITCHBREAK, "break");
		m_pASTCurrentNode->addChild(pSwitchBreakNode);

		if (!GrammerUtils::match(';', MANDATORY))
			return false;
	}


	removeLastFromBlockString();
	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::defaultCase() {
	if (!GrammerUtils::match("default", MANDATORY))
		return false;

	updateBlockString("switchcase");

	Tree* pTemp = nullptr;
	Tree* pSwitchDefaultNode = makeLeaf(ASTNodeType::ASTNode_SWITCHDEFAULT, GrammerUtils::m_pPrevToken.getText());
	m_pASTCurrentNode->addChild(pSwitchDefaultNode);
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSwitchDefaultNode;
	}

	if (!GrammerUtils::match(':', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL)) {

	}

	else {

	}


	removeLastFromBlockString();
	m_pASTCurrentNode = pTemp;

	if (!GrammerUtils::match("break", MANDATORY))
		return false;
	if (!GrammerUtils::match(';', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::forStatement() {
	if (!GrammerUtils::match("for", MANDATORY))
		return false;
	if (!GrammerUtils::match('(', MANDATORY))
		return false;

	updateBlockString("for");

	////////////////////////////////
	// for ( init-expression ; cond-expression ; loop-expression ) 
	// {
	//		statement;
	// }
	////////////////////////////////
	// init-expression;
	// while(cond-expression)
	// {
	//   	...
	//		...
	//		loop-expression
	// }
	////////////////////////////////
	// A "for" loop is an extended "while" loop where:
	// 		- The "init-expression" list forms a set of statements before the "while" block.
	//		- The "cond-expression" forms the "cond-expression" of the "while" block.
	//		- And the "loop-expression" are the set of statements that are at the tail-end of the "while" block.
	////////////////////////////////

	if (!oneOrMoreInitExprs()) {
	}
	else {
	}

	if (!GrammerUtils::match(';', MANDATORY))
		return false;

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

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	if (!GrammerUtils::match(';', MANDATORY))
		return false;

	Tree* pFor_LoopExpressionsLeaf = makeLeaf(ASTNodeType::ASTNode_INVALID, "");
	{
		pFor_LoopExpressionsLeaf->m_pParentNode = pWhileNode;
		m_pASTCurrentNode = pFor_LoopExpressionsLeaf;
	}

	if (!oneOrMoreLoopExprs()) {
	}
	else {
	}

	if (!GrammerUtils::match(')', MANDATORY))
		return false;

	m_pASTCurrentNode = pWhileNode;

	if (!GrammerUtils::match('{', OPTIONAL)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL)) {

	}

	else {

	}


	removeLastFromBlockString();

	for (Tree* pLoopExpr : pFor_LoopExpressionsLeaf->m_vStatements)
	{
		pWhileNode->addChild(pLoopExpr);
	}

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pWhileNode);

	return true;

}

bool TinyCReader::oneOrMoreInitExprs() {
	if (!initExpr())
		return false;
	while (true) {
		if (GrammerUtils::match(',', OPTIONAL)) {

			if (!initExpr())
				return false;
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::initExpr() {
	if (newPtrOrArrayOrInt()) {
		return true;
	}
	else
		if (assignmentRHS()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::oneOrMoreLoopExprs() {
	if (!loopExpr())
		return false;
	while (true) {
		if (GrammerUtils::match(',', OPTIONAL)) {

			if (!loopExpr())
				return false;
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::loopExpr() {
	if (prePostFixedIncrDecr()) {
		return true;
	}
	else
		if (assignmentRHS()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::print() {
	if (!GrammerUtils::match("print", MANDATORY))
		return false;
	if (!GrammerUtils::match('(', MANDATORY))
		return false;

	Tree* pPrintNode = makeLeaf(ASTNodeType::ASTNode_PRINT, GrammerUtils::m_pPrevToken.getText());
	Tree* pTemp = nullptr;
	{
		m_pASTCurrentNode->addChild(pPrintNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pPrintNode;
	}

	if (!print_list())
		return false;
	if (!GrammerUtils::match(')', MANDATORY))
		return false;

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::print_list() {
	while (true) {
		if (print_list_0()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::print_list_0() {
	if (GrammerUtils::match(TokenType::Type::TK_STRING, OPTIONAL)) {

		Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, GrammerUtils::m_pPrevToken.getText());
		{
			m_pASTCurrentNode->addChild(pStringNode);
		}

		if (!GrammerUtils::match(',', OPTIONAL)) {

		}

		else {

		}

		return true;
	}
	else
		if (expr()) {

			Tree* pExpressionLeftLeaf = createPostFixExpr();
			{
				m_pASTCurrentNode->addChild(pExpressionLeftLeaf);
				pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;
			}

			if (!GrammerUtils::match(',', OPTIONAL)) {

			}

			else {

			}

			return true;
		}
		else
			if (GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

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
	if (!GrammerUtils::match("putc", MANDATORY))
		return false;

	Tree* pPutCNode = makeLeaf(ASTNodeType::ASTNode_PUTC, GrammerUtils::m_pPrevToken.getText());
	Tree* pTemp = nullptr;
	{
		m_pASTCurrentNode->addChild(pPutCNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pPutCNode;
	}

	if (!GrammerUtils::match('(', MANDATORY))
		return false;
	if (!putcList())
		return false;
	if (!GrammerUtils::match(')', MANDATORY))
		return false;

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::putcList() {
	if (GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

		std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		assert(!sFullyQualifiedVariableName.empty());

		Tree* pIdentifierNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
		m_pASTCurrentNode->addChild(pIdentifierNode);

		return true;
	}
	else
		if (GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

			Tree* pIntegerNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, GrammerUtils::m_pPrevToken.getText());
			m_pASTCurrentNode->addChild(pIntegerNode);

			return true;
		}
		else
			if (GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

				Tree* pCharacterNode = makeLeaf(ASTNodeType::ASTNode_CHARACTER, GrammerUtils::m_pPrevToken.getText());
				m_pASTCurrentNode->addChild(pCharacterNode);

				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::bracesstmtlist() {
	if (!GrammerUtils::match('{', MANDATORY))
		return false;

	updateBlockString("{");

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', MANDATORY))
		return false;

	removeLastFromBlockString();

	return true;

}

bool TinyCReader::newPtrOrArrayOrInt() {
	if (!primitiveType())
		return false;
	if (!primPtrOrArrayOrInt())
		return false;
	return true;

}

bool TinyCReader::primPtrOrArrayOrInt() {
	if (primPtr()) {
		return true;
	}
	else
		if (primArrayOrInt()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::primPtr() {

	std::string sPointerType = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match('*', MANDATORY))
		return false;
	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	if (!GrammerUtils::match('=', MANDATORY))
		return false;

	ASTNodeType eASTNodeType = ASTNodeType::ASTNode_TYPE;
	Tree* pPrimPtrNode = makeLeaf(eASTNodeType, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		pTemp = m_pASTCurrentNode;
		pPrimPtrNode->m_sAdditionalInfo.append(sVariableName);
		pPrimPtrNode->m_bIsPointerType = true;
		pPrimPtrNode->setAdditionalInfo("type", sPointerType);
		pPrimPtrNode->m_pParentNode = m_pASTCurrentNode;
		m_pASTCurrentNode = pPrimPtrNode;
	}

	if (!ptrAssign())
		return false;

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pPrimPtrNode);

	return true;

}

bool TinyCReader::primArrayOrInt() {
	if (primArray()) {
		return true;
	}
	else
		if (primType()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::primArray() {

	std::string sPrimitiveType = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match(TokenType::Type::TK_DEREFARRAY, MANDATORY))
		return false;

	std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	if (!GrammerUtils::match('[', MANDATORY))
		return false;

	ASTNodeType eASTNodeType = ASTNodeType::ASTNode_TYPEARRAY;
	Tree* pPrimTypeArrayNode = makeLeaf(eASTNodeType, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		pTemp = m_pASTCurrentNode;

		pPrimTypeArrayNode->m_sAdditionalInfo.append(sVariableName);
		pPrimTypeArrayNode->m_bIsPointerType = true;
		pPrimTypeArrayNode->setAdditionalInfo("type", sPrimitiveType);
		pPrimTypeArrayNode->m_pParentNode = m_pASTCurrentNode;
	}

	if (!GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

	}

	else {


		Tree* pArraySizeLeaf = makeLeaf(ASTNodeType::ASTNode_INTEGER, GrammerUtils::m_pPrevToken.getText());
		{
			pPrimTypeArrayNode->m_pLeftNode = pArraySizeLeaf;
			pArraySizeLeaf->m_pParentNode = pPrimTypeArrayNode;
		}

	}


	m_pASTCurrentNode = pPrimTypeArrayNode;

	if (!primArrayOptionalRHS()) {
	}
	else {
	}


	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pPrimTypeArrayNode);

	return true;

}

bool TinyCReader::primArrayOptionalRHS() {
	if (!GrammerUtils::match(']', MANDATORY))
		return false;
	if (!GrammerUtils::match('=', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', MANDATORY))
		return false;

	Tree* pPrimTypeArrayNode = m_pASTCurrentNode;
	Tree* pArrayElementsLeaf = makeLeaf(ASTNodeType::ASTNode_TYPEARRAYELEMENTS, "");
	{
		pPrimTypeArrayNode->m_pRightNode = pArrayElementsLeaf;
		pArrayElementsLeaf->m_pParentNode = pPrimTypeArrayNode;
	}

	while (true) {

		Tree* pExpressionArrayElementLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
		{
			pExpressionArrayElementLeaf->m_pParentNode = pArrayElementsLeaf;
			m_pASTCurrentNode = pExpressionArrayElementLeaf;
		}

		if (expr()) {

			m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
			pArrayElementsLeaf->addChild(pExpressionArrayElementLeaf);

			if (!GrammerUtils::match(',', OPTIONAL)) {

			}

			else {

			}

		}
		else
			break;
	}

	if (!GrammerUtils::match('}', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::primType() {

	std::string sPrimitiveType = GrammerUtils::m_pPrevToken.getText();

	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	if (!GrammerUtils::match('=', MANDATORY))
		return false;

	ASTNodeType eASTNodeType = ASTNodeType::ASTNode_TYPE;
	Tree* pPrimIntNode = makeLeaf(eASTNodeType, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		pPrimIntNode->m_sAdditionalInfo.append(sVariableName);
		pPrimIntNode->setAdditionalInfo("type", sPrimitiveType);
		pPrimIntNode->m_pParentNode = m_pASTCurrentNode;
	}

	Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pPrimIntNode->m_pLeftNode = pExpressionLeftLeaf;
		pExpressionLeftLeaf->m_pParentNode = pPrimIntNode;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pExpressionLeftLeaf;
	}

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pPrimIntNode);

	return true;

}

bool TinyCReader::ptrAssign() {
	if (malloc()) {
		return true;
	}
	else
		return false;

	return true;

}

bool TinyCReader::malloc() {
	if (!GrammerUtils::match("malloc", MANDATORY))
		return false;

	Tree* pMallocNode = makeLeaf(ASTNodeType::ASTNode_MALLOC, "");
	{
		pMallocNode->m_pParentNode = m_pASTCurrentNode;
	}

	Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	Tree* pTemp = nullptr;
	{
		pMallocNode->m_pLeftNode = pExpressionLeftLeaf;
		pExpressionLeftLeaf->m_pParentNode = pMallocNode;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pExpressionLeftLeaf;
	}


	if (!GrammerUtils::match('(', MANDATORY))
		return false;
	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pMallocNode);

	if (!GrammerUtils::match(')', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::assignmentDerefArray() {
	if (!GrammerUtils::match(TokenType::Type::TK_DEREFARRAY, MANDATORY))
		return false;

	TokenType::Type eTokenType = GrammerUtils::m_pPrevToken.m_eTokenType;
	std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
	assert(!sFullyQualifiedVariableName.empty());

	Tree* pTemp = nullptr;
	Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sFullyQualifiedVariableName.c_str());
	{
		pAssignmentNode->m_pParentNode = m_pASTCurrentNode;
	}

	Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_DEREFARRAY, sFullyQualifiedVariableName.c_str());
	Tree* pArrayIndexExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pIdentifierLeaf->m_sAdditionalInfo = sVariableName;

		pAssignmentNode->m_pRightNode = pIdentifierLeaf;
		pIdentifierLeaf->m_pParentNode = pAssignmentNode;

		pTemp = m_pASTCurrentNode;

		pIdentifierLeaf->m_pLeftNode = pArrayIndexExpressionLeaf;
		pArrayIndexExpressionLeaf->m_pParentNode = pIdentifierLeaf;

		m_pASTCurrentNode = pArrayIndexExpressionLeaf;
	}

	if (!GrammerUtils::match('[', MANDATORY))
		return false;
	if (!expr())
		return false;

	pArrayIndexExpressionLeaf = createPostFixExpr(pArrayIndexExpressionLeaf);
	pIdentifierLeaf->addChild(pArrayIndexExpressionLeaf);

	if (!GrammerUtils::match(']', MANDATORY))
		return false;
	if (!GrammerUtils::match('=', MANDATORY))
		return false;

	Tree* pRValueExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pAssignmentNode->m_pLeftNode = pRValueExpressionLeaf;
		pRValueExpressionLeaf->m_pParentNode = pAssignmentNode;

		m_pASTCurrentNode = pRValueExpressionLeaf;
	}

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAssignmentNode);

	return true;

}

bool TinyCReader::arrayDeclaration() {
	if (!primitiveType())
		return false;
	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;
	if (!GrammerUtils::match('[', MANDATORY))
		return false;
	if (!expr()) {
	}
	else {
	}

	if (!GrammerUtils::match(']', MANDATORY))
		return false;
	if (arrayDeclaration_Option0()) {
		return true;
	}
	else
		if (arrayDeclaration_Option1()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::arrayDeclaration_Option0() {

	std::string sSemiColon = GrammerUtils::m_pToken.getText();
	return (sSemiColon == ";");

	return true;

}

bool TinyCReader::arrayDeclaration_Option1() {
	if (!GrammerUtils::match('=', MANDATORY))
		return false;
	if (!GrammerUtils::match('{', MANDATORY))
		return false;
	while (true) {
		if (expr()) {
			if (!GrammerUtils::match(',', OPTIONAL)) {

			}

			else {

			}

		}
		else
			break;
	}

	if (!GrammerUtils::match('}', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::assignmentRHS() {
	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	TokenType::Type eTokenType = GrammerUtils::m_pPrevToken.m_eTokenType;
	std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
	assert(!sFullyQualifiedVariableName.empty());

	Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sFullyQualifiedVariableName.c_str());
	{
		pAssignmentNode->m_pParentNode = m_pASTCurrentNode;
	}

	Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
	{
		pIdentifierLeaf->m_sAdditionalInfo = sVariableName;
		pAssignmentNode->m_pRightNode = pIdentifierLeaf;
	}

	if (!GrammerUtils::match('=', MANDATORY))
		return false;

	Tree* pTemp = nullptr;
	Tree* pRValueExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pAssignmentNode->m_pLeftNode = pRValueExpressionLeftLeaf;
		pRValueExpressionLeftLeaf->m_pParentNode = pAssignmentNode;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pRValueExpressionLeftLeaf;
	}

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAssignmentNode);

	return true;

}

bool TinyCReader::freePtrStatement() {
	if (!GrammerUtils::match("free", MANDATORY))
		return false;
	if (!GrammerUtils::match('(', MANDATORY))
		return false;
	if (!GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, MANDATORY))
		return false;

	std::string sVariableName = GrammerUtils::m_pPrevToken.getText();
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
	assert(!sFullyQualifiedVariableName.empty());

	Tree* pFreePtrLeaf = makeLeaf(ASTNodeType::ASTNode_FREE, sFullyQualifiedVariableName.c_str());
	m_pASTCurrentNode->addChild(pFreePtrLeaf);

	if (!GrammerUtils::match(')', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::expr() {
	if (ptrAssign()) {
		return true;
	}
	else
		if (startExpr()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::startExpr() {
	if (!equalityExpr())
		return false;
	while (true) {
		if (logicalAndOrExpr()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::logicalAndOrExpr() {
	if (!logicalAndOr())
		return false;
	if (!equalityExpr())
		return false;
	return true;

}

bool TinyCReader::logicalAndOr() {
	if (GrammerUtils::match("&&", OPTIONAL)) {

		checkOpPrecedenceAndPush("&&");

		return true;
	}
	else
		if (GrammerUtils::match("||", OPTIONAL)) {

			checkOpPrecedenceAndPush("||");

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::equalityExpr() {
	if (!relationalExpr())
		return false;
	while (true) {
		if (eqNeqExpr()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::eqNeqExpr() {
	if (!eqNeq())
		return false;
	if (!relationalExpr())
		return false;
	return true;

}

bool TinyCReader::eqNeq() {
	if (GrammerUtils::match("==", OPTIONAL)) {

		checkOpPrecedenceAndPush("==");

		return true;
	}
	else
		if (GrammerUtils::match("!=", OPTIONAL)) {

			checkOpPrecedenceAndPush("!=");

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::relationalExpr() {
	if (!addSubExpr())
		return false;
	while (true) {
		if (lteqGteqExpr()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::lteqGteqExpr() {
	if (!lteqGteq())
		return false;
	if (!addSubExpr())
		return false;
	return true;

}

bool TinyCReader::lteqGteq() {
	if (GrammerUtils::match('<', OPTIONAL)) {

		checkOpPrecedenceAndPush("<");

		return true;
	}
	else
		if (GrammerUtils::match("<=", OPTIONAL)) {

			checkOpPrecedenceAndPush("<=");

			return true;
		}
		else
			if (GrammerUtils::match('>', OPTIONAL)) {

				checkOpPrecedenceAndPush(">");

				return true;
			}
			else
				if (GrammerUtils::match(">=", OPTIONAL)) {

					checkOpPrecedenceAndPush(">=");

					return true;
				}
				else
					return false;

	return true;

}

bool TinyCReader::addSubExpr() {
	if (!mulDivExpr())
		return false;
	while (true) {
		if (plusMinusExpr()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::plusMinusExpr() {
	if (!plusMinus())
		return false;
	if (!mulDivExpr())
		return false;
	return true;

}

bool TinyCReader::plusMinus() {
	if (GrammerUtils::match('+', OPTIONAL)) {

		checkOpPrecedenceAndPush("+");

		return true;
	}
	else
		if (GrammerUtils::match('-', OPTIONAL)) {

			checkOpPrecedenceAndPush("-");

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::mulDivExpr() {
	if (!bitwiseExpr())
		return false;
	while (true) {
		if (mulDivModExpr()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::mulDivModExpr() {
	if (!mulDivMod())
		return false;
	if (!bitwiseExpr())
		return false;
	return true;

}

bool TinyCReader::mulDivMod() {
	if (GrammerUtils::match('*', OPTIONAL)) {

		checkOpPrecedenceAndPush("*");

		return true;
	}
	else
		if (GrammerUtils::match('/', OPTIONAL)) {

			checkOpPrecedenceAndPush("/");

			return true;
		}
		else
			if (GrammerUtils::match('%', OPTIONAL)) {

				checkOpPrecedenceAndPush("%");

				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::bitwiseExpr() {
	if (!primary())
		return false;
	while (true) {
		if (bitwiseOrAndXorExpr()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::bitwiseOrAndXorExpr() {
	if (!bitwiseOrAndXor())
		return false;
	if (!primary())
		return false;
	return true;

}

bool TinyCReader::bitwiseOrAndXor() {
	if (GrammerUtils::match('&', OPTIONAL)) {

		checkOpPrecedenceAndPush("&");

		return true;
	}
	else
		if (GrammerUtils::match('|', OPTIONAL)) {

			checkOpPrecedenceAndPush("|");

			return true;
		}
		else
			if (GrammerUtils::match('^', OPTIONAL)) {

				checkOpPrecedenceAndPush("^");

				return true;
			}
			else
				if (GrammerUtils::match("<<", OPTIONAL)) {

					checkOpPrecedenceAndPush("<<");

					return true;
				}
				else
					if (GrammerUtils::match(">>", OPTIONAL)) {

						checkOpPrecedenceAndPush(">>");

						return true;
					}
					else
						return false;

	return true;

}

bool TinyCReader::primary() {
	if (operands()) {
		return true;
	}
	else
		if (unary_expr()) {
			return true;
		}
		else
			if (paren_expr()) {
				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::operands() {

	std::string sOperand;

	if (functionCall()) {

		// The idea here is to create a temporary variable of the type returned by the function
		// & add it before the expression statement.
		// The temporary variable is then inserted in the expression.
		// Eg:
		// 		int32_t iRet = 10;
		//		iRet = 10 + retFunc(); // where retFunc return type is "int32_t".
		//		This will create a dummy code as follows:
		//			int32_t iRet = 10;
		//			int32_t iRet_retFunc = retFunc;	// This line of code will be inserted by the following piece of code.
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

			Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedTempVariableName.c_str());
			{
				pPrimIntNode->m_sAdditionalInfo.append(sFullyQualifiedTempVariableName);
				pPrimIntNode->setAdditionalInfo("type", "int32_t");
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
		if (tk_identifier()) {
			return true;
		}
		else
			if (GrammerUtils::match(TokenType::Type::TK_INTEGER, OPTIONAL)) {

				sOperand = GrammerUtils::m_pPrevToken.getText();
				m_vPostFix.push_back(sOperand);

				return true;
			}
			else
				if (GrammerUtils::match(TokenType::Type::TK_CHARACTER, OPTIONAL)) {

					sOperand = GrammerUtils::m_pPrevToken.getText();
					char pStr[255] = { 0 };
					sprintf_s(pStr, "%d", sOperand.c_str()[0]);

					m_vPostFix.push_back(pStr);

					return true;
				}
				else
					return false;

	return true;

}

bool TinyCReader::tk_identifier() {
	if (preFixInExpr()) {
		return true;
	}
	else
		if (postFixIncrDecrInExpr()) {
			return true;
		}
		else
			if (GrammerUtils::match(TokenType::Type::TK_IDENTIFIER, OPTIONAL)) {

				std::string sOperand = GrammerUtils::m_pPrevToken.getText();
				std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sOperand);
				assert(!sFullyQualifiedVariableName.empty());
				if (!sFullyQualifiedVariableName.empty())
				{
					sOperand = sFullyQualifiedVariableName;
				}

				m_vPostFix.push_back(sOperand);

				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::unary_expr() {
	if (!unary_oper())
		return false;
	if (!expr())
		return false;
	return true;

}

bool TinyCReader::unary_oper() {
	if (GrammerUtils::match('-', OPTIONAL)) {

		checkOpPrecedenceAndPush("NEGATE");

		return true;
	}
	else
		if (GrammerUtils::match('!', OPTIONAL)) {

			checkOpPrecedenceAndPush("!");

			return true;
		}
		else
			if (GrammerUtils::match('~', OPTIONAL)) {

				checkOpPrecedenceAndPush("~");

				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::preFixInExpr() {
	if (GrammerUtils::match(TokenType::Type::TK_PREFIXDECR, OPTIONAL)) {

		if (m_pASTCurrentNode->m_pLeftNode == nullptr)
		{
			Tree* pPreFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_PREFIX, "");
			m_pASTCurrentNode->m_pLeftNode = pPreFixNode;
		}

		std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREDECR, sFullyQualifiedVariableName.c_str());
		{
			m_pASTCurrentNode->m_pLeftNode->addChild(pPreDecrNode);
			m_vPostFix.push_back(sFullyQualifiedVariableName);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType::Type::TK_PREFIXINCR, OPTIONAL)) {

			if (m_pASTCurrentNode->m_pLeftNode == nullptr)
			{
				Tree* pPreFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_PREFIX, "");
				m_pASTCurrentNode->m_pLeftNode = pPreFixNode;
			}

			std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPreIncrNode = makeLeaf(ASTNodeType::ASTNode_PREINCR, sFullyQualifiedVariableName.c_str());
			{
				m_pASTCurrentNode->m_pLeftNode->addChild(pPreIncrNode);
				m_vPostFix.push_back(sFullyQualifiedVariableName);
			}

			return true;
		}
		else
			if (rValueDeref()) {
				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::rValueDeref() {
	if (!GrammerUtils::match(TokenType::Type::TK_DEREFARRAY, MANDATORY))
		return false;

	std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);

	if (!GrammerUtils::match('[', MANDATORY))
		return false;

	checkOpPrecedenceAndPush("(");

	if (!expr())
		return false;

	checkOpPrecedenceAndPush(")");


	m_vPostFix.push_back(sFullyQualifiedVariableName);
	m_vPostFix.push_back("@");

	if (!GrammerUtils::match(']', MANDATORY))
		return false;
	return true;

}

bool TinyCReader::postFixIncrDecrInExpr() {
	if (GrammerUtils::match(TokenType::Type::TK_POSTFIXDECR, OPTIONAL)) {

		if (m_pASTCurrentNode->m_pRightNode == nullptr)
		{
			Tree* pPostFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_POSTFIX, "");
			m_pASTCurrentNode->m_pRightNode = pPostFixNode;
		}

		std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPostDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTDECR, sFullyQualifiedVariableName.c_str());
		{
			m_pASTCurrentNode->m_pRightNode->addChild(pPostDecrNode);
			m_vPostFix.push_back(sFullyQualifiedVariableName);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType::Type::TK_POSTFIXINCR, OPTIONAL)) {

			if (m_pASTCurrentNode->m_pRightNode == nullptr)
			{
				Tree* pPostFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_POSTFIX, "");
				m_pASTCurrentNode->m_pRightNode = pPostFixNode;
			}

			std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPostIncrNode = makeLeaf(ASTNodeType::ASTNode_POSTINCR, sFullyQualifiedVariableName.c_str());
			{
				m_pASTCurrentNode->m_pRightNode->addChild(pPostIncrNode);
				m_vPostFix.push_back(sFullyQualifiedVariableName);
			}

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::paren_expr() {
	if (!GrammerUtils::match('(', MANDATORY))
		return false;

	checkOpPrecedenceAndPush("(");

	if (!expr())
		return false;
	if (!GrammerUtils::match(')', MANDATORY))
		return false;

	checkOpPrecedenceAndPush(")");

	return true;

}

