#include "TinyCReader.h"
#include "GrammerUtils.h"

#define __START_BLOCK_STRING__(_BLOCKNAME_) \
	updateBlockString(_BLOCKNAME_); \
	startBlockMarker(); \

#define __END_CURRENT_BLOCK__ \
	removeLastFromBlockString(); \
	endBlockMarker(); \

#define __START_INTERFACE__ 										m_bInterfaceInProcess = true;
#define __END_INTERFACE__ 											m_bInterfaceInProcess = false;

#define __START_STRUCT__ 											m_bStructInProcess = true;
#define __END_STRUCT__ 												m_bStructInProcess = false;

#define __START_FUNCTION__ 											m_bFunctionInProcess = true;
#define __END_FUNCTION__ 											m_bFunctionInProcess = false;

#define SAVE_CURRENT_TOKEN											GrammerUtils::m_pSavedToken = GrammerUtils::m_pToken;
#define SAVE_PREV_TOKEN												GrammerUtils::m_pSavedToken = GrammerUtils::m_pPrevToken;

#define SAVED_TOKEN 												GrammerUtils::m_pSavedToken
#define SAVED_TOKEN_TEXT 											GrammerUtils::m_pSavedToken.getText()
#define PREV_TOKEN_TEXT 											GrammerUtils::m_pPrevToken.getText()

#define GET_INFO_FOR_KEY(__node__, __key__)							__node__->getAdditionalInfoFor(__key__)
#define SET_INFO_FOR_KEY(__node__, __key__, __info__)				__node__->setAdditionalInfo(__key__, __info__)
#define APPEND_INFO_FOR_KEY(__node__, __key__, __appendValue__)		__node__->appendAdditionalInfo(__key__, __appendValue__)

TinyCReader::TinyCReader()
	: m_bStructInProcess(false)
	, m_bFunctionInProcess(false)
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
		pLeaf->setAdditionalInfo("text", sPostFixExpr);

	return pLeaf;
}

Tree* TinyCReader::makeLeaf(ASTNodeType eASTNodeType, const char* sText)
{
	Tree* pLeaf = new Tree();
	{
		pLeaf->m_eASTNodeType = eASTNodeType;
		pLeaf->setAdditionalInfo("text", sText);
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
	bool bFound = false;

	if (NOT bFound)
	{
		for (Tree* pChild : pNode->m_vStatements)
		{
			if (pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPE
				|
				pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPEARRAY
				|
				pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPESTRUCT
				) {
				if (pChild->m_sAdditionalInfo == sVariable)
				{
					sFullyQualifiedName = pChild->getAdditionalInfoFor("text");
					bFound = true;
					break;
				}
			}
		}
	}

	if (NOT bFound)
	{
		for (Tree* pStaticVar : FunctionInfo::m_vStaticVariables)
		{
			if (pStaticVar->getAdditionalInfoFor("text") == sVariable)
			{
				sFullyQualifiedName = pStaticVar->getAdditionalInfoFor("text");
				bFound = true;
				break;
			}
		}
	}

	// Check for 'Member Variables'
	if (NOT bFound)
	{
		Tree* pStructNode = pNode->m_pParentNode;
	LABEL_STRUCT_INFO:
		if (pStructNode->m_eASTNodeType == ASTNodeType::ASTNode_STRUCTDEF)
		{
			// 1. Check if its a Member Variable of the 'Struct'
			for (Tree* pChildNode : pStructNode->m_vStatements) // Starts with index 0
			{
				if (pChildNode->m_eASTNodeType == ASTNodeType::ASTNode_TYPE
					||
					pChildNode->m_eASTNodeType == ASTNodeType::ASTNode_TYPEARRAY
					) {
					if (pChildNode->m_sAdditionalInfo == sVariable)
					{
						sFullyQualifiedName = pChildNode->getAdditionalInfoFor("text");
						bFound = true;
						break;
					}
				}
			}

			// 2. Check if its a Member Variable of the Parent 'Struct'
			if (NOT bFound)
			{
				std::string sStructParentName = GET_INFO_FOR_KEY(pStructNode, "extends");
				if (NOT sStructParentName.empty())
				{
					pStructNode = getStructNodeByName(sStructParentName);
					goto LABEL_STRUCT_INFO;
				}
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

void TinyCReader::addStructType(std::string sType)
{
	GrammerUtils::m_vUserDefinedTypes.push_back(sType);
}

bool TinyCReader::isValidStructType(std::string sType)
{
	for (std::string sInBuiltType : GrammerUtils::m_vUserDefinedTypes)
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

void TinyCReader::addInterfaceType(std::string sType)
{
	GrammerUtils::m_vUserDefinedInterfaces.push_back(sType);
}

bool TinyCReader::isValidInterfaceType(std::string sType)
{
	for (std::string sInBuiltInterface : GrammerUtils::m_vUserDefinedInterfaces)
	{
		if (sType == sInBuiltInterface)
		{
			GrammerUtils::m_pPrevToken = GrammerUtils::m_pToken;
			GrammerUtils::getNextToken();

			return true;
		}
	}

	return false;
}

Tree* TinyCReader::hasNodeOfType(Tree* pNode, ASTNodeType eASTNodeType)
{
	Tree* pReturnNode = nullptr;
	if (pNode != nullptr)
	{
		for (Tree* pChild : pNode->m_vStatements)
		{
			if (pChild != nullptr)
			{
				if (pChild->m_eASTNodeType == eASTNodeType)
				{
					pReturnNode = pChild;
					break;
				}
			}
		}
	}

	return pReturnNode;
}

void TinyCReader::startBlockMarker()
{
	m_vLocalHeapVarStack.push("{");
}

void TinyCReader::pushLocalHeapVar(std::string sVariableName)
{
	if (NOT sVariableName.empty())
	{
		m_vLocalHeapVarStack.push(sVariableName);
	}
}

void TinyCReader::endBlockMarker()
{
	while (NOT m_vLocalHeapVarStack.empty())
	{
		std::string sLocalHeapVar = m_vLocalHeapVarStack.top();
		m_vLocalHeapVarStack.pop();
		if (sLocalHeapVar == "{")
		{
			break;
		}
		else
		{
			Tree* pFreeASTNode = GrammerUtils::createNodeOfType(ASTNodeType::ASTNode_FREE, sLocalHeapVar.c_str());
			if (pFreeASTNode != nullptr)
			{
				m_pASTCurrentNode->addChild(pFreeASTNode);
			}
		}
	}
}

E_VARIABLESCOPE TinyCReader::getCurrentScope()
{
	E_VARIABLESCOPE eCurrentScope = E_VARIABLESCOPE::INVALID;
	if (m_bStructInProcess)
	{
		if (m_bFunctionInProcess)
		{
			eCurrentScope = E_VARIABLESCOPE::LOCAL;
		}
		else
		{
			eCurrentScope = E_VARIABLESCOPE::MEMBER;
		}
	}
	else
	{
		if (m_bFunctionInProcess)
		{
			eCurrentScope = E_VARIABLESCOPE::LOCAL;
		}
		else
		{
			eCurrentScope = E_VARIABLESCOPE::STATIC;
		}
	}

	return eCurrentScope;
}

std::string TinyCReader::getCurrentScopeString()
{
	return toString(getCurrentScope());
}

Tree* TinyCReader::getStructNodeByName(std::string sStructName)
{
	for (Tree* pStruct : m_vStructs)
	{
		if (GET_INFO_FOR_KEY(pStruct, "text") == sStructName)
		{
			return pStruct;
		}
	}

	assert(false);
}

void TinyCReader::handleFunctionCallInExpr()
{
	// The idea here is to create a temporary variable of the type returned by the function
	// & add it before the expression statement.
	// The temporary variable is then inserted in the expression.
	// Eg:
	// 		int32_t iRet = 10;
	//		iRet = 10 + retFunc(); // where retFunc return type is "int32_t".
	//		This will create a dummy code as follows:
	//			int32_t iRet = 10;
	//			int32_t iRet_retFunc = retFunc();	// This line of code will be inserted by the following piece of code.
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
		char sUniqueFuncName[255] = {};
		sprintf_s(sUniqueFuncName, "%u", pFunctionCallNode);

		std::string sFuncName = GET_INFO_FOR_KEY(pFunctionCallNode, "text");
		std::string sFullyQualifiedTempVariableName;
		sFullyQualifiedTempVariableName.append(GET_INFO_FOR_KEY(pAssignNode, "text"));
		sFullyQualifiedTempVariableName.append("_");
		sFullyQualifiedTempVariableName.append(sFuncName);
		sFullyQualifiedTempVariableName.append("_");
		sFullyQualifiedTempVariableName.append(sUniqueFuncName);

		Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedTempVariableName.c_str());
		{
			GrammerUtils::m_vUserDefinedVariables.push_back(sFullyQualifiedTempVariableName);
			GrammerUtils::m_vUserDefinedVariables.push_back(sFuncName);

			pPrimIntNode->m_sAdditionalInfo.append(sFullyQualifiedTempVariableName);
			SET_INFO_FOR_KEY(pPrimIntNode, "givenName", sFuncName);
			SET_INFO_FOR_KEY(pPrimIntNode, "type", "int32_t");
			SET_INFO_FOR_KEY(pPrimIntNode, "scope", getCurrentScopeString());
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
}


bool TinyCReader::def() {

	addType("int8_t");
	addType("int16_t");
	addType("int32_t");
	addType("float");

	while (true) {
		if (objectList()) {
		}
		else
			break;
	}

	return true;

}

bool TinyCReader::objectList() {
	if (interfaceDeclaration()) {
		return true;
	}
	else
		if (structDeclaration()) {
			return true;
		}
		else
			if (staticDeclaration()) {
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

bool TinyCReader::interfaceDeclaration() {
	if (!GrammerUtils::match("interface", MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	__START_INTERFACE__
		std::string sInterfaceName = PREV_TOKEN_TEXT;
	__START_BLOCK_STRING__(sInterfaceName)

		Tree* pInterfaceDefNode = makeLeaf(ASTNodeType::ASTNode_INTERFACEDEF, sInterfaceName.c_str());
	Tree* pInterfaceExtendsListNode = makeLeaf(ASTNodeType::ASTNode_INTERFACEEXTENDLIST, "");
	{
		pInterfaceDefNode->m_pRightNode = pInterfaceExtendsListNode;

		m_pASTCurrentNode->addChild(pInterfaceDefNode);
		addInterfaceType(sInterfaceName);
	}

	std::string sStructParentList = "";

	if (!GrammerUtils::match(':', OPTIONAL_)) {

	}

	else {

		if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
			return false;

		pInterfaceExtendsListNode->addChild(makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT));

		while (true) {
			if (GrammerUtils::match(',', OPTIONAL_)) {

				if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
					return false;

				pInterfaceExtendsListNode->addChild(makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT));

			}
			else
				break;
		}


		SET_INFO_FOR_KEY(pInterfaceDefNode, "extends", sStructParentList);

	}

	if (!GrammerUtils::match('{', MANDATORY_))
		return false;

	Tree* pTemp = nullptr;
	pTemp = m_pASTCurrentNode; // Save Root Node temporarily
	m_pASTCurrentNode = pInterfaceDefNode;

	Tree* pInterfaceStartNode = makeLeaf(ASTNodeType::ASTNode_INTERFACESTART, "");
	m_pASTCurrentNode->addChild(pInterfaceStartNode);

	while (true) {
		if (interfaceObjectList()) {
			if (!GrammerUtils::match(';', OPTIONAL_)) {

			}

			else {

			}

		}
		else
			break;
	}

	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	__END_INTERFACE__
		__END_CURRENT_BLOCK__

		Tree* pInterfaceEndNode = makeLeaf(ASTNodeType::ASTNode_INTERFACEEND, "");
	m_pASTCurrentNode->addChild(pInterfaceEndNode);
	m_vInterfaces.push_back(pInterfaceDefNode);

	m_pASTCurrentNode = pTemp;

	if (!GrammerUtils::match(';', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::interfaceObjectList() {
	if (!functionDeclaration())
		return false;
	return true;

}

bool TinyCReader::functionDeclaration() {
	if (!GrammerUtils::match("inline", MANDATORY_))
		return false;

	std::string sPrevText = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sReturnType = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match(TokenType_::Type::TK_FUNCTIONCALL, MANDATORY_))
		return false;

	std::string sFunctionName = PREV_TOKEN_TEXT;

	Tree* pFunctionDefNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONDEF, sFunctionName.c_str());
	Tree* pReturnTypeNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONRETURNTYPE, sReturnType.c_str());
	Tree* pArgListNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONARGLIST, "");
	{
		pReturnTypeNode->m_pParentNode = pFunctionDefNode;
		pArgListNode->m_pParentNode = pFunctionDefNode;

		m_pASTCurrentNode->addChild(pFunctionDefNode);

		pFunctionDefNode->m_pLeftNode = pReturnTypeNode;
		pFunctionDefNode->m_pRightNode = pArgListNode;

		if (sPrevText == "virtual")
		{
			SET_INFO_FOR_KEY(pFunctionDefNode, "isVirtual", "virtual");
		}
	}

	if (!GrammerUtils::match('(', MANDATORY_))
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

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	SET_INFO_FOR_KEY(pFunctionDefNode, "isPure", "pure");

	Tree* pFuncEndNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONEND, "");
	m_pASTCurrentNode->addChild(pFuncEndNode);

	return true;

}

bool TinyCReader::structDeclaration() {
	if (!GrammerUtils::match("struct", MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	__START_STRUCT__
		std::string sStructName = PREV_TOKEN_TEXT;
	__START_BLOCK_STRING__(sStructName)

		Tree* pStructDefNode = makeLeaf(ASTNodeType::ASTNode_STRUCTDEF, sStructName.c_str());
	Tree* pStructImplementsListNode = makeLeaf(ASTNodeType::ASTNode_STRUCTIMPLEMENTLIST, "");
	{
		pStructDefNode->m_pRightNode = pStructImplementsListNode;

		m_pASTCurrentNode->addChild(pStructDefNode);
		addStructType(sStructName);
	}

	if (!GrammerUtils::match(':', OPTIONAL_)) {

	}

	else {

		if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
			return false;

		std::string sStructParentName = PREV_TOKEN_TEXT;
		SET_INFO_FOR_KEY(pStructDefNode, "extends", sStructParentName);

	}

	if (!GrammerUtils::match("implements", OPTIONAL_)) {

	}

	else {

		if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
			return false;

		pStructImplementsListNode->addChild(makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT));

		while (true) {
			if (GrammerUtils::match(',', OPTIONAL_)) {

				if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
					return false;

				pStructImplementsListNode->addChild(makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT));

			}
			else
				break;
		}

	}

	if (!GrammerUtils::match('{', MANDATORY_))
		return false;

	Tree* pTemp = nullptr;
	pTemp = m_pASTCurrentNode; // Save Root Node temporarily
	m_pASTCurrentNode = pStructDefNode;

	Tree* pStructStartNode = makeLeaf(ASTNodeType::ASTNode_STRUCTSTART, "");
	m_pASTCurrentNode->addChild(pStructStartNode);

	while (true) {
		if (structObjectList()) {
			if (!GrammerUtils::match(';', OPTIONAL_)) {

			}

			else {

			}

		}
		else
			break;
	}

	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	__END_STRUCT__
		__END_CURRENT_BLOCK__

		Tree* pStructEndNode = makeLeaf(ASTNodeType::ASTNode_STRUCTEND, "");
	m_pASTCurrentNode->addChild(pStructEndNode);
	m_vStructs.push_back(pStructDefNode);

	m_pASTCurrentNode = pTemp;

	if (!GrammerUtils::match(';', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::structObjectList() {
	if (structInlineFunction()) {
		return true;
	}
	else
		if (newPtrOrArrayOrInt()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::structInlineFunction() {

	std::string sOverrideOrImplementor = "";

	if (!GrammerUtils::match(TokenType_::Type::TK_STRING, OPTIONAL_)) {

	}

	else {


		// "@Override" or "@Implementor" identifier
		sOverrideOrImplementor = PREV_TOKEN_TEXT;

	}

	if (!GrammerUtils::match("inline", MANDATORY_))
		return false;
	if (!GrammerUtils::match("virtual", OPTIONAL_)) {

	}

	else {

	}

	if (!GrammerUtils::match("static", OPTIONAL_)) {

	}

	else {

	}

	if (!functionDef())
		return false;

	Tree* pFunctionDefNode = m_pASTCurrentNode->getLastStatement();
	assert(pFunctionDefNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONDEF);
	if (pFunctionDefNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONDEF
		&&
		NOT sOverrideOrImplementor.empty()
		)
	{
		SET_INFO_FOR_KEY(pFunctionDefNode, "@Override | @Implementor", sOverrideOrImplementor);
	}

	return true;

}

bool TinyCReader::staticDeclaration() {
	if (!GrammerUtils::match("static", MANDATORY_))
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

bool TinyCReader::structType() {

	std::string sType = GrammerUtils::m_pToken.getText();
	return isValidStructType(sType);

	return true;

}

bool TinyCReader::staticPtr() {
	if (!primitiveType())
		return false;

	std::string sPointerType = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match('*', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	Tree* pStaticPtrNode = makeLeaf(ASTNodeType::ASTNode_TYPESTATIC, PREV_TOKEN_TEXT);
	{
		GrammerUtils::m_vUserDefinedVariables.push_back(PREV_TOKEN_TEXT);

		pStaticPtrNode->m_bIsPointerType = true;
		SET_INFO_FOR_KEY(pStaticPtrNode, "givenName", PREV_TOKEN_TEXT);
		SET_INFO_FOR_KEY(pStaticPtrNode, "type", sPointerType);
		SET_INFO_FOR_KEY(pStaticPtrNode, "scope", toString(E_VARIABLESCOPE::STATIC));

		m_pASTCurrentNode->addChild(pStaticPtrNode);
		FunctionInfo::addStaticVariable(pStaticPtrNode);
	}

	if (!GrammerUtils::match(';', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::functionDef() {

	std::string sPrevText = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sReturnType = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match(TokenType_::Type::TK_FUNCTIONCALL, MANDATORY_))
		return false;

	__START_FUNCTION__
		std::string sFunctionName = PREV_TOKEN_TEXT;
	__START_BLOCK_STRING__(sFunctionName)

		Tree* pFunctionDefNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONDEF, sFunctionName.c_str());
	Tree* pReturnTypeNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONRETURNTYPE, sReturnType.c_str());
	Tree* pArgListNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONARGLIST, "");
	{
		pReturnTypeNode->m_pParentNode = pFunctionDefNode;
		pArgListNode->m_pParentNode = pFunctionDefNode;

		m_pASTCurrentNode->addChild(pFunctionDefNode);

		pFunctionDefNode->m_pLeftNode = pReturnTypeNode;
		pFunctionDefNode->m_pRightNode = pArgListNode;

		if (sPrevText == "virtual")
		{
			SET_INFO_FOR_KEY(pFunctionDefNode, "isVirtual", "virtual");
		}

		if (sPrevText == "static")
		{
			SET_INFO_FOR_KEY(pFunctionDefNode, "isStatic", "static");
		}
	}

	if (!GrammerUtils::match('(', MANDATORY_))
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

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', MANDATORY_))
		return false;

	pTemp = m_pASTCurrentNode; // Save Root Node temporarily
	m_pASTCurrentNode = pFunctionDefNode;

	Tree* pFuncStartNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONSTART, "");
	m_pASTCurrentNode->addChild(pFuncStartNode);

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	__END_FUNCTION__
		__END_CURRENT_BLOCK__

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
	if (!argumentType())
		return false;
	if (!GrammerUtils::match(',', OPTIONAL_)) {

	}

	else {

	}

	return true;

}

bool TinyCReader::argumentType() {
	if (!variableType())
		return false;

	std::string sType = PREV_TOKEN_TEXT;
	bool bIsAPointer = false;

	if (!GrammerUtils::match('*', OPTIONAL_)) {

	}

	else {


		bIsAPointer = true;

	}

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sArgName = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedArgName;
	sFullyQualifiedArgName.append(getBlockString());
	sFullyQualifiedArgName.append(sArgName);

	Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedArgName.c_str());
	{
		pPrimIntNode->m_sAdditionalInfo.append(sArgName);
		SET_INFO_FOR_KEY(pPrimIntNode, "givenName", sArgName);
		SET_INFO_FOR_KEY(pPrimIntNode, "type", sType);
		SET_INFO_FOR_KEY(pPrimIntNode, "scope", toString(E_VARIABLESCOPE::ARGUMENT));
		pPrimIntNode->m_bIsPointerType = bIsAPointer;

		m_pASTCurrentNode->addChild(pPrimIntNode);
		m_pASTCurrentNode->m_sAdditionalInfo.append("int32_t_");
	}

	return true;

}

bool TinyCReader::variableType() {
	if (primitiveType()) {
		return true;
	}
	else
		if (structType()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::stmt_list() {
	while (true) {
		if (stmt()) {
			if (!GrammerUtils::match(';', OPTIONAL_)) {

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
	if (systemFunctionCall()) {
		return true;
	}
	else
		if (functionCall()) {
			return true;
		}
		else
			if (structMemberVariableAssignmentOrFunctionCall()) {
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
							if (newStructPtr()) {
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
															if (memSet()) {
																return true;
															}
															else
																if (memCpy()) {
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
	if (GrammerUtils::match(TokenType_::Type::TK_PREFIXDECR, OPTIONAL_)) {

		std::string sVariableName = PREV_TOKEN_TEXT;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREDECR, sFullyQualifiedVariableName.c_str());
		{
			m_pASTCurrentNode->addChild(pPreDecrNode);
			SET_INFO_FOR_KEY(pPreDecrNode, "givenName", sVariableName);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_PREFIXINCR, OPTIONAL_)) {

			std::string sVariableName = PREV_TOKEN_TEXT;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREINCR, sFullyQualifiedVariableName.c_str());
			{
				m_pASTCurrentNode->addChild(pPreDecrNode);
				SET_INFO_FOR_KEY(pPreDecrNode, "givenName", sVariableName);
			}

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::postFixIncrDecr() {
	if (GrammerUtils::match(TokenType_::Type::TK_POSTFIXDECR, OPTIONAL_)) {

		std::string sVariableName = PREV_TOKEN_TEXT;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTDECR, sFullyQualifiedVariableName.c_str());
		{
			m_pASTCurrentNode->addChild(pPreDecrNode);
			SET_INFO_FOR_KEY(pPreDecrNode, "givenName", sVariableName);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_POSTFIXINCR, OPTIONAL_)) {

			std::string sVariableName = PREV_TOKEN_TEXT;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTINCR, sFullyQualifiedVariableName.c_str());
			{
				m_pASTCurrentNode->addChild(pPreDecrNode);
				SET_INFO_FOR_KEY(pPreDecrNode, "givenName", sVariableName);
			}

			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::returnStatement() {
	if (!GrammerUtils::match("return", MANDATORY_))
		return false;

	__END_CURRENT_BLOCK__					// Sort of a hack, as this will be the last statement of the function,
											// all the array pointers need to be freed before 'return expr;'.

		__START_BLOCK_STRING__("return")

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

	__END_CURRENT_BLOCK__

		m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);
	pReturnStmtNode->addChild(pExpressionLeftLeaf);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pReturnStmtNode);

	return true;

}

bool TinyCReader::systemFunctionCall() {
	if (!GrammerUtils::match(TokenType_::Type::TK_SYSTEMFUNCTIONCALL, MANDATORY_))
		return false;

	std::string sIdentifier = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	Tree* pSystemFunctionCallNode = makeLeaf(ASTNodeType::ASTNode_SYSTEMFUNCTIONCALL, sIdentifier.c_str());
	Tree* pTemp = nullptr;
	{
		m_pASTCurrentNode->addChild(pSystemFunctionCallNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSystemFunctionCallNode;
	}

	if (!functionArgumentList()) {
	}
	else {
	}

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	Tree* pSystemFuncCallEndNode = makeLeaf(ASTNodeType::ASTNode_SYSTEMFUNCTIONCALLEND, sIdentifier.c_str());
	m_pASTCurrentNode->addChild(pSystemFuncCallEndNode);

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::functionCall() {
	if (!GrammerUtils::match(TokenType_::Type::TK_FUNCTIONCALL, MANDATORY_))
		return false;

	std::string sIdentifier = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match('(', MANDATORY_))
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

	if (!GrammerUtils::match(')', MANDATORY_))
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
	if (GrammerUtils::match(TokenType_::Type::TK_STRING, OPTIONAL_)) {

		Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT);
		{
			m_pASTCurrentNode->addChild(pStringNode);
			m_pASTCurrentNode->m_sAdditionalInfo.append("string_");
		}

		if (!GrammerUtils::match(',', OPTIONAL_)) {

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

				m_pASTCurrentNode->m_sAdditionalInfo.append("int32_t_");
			}

			if (!GrammerUtils::match(',', OPTIONAL_)) {

			}

			else {

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
	if (!GrammerUtils::match("if", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	__START_BLOCK_STRING__("if")

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

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL_)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL_)) {

	}

	else {

	}


	__END_CURRENT_BLOCK__

		if (!elseStatement()) {
		}
		else {
		}


		m_pASTCurrentNode = pTemp;
		m_pASTCurrentNode->addChild(pIfNode);

		return true;

}

bool TinyCReader::elseStatement() {
	if (!GrammerUtils::match("else", MANDATORY_))
		return false;

	__START_BLOCK_STRING__("else")

		Tree* pElseNode = makeLeaf(ASTNodeType::ASTNode_ELSE, PREV_TOKEN_TEXT);
	Tree* pIfNode = nullptr;
	{
		pIfNode = m_pASTCurrentNode;
		pElseNode->m_pParentNode = pIfNode->m_pParentNode;

		m_pASTCurrentNode = pElseNode;
	}

	if (!GrammerUtils::match('{', OPTIONAL_)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL_)) {

	}

	else {

	}


	__END_CURRENT_BLOCK__

		pIfNode->m_pRightNode = pElseNode;
	{
		m_pASTCurrentNode = pIfNode;
	}

	return true;

}

bool TinyCReader::whileStatement() {
	if (!GrammerUtils::match("while", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	__START_BLOCK_STRING__("while")

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

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL_)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL_)) {

	}

	else {

	}


	__END_CURRENT_BLOCK__

		m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pWhileNode);

	return true;

}

bool TinyCReader::switchStatement() {
	if (!GrammerUtils::match("switch", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	__START_BLOCK_STRING__("switch")

		Tree* pTemp = nullptr;
	Tree* pSwitchNode = makeLeaf(ASTNodeType::ASTNode_SWITCH, "switch");
	{
		pSwitchNode->m_pParentNode = m_pASTCurrentNode;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSwitchNode;
	}

	if (!switchArgument())
		return false;
	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', MANDATORY_))
		return false;
	if (!oneOrMoreCasesOrDefault())
		return false;
	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	__END_CURRENT_BLOCK__

		m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pSwitchNode);

	return true;

}

bool TinyCReader::switchArgument() {
	if (GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, OPTIONAL_)) {

		std::string sIdentifier = PREV_TOKEN_TEXT;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
		assert(!sFullyQualifiedVariableName.empty());

		Tree* pSwitchArgumentNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
		m_pASTCurrentNode->m_pLeftNode = pSwitchArgumentNode;
		SET_INFO_FOR_KEY(pSwitchArgumentNode, "givenName", PREV_TOKEN_TEXT);

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {

			Tree* pSwitchArgumentNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, PREV_TOKEN_TEXT);
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
	if (!GrammerUtils::match("case", MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_INTEGER, MANDATORY_))
		return false;

	__START_BLOCK_STRING__("switchcase")

		Tree* pTemp = nullptr;
	Tree* pSwitchCaseNode = makeLeaf(ASTNodeType::ASTNode_SWITCHCASE, PREV_TOKEN_TEXT);
	m_pASTCurrentNode->addChild(pSwitchCaseNode);
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSwitchCaseNode;
	}

	if (!GrammerUtils::match(':', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL_)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL_)) {

	}

	else {

	}

	if (!GrammerUtils::match("break", OPTIONAL_)) {

	}

	else {


		Tree* pSwitchBreakNode = makeLeaf(ASTNodeType::ASTNode_SWITCHBREAK, "break");
		m_pASTCurrentNode->addChild(pSwitchBreakNode);

		if (!GrammerUtils::match(';', MANDATORY_))
			return false;
	}


	__END_CURRENT_BLOCK__
		m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::defaultCase() {
	if (!GrammerUtils::match("default", MANDATORY_))
		return false;

	__START_BLOCK_STRING__("switchcase")

		Tree* pTemp = nullptr;
	Tree* pSwitchDefaultNode = makeLeaf(ASTNodeType::ASTNode_SWITCHDEFAULT, PREV_TOKEN_TEXT);
	m_pASTCurrentNode->addChild(pSwitchDefaultNode);
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSwitchDefaultNode;
	}

	if (!GrammerUtils::match(':', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', OPTIONAL_)) {

	}

	else {

	}

	if (!stmt_list())
		return false;
	if (!GrammerUtils::match('}', OPTIONAL_)) {

	}

	else {

	}


	__END_CURRENT_BLOCK__
		m_pASTCurrentNode = pTemp;

	if (!GrammerUtils::match("break", MANDATORY_))
		return false;
	if (!GrammerUtils::match(';', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::forStatement() {
	if (!GrammerUtils::match("for", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	__START_BLOCK_STRING__("for")

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

		if (!GrammerUtils::match(';', MANDATORY_))
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

		if (!GrammerUtils::match(';', MANDATORY_))
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

		if (!GrammerUtils::match(')', MANDATORY_))
			return false;

		m_pASTCurrentNode = pWhileNode;

		if (!GrammerUtils::match('{', OPTIONAL_)) {

		}

		else {

		}

		if (!stmt_list())
			return false;
		if (!GrammerUtils::match('}', OPTIONAL_)) {

		}

		else {

		}


		__END_CURRENT_BLOCK__

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
		if (GrammerUtils::match(',', OPTIONAL_)) {

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
		if (GrammerUtils::match(',', OPTIONAL_)) {

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
	if (!GrammerUtils::match("print", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	Tree* pPrintNode = makeLeaf(ASTNodeType::ASTNode_PRINT, PREV_TOKEN_TEXT);
	Tree* pTemp = nullptr;
	{
		m_pASTCurrentNode->addChild(pPrintNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pPrintNode;
	}

	if (!print_list())
		return false;
	if (!GrammerUtils::match(')', MANDATORY_))
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
	if (GrammerUtils::match(TokenType_::Type::TK_STRING, OPTIONAL_)) {

		Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT);
		{
			m_pASTCurrentNode->addChild(pStringNode);
		}

		if (!GrammerUtils::match(',', OPTIONAL_)) {

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

			if (!GrammerUtils::match(',', OPTIONAL_)) {

			}

			else {

			}

			return true;
		}
		else
			if (GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {

				Tree* pCharacterNode = makeLeaf(ASTNodeType::ASTNode_CHARACTER, PREV_TOKEN_TEXT);
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
	if (!GrammerUtils::match("putc", MANDATORY_))
		return false;

	Tree* pPutCNode = makeLeaf(ASTNodeType::ASTNode_PUTC, PREV_TOKEN_TEXT);
	Tree* pTemp = nullptr;
	{
		m_pASTCurrentNode->addChild(pPutCNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pPutCNode;
	}

	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!putcList())
		return false;
	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::memSet() {
	if (!GrammerUtils::match("memSet", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	Tree* pAST_MemSet = makeLeaf(ASTNodeType::ASTNode_MEMSET, "");
	Tree* pTemp;

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sIdentifier = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
	assert(!sFullyQualifiedVariableName.empty());

	SET_INFO_FOR_KEY(pAST_MemSet, "src", sFullyQualifiedVariableName);

	Tree* pExpressionMemSetValueLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pExpressionMemSetValueLeaf;
	}

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pExpressionMemSetValueLeaf = createPostFixExpr(m_pASTCurrentNode);
	pAST_MemSet->m_pLeftNode = pExpressionMemSetValueLeaf;

	Tree* pExpressionMemSetSizeLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		m_pASTCurrentNode = pExpressionMemSetSizeLeaf;
	}

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pExpressionMemSetSizeLeaf = createPostFixExpr(m_pASTCurrentNode);
	pAST_MemSet->m_pRightNode = pExpressionMemSetSizeLeaf;

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAST_MemSet);

	return true;

}

bool TinyCReader::memCpy() {
	if (!GrammerUtils::match("memCpy", MANDATORY_))
		return false;

	Tree* pAST_MemCpy = makeLeaf(ASTNodeType::ASTNode_MEMCPY, "");

	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sIdentifier = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
	assert(!sFullyQualifiedVariableName.empty());

	SET_INFO_FOR_KEY(pAST_MemCpy, "src", sFullyQualifiedVariableName);

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	sIdentifier = PREV_TOKEN_TEXT;
	sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
	assert(!sFullyQualifiedVariableName.empty());

	SET_INFO_FOR_KEY(pAST_MemCpy, "dst", sFullyQualifiedVariableName);

	Tree* pSizeExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	Tree* pTemp;
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSizeExpressionLeaf;
	}

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pSizeExpressionLeaf = createPostFixExpr(m_pASTCurrentNode);
	pAST_MemCpy->m_pRightNode = pSizeExpressionLeaf;

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAST_MemCpy);

	return true;

}

bool TinyCReader::memCmp() {
	if (!GrammerUtils::match("memCmp", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	Tree* pAST_MemCmp = makeLeaf(ASTNodeType::ASTNode_MEMCMP, "");

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sIdentifier = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
	assert(!sFullyQualifiedVariableName.empty());

	SET_INFO_FOR_KEY(pAST_MemCmp, "src", sFullyQualifiedVariableName);

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	sIdentifier = PREV_TOKEN_TEXT;
	sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
	assert(!sFullyQualifiedVariableName.empty());

	SET_INFO_FOR_KEY(pAST_MemCmp, "dst", sFullyQualifiedVariableName);

	Tree* pSizeExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	Tree* pTemp;
	{
		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pSizeExpressionLeaf;
	}

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pSizeExpressionLeaf = createPostFixExpr(m_pASTCurrentNode);
	pAST_MemCmp->m_pRightNode = pSizeExpressionLeaf;

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAST_MemCmp);

	return true;

}

bool TinyCReader::memChr() {
	if (!GrammerUtils::match("memChr", MANDATORY_))
		return false;

	Tree* pAST_MemChr = makeLeaf(ASTNodeType::ASTNode_MEMCHR, "");

	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sIdentifier = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sIdentifier.c_str());
	assert(!sFullyQualifiedVariableName.empty());

	SET_INFO_FOR_KEY(pAST_MemChr, "src", sFullyQualifiedVariableName);

	Tree* pExpressionMemValueLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	Tree* pTemp;
	{
		pAST_MemChr->m_pLeftNode = pExpressionMemValueLeaf;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pExpressionMemValueLeaf;
	}

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pExpressionMemValueLeaf = createPostFixExpr(m_pASTCurrentNode);

	Tree* pExpressionMemSizeLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pAST_MemChr->m_pRightNode = pExpressionMemSizeLeaf;
		m_pASTCurrentNode = pExpressionMemSizeLeaf;
	}

	if (!GrammerUtils::match(',', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pExpressionMemSizeLeaf = createPostFixExpr(m_pASTCurrentNode);

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAST_MemChr);

	return true;

}

bool TinyCReader::sizeOf() {
	if (!GrammerUtils::match("sizeOf", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	Tree* pSizeOfNode = makeLeaf(ASTNodeType::ASTNode_SIZEOF, "sizeof");
	{
		m_pASTCurrentNode->addChild(pSizeOfNode);
	}

	if (!GrammerUtils::match(TokenType_::Type::TK_STRING, MANDATORY_))
		return false;

	Tree* pStringNode = makeLeaf(ASTNodeType::ASTNode_STRING, PREV_TOKEN_TEXT);
	pSizeOfNode->m_pLeftNode = pStringNode;

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::putcList() {
	if (GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, OPTIONAL_)) {

		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, PREV_TOKEN_TEXT);
		assert(!sFullyQualifiedVariableName.empty());

		Tree* pIdentifierNode = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
		m_pASTCurrentNode->addChild(pIdentifierNode);
		SET_INFO_FOR_KEY(pIdentifierNode, "givenName", PREV_TOKEN_TEXT);

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {

			Tree* pIntegerNode = makeLeaf(ASTNodeType::ASTNode_INTEGER, PREV_TOKEN_TEXT);
			m_pASTCurrentNode->addChild(pIntegerNode);

			return true;
		}
		else
			if (GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {

				Tree* pCharacterNode = makeLeaf(ASTNodeType::ASTNode_CHARACTER, PREV_TOKEN_TEXT);
				m_pASTCurrentNode->addChild(pCharacterNode);

				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::bracesstmtlist() {
	if (!GrammerUtils::match('{', MANDATORY_))
		return false;

	__START_BLOCK_STRING__("{")

		if (!stmt_list())
			return false;
	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	__END_CURRENT_BLOCK__

		return true;

}

bool TinyCReader::newStructPtr() {
	if (!structType())
		return false;

	std::string sStructType = PREV_TOKEN_TEXT;
	bool bIsPointerType = false;

	if (!GrammerUtils::match('*', OPTIONAL_)) {

	}

	else {


		bIsPointerType = true;

	}

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sVariableName = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	Tree* pStructPtrNode = makeLeaf(ASTNodeType::ASTNode_TYPESTRUCT, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		GrammerUtils::m_vUserDefinedVariables.push_back(sFullyQualifiedVariableName);
		GrammerUtils::m_vUserDefinedVariables.push_back(sVariableName);

		pStructPtrNode->m_sAdditionalInfo.append(sVariableName);
		pStructPtrNode->m_bIsPointerType = true;
		SET_INFO_FOR_KEY(pStructPtrNode, "givenName", sVariableName);
		SET_INFO_FOR_KEY(pStructPtrNode, "type", sStructType);
		SET_INFO_FOR_KEY(pStructPtrNode, "scope", getCurrentScopeString());
		pStructPtrNode->m_pParentNode = m_pASTCurrentNode;

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pStructPtrNode;
	}

	if (!GrammerUtils::match('=', OPTIONAL_)) {

	}

	else {

		if (!GrammerUtils::match("new", MANDATORY_))
			return false;
		if (!functionCall())
			return false;
	}


	// Add DUMMY Constructor call if "ASTNode_FUNCTIONCALL" not found !
	Tree* pFunctionCallNode = hasNodeOfType(m_pASTCurrentNode, ASTNodeType::ASTNode_FUNCTIONCALL);
	if (pFunctionCallNode == nullptr)
	{
		pFunctionCallNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONCALL, sStructType.c_str());
		Tree* pFuncCallEndNode = makeLeaf(ASTNodeType::ASTNode_FUNCTIONCALLEND, sStructType.c_str());

		pFunctionCallNode->addChild(pFuncCallEndNode);
		m_pASTCurrentNode->addChild(pFunctionCallNode);

		if (NOT bIsPointerType && m_pASTCurrentNode->m_eASTNodeType != ASTNodeType::ASTNode_STRUCTDEF)
			pushLocalHeapVar(sFullyQualifiedVariableName);
	}

	SET_INFO_FOR_KEY(pFunctionCallNode, "memberFunctionOf", sStructType);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pStructPtrNode);

	return true;

}

bool TinyCReader::userDefinedConstructorCall() {
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!functionArgumentList()) {
	}
	else {
	}

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
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

	std::string sPointerType = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match('*', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sVariableName = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	Tree* pPrimPtrNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		pTemp = m_pASTCurrentNode;

		GrammerUtils::m_vUserDefinedVariables.push_back(sFullyQualifiedVariableName);
		GrammerUtils::m_vUserDefinedVariables.push_back(sVariableName);

		pPrimPtrNode->m_sAdditionalInfo.append(sVariableName);
		pPrimPtrNode->m_bIsPointerType = true;
		SET_INFO_FOR_KEY(pPrimPtrNode, "givenName", sVariableName);
		SET_INFO_FOR_KEY(pPrimPtrNode, "type", sPointerType);
		SET_INFO_FOR_KEY(pPrimPtrNode, "scope", getCurrentScopeString());
		pPrimPtrNode->m_pParentNode = m_pASTCurrentNode;

		m_pASTCurrentNode = pPrimPtrNode;
	}

	if (!primPtrOptionalRHS()) {
	}
	else {
	}


	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pPrimPtrNode);

	return true;

}

bool TinyCReader::primPtrOptionalRHS() {
	if (!GrammerUtils::match('=', MANDATORY_))
		return false;
	if (!ptrAssign())
		return false;
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

	std::string sPrimitiveType = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match(TokenType_::Type::TK_DEREFARRAY, MANDATORY_))
		return false;

	std::string sVariableName = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	if (!GrammerUtils::match('[', MANDATORY_))
		return false;

	if (m_pASTCurrentNode->m_eASTNodeType != ASTNodeType::ASTNode_STRUCTDEF)
		pushLocalHeapVar(sFullyQualifiedVariableName);

	Tree* pPrimTypeArrayNode = makeLeaf(ASTNodeType::ASTNode_TYPEARRAY, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		pTemp = m_pASTCurrentNode;

		GrammerUtils::m_vUserDefinedVariables.push_back(sFullyQualifiedVariableName);
		GrammerUtils::m_vUserDefinedVariables.push_back(sVariableName);

		pPrimTypeArrayNode->m_sAdditionalInfo.append(sVariableName);
		pPrimTypeArrayNode->m_bIsPointerType = true;
		SET_INFO_FOR_KEY(pPrimTypeArrayNode, "givenName", sVariableName);
		SET_INFO_FOR_KEY(pPrimTypeArrayNode, "type", sPrimitiveType);
		SET_INFO_FOR_KEY(pPrimTypeArrayNode, "scope", getCurrentScopeString());
		pPrimTypeArrayNode->m_pParentNode = m_pASTCurrentNode;
	}

	if (!GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {

	}

	else {


		Tree* pArraySizeLeaf = makeLeaf(ASTNodeType::ASTNode_INTEGER, PREV_TOKEN_TEXT);
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
	if (!GrammerUtils::match(']', MANDATORY_))
		return false;
	if (!GrammerUtils::match('=', MANDATORY_))
		return false;
	if (!GrammerUtils::match('{', MANDATORY_))
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

			if (!GrammerUtils::match(',', OPTIONAL_)) {

			}

			else {

			}

		}
		else
			break;
	}

	if (!GrammerUtils::match('}', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::primType() {

	std::string sPrimitiveType = PREV_TOKEN_TEXT;

	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sVariableName = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName;
	sFullyQualifiedVariableName.append(getBlockString());
	sFullyQualifiedVariableName.append(sVariableName);

	Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedVariableName.c_str());
	Tree* pTemp = nullptr;
	{
		pTemp = m_pASTCurrentNode;

		GrammerUtils::m_vUserDefinedVariables.push_back(sFullyQualifiedVariableName);
		GrammerUtils::m_vUserDefinedVariables.push_back(sVariableName);

		pPrimIntNode->m_sAdditionalInfo.append(sVariableName);
		SET_INFO_FOR_KEY(pPrimIntNode, "givenName", sVariableName);
		SET_INFO_FOR_KEY(pPrimIntNode, "type", sPrimitiveType);
		SET_INFO_FOR_KEY(pPrimIntNode, "scope", getCurrentScopeString());

		pPrimIntNode->m_pParentNode = m_pASTCurrentNode;
		m_pASTCurrentNode = pPrimIntNode;
	}

	if (!primTypeOptionalRHS()) {
	}
	else {
	}


	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pPrimIntNode);

	return true;

}

bool TinyCReader::primTypeOptionalRHS() {
	if (!GrammerUtils::match('=', MANDATORY_))
		return false;

	Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		m_pASTCurrentNode->m_pLeftNode = pExpressionLeftLeaf;
		pExpressionLeftLeaf->m_pParentNode = m_pASTCurrentNode;

		m_pASTCurrentNode = pExpressionLeftLeaf;
	}

	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	return true;

}

bool TinyCReader::ptrAssign() {
	if (malloc()) {
		return true;
	}
	else
		if (memChr()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::malloc() {
	if (!GrammerUtils::match("malloc", MANDATORY_))
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


	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!expr())
		return false;

	m_pASTCurrentNode = createPostFixExpr(m_pASTCurrentNode);

	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pMallocNode);

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::assignmentDerefArray() {
	if (!GrammerUtils::match(TokenType_::Type::TK_DEREFARRAY, MANDATORY_))
		return false;

	TokenType_::Type eTokenType = GrammerUtils::m_pPrevToken.m_eTokenType;
	std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
	assert(!sFullyQualifiedVariableName.empty());

	Tree* pTemp = nullptr;
	Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sFullyQualifiedVariableName.c_str());
	{
		pAssignmentNode->m_pParentNode = m_pASTCurrentNode;
		SET_INFO_FOR_KEY(pAssignmentNode, "givenName", sVariableName);
	}

	Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_DEREFARRAY, sFullyQualifiedVariableName.c_str());
	Tree* pArrayIndexExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pIdentifierLeaf->m_sAdditionalInfo = sVariableName;
		SET_INFO_FOR_KEY(pIdentifierLeaf, "givenName", sVariableName);

		pAssignmentNode->m_pRightNode = pIdentifierLeaf;
		pIdentifierLeaf->m_pParentNode = pAssignmentNode;

		pTemp = m_pASTCurrentNode;

		pIdentifierLeaf->m_pLeftNode = pArrayIndexExpressionLeaf;
		pArrayIndexExpressionLeaf->m_pParentNode = pIdentifierLeaf;

		m_pASTCurrentNode = pArrayIndexExpressionLeaf;
	}

	if (!GrammerUtils::match('[', MANDATORY_))
		return false;
	if (!expr())
		return false;

	pArrayIndexExpressionLeaf = createPostFixExpr(pArrayIndexExpressionLeaf);
	pIdentifierLeaf->addChild(pArrayIndexExpressionLeaf);

	if (!GrammerUtils::match(']', MANDATORY_))
		return false;
	if (!GrammerUtils::match('=', MANDATORY_))
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

bool TinyCReader::structMemberVariableAssignmentOrFunctionCall() {
	if (structStaticMemberAccess()) {
		return true;
	}
	else
		if (structObjectMemberAccess()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::structStaticMemberAccess() {
	if (!GrammerUtils::match(TokenType_::Type::TK_STATICMEMBERACCESS, MANDATORY_))
		return false;

	SAVE_PREV_TOKEN

		if (!GrammerUtils::match(':', MANDATORY_))
			return false;
	if (!GrammerUtils::match(':', MANDATORY_))
		return false;
	if (!structMemberVariableLValueOrFunctionCall())
		return false;
	return true;

}

bool TinyCReader::structObjectMemberAccess() {
	if (!GrammerUtils::match(TokenType_::Type::TK_MEMBERACCESS, MANDATORY_))
		return false;

	SAVE_PREV_TOKEN

		if (!GrammerUtils::match('-', MANDATORY_))
			return false;
	if (!GrammerUtils::match('>', MANDATORY_))
		return false;
	if (!structMemberVariableLValueOrFunctionCall())
		return false;
	return true;

}

bool TinyCReader::structMemberVariableLValueOrFunctionCall() {

	Token eSavedToken = SAVED_TOKEN;
	Tree* pTemp = nullptr;
	Tree* pAssignmentNode = nullptr;

	std::string sObjectName = SAVED_TOKEN_TEXT;

	switch (eSavedToken.m_eTokenType)
	{
	case TokenType_::Type::TK_MEMBERACCESS:
	{
		std::string sFullyQualifiedObjectName = "";
		if (sObjectName == "this")
			sFullyQualifiedObjectName = "this";
		else
			sFullyQualifiedObjectName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sObjectName.c_str());

		pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sFullyQualifiedObjectName.c_str());
		SET_INFO_FOR_KEY(pAssignmentNode, "accessType", "object");
	}
	break;
	case TokenType_::Type::TK_STATICMEMBERACCESS:
	{
		pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sObjectName.c_str());
		SET_INFO_FOR_KEY(pAssignmentNode, "accessType", "static");
	}
	break;
	}

	{
		pAssignmentNode->m_pParentNode = m_pASTCurrentNode;
		SET_INFO_FOR_KEY(pAssignmentNode, "givenName", sObjectName);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pAssignmentNode;
	}

	if (!structVariableLValueOrArrayLValueOrFunctionCall())
		return false;
	if (!GrammerUtils::match('=', OPTIONAL_)) {

	}

	else {


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

	}


	m_pASTCurrentNode = pTemp;
	m_pASTCurrentNode->addChild(pAssignmentNode);

	return true;

}

bool TinyCReader::structVariableLValueOrArrayLValueOrFunctionCall() {
	if (functionCall()) {

		m_pASTCurrentNode->m_eASTNodeType = ASTNodeType::ASTNode_MEMBERACCESS;

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, OPTIONAL_)) {

			Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_MEMBERACCESS, PREV_TOKEN_TEXT);
			{
				pIdentifierLeaf->m_sAdditionalInfo = PREV_TOKEN_TEXT;
				SET_INFO_FOR_KEY(pIdentifierLeaf, "givenName", PREV_TOKEN_TEXT);
				m_pASTCurrentNode->m_pRightNode = pIdentifierLeaf;
			}

			return true;
		}
		else
			if (structMemberVariableLValueArrayAccess()) {
				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::structMemberVariableLValueArrayAccess() {
	if (!GrammerUtils::match(TokenType_::Type::TK_DEREFARRAY, MANDATORY_))
		return false;

	Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_MEMBERACCESSDEREF, PREV_TOKEN_TEXT);
	Tree* pArrayIndexExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
	{
		pIdentifierLeaf->m_sAdditionalInfo = PREV_TOKEN_TEXT;
		SET_INFO_FOR_KEY(pIdentifierLeaf, "givenName", PREV_TOKEN_TEXT);
		m_pASTCurrentNode->m_pRightNode = pIdentifierLeaf;
	}

	if (!GrammerUtils::match('[', MANDATORY_))
		return false;

	checkOpPrecedenceAndPush("(");

	if (!expr())
		return false;

	checkOpPrecedenceAndPush(")");


	pArrayIndexExpressionLeaf = createPostFixExpr(pArrayIndexExpressionLeaf);
	pIdentifierLeaf->m_pLeftNode = pArrayIndexExpressionLeaf;

	if (!GrammerUtils::match(']', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::assignmentRHS() {
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sVariableName = PREV_TOKEN_TEXT;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
	assert(!sFullyQualifiedVariableName.empty());

	Tree* pAssignmentNode = makeLeaf(ASTNodeType::ASTNode_ASSIGN, sFullyQualifiedVariableName.c_str());
	{
		pAssignmentNode->m_pParentNode = m_pASTCurrentNode;
		SET_INFO_FOR_KEY(pAssignmentNode, "givenName", sVariableName);
	}

	Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, sFullyQualifiedVariableName.c_str());
	{
		pIdentifierLeaf->m_sAdditionalInfo = sVariableName;
		SET_INFO_FOR_KEY(pIdentifierLeaf, "givenName", sVariableName);

		pAssignmentNode->m_pRightNode = pIdentifierLeaf;
	}

	if (!GrammerUtils::match('=', MANDATORY_))
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
	if (!GrammerUtils::match("free", MANDATORY_))
		return false;
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
		return false;

	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, PREV_TOKEN_TEXT);
	assert(!sFullyQualifiedVariableName.empty());

	Tree* pFreePtrLeaf = makeLeaf(ASTNodeType::ASTNode_FREE, sFullyQualifiedVariableName.c_str());
	SET_INFO_FOR_KEY(pFreePtrLeaf, "givenName", PREV_TOKEN_TEXT);

	m_pASTCurrentNode->addChild(pFreePtrLeaf);

	if (!GrammerUtils::match(')', MANDATORY_))
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
	if (GrammerUtils::match("&&", OPTIONAL_)) {

		checkOpPrecedenceAndPush("&&");

		return true;
	}
	else
		if (GrammerUtils::match("||", OPTIONAL_)) {

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
	if (GrammerUtils::match("==", OPTIONAL_)) {

		checkOpPrecedenceAndPush("==");

		return true;
	}
	else
		if (GrammerUtils::match("!=", OPTIONAL_)) {

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
	if (GrammerUtils::match('<', OPTIONAL_)) {

		checkOpPrecedenceAndPush("<");

		return true;
	}
	else
		if (GrammerUtils::match("<=", OPTIONAL_)) {

			checkOpPrecedenceAndPush("<=");

			return true;
		}
		else
			if (GrammerUtils::match('>', OPTIONAL_)) {

				checkOpPrecedenceAndPush(">");

				return true;
			}
			else
				if (GrammerUtils::match(">=", OPTIONAL_)) {

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
	if (GrammerUtils::match('+', OPTIONAL_)) {

		checkOpPrecedenceAndPush("+");

		return true;
	}
	else
		if (GrammerUtils::match('-', OPTIONAL_)) {

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
	if (GrammerUtils::match('*', OPTIONAL_)) {

		checkOpPrecedenceAndPush("*");

		return true;
	}
	else
		if (GrammerUtils::match('/', OPTIONAL_)) {

			checkOpPrecedenceAndPush("/");

			return true;
		}
		else
			if (GrammerUtils::match('%', OPTIONAL_)) {

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
	if (GrammerUtils::match('&', OPTIONAL_)) {

		checkOpPrecedenceAndPush("&");

		return true;
	}
	else
		if (GrammerUtils::match('|', OPTIONAL_)) {

			checkOpPrecedenceAndPush("|");

			return true;
		}
		else
			if (GrammerUtils::match('^', OPTIONAL_)) {

				checkOpPrecedenceAndPush("^");

				return true;
			}
			else
				if (GrammerUtils::match("<<", OPTIONAL_)) {

					checkOpPrecedenceAndPush("<<");

					return true;
				}
				else
					if (GrammerUtils::match(">>", OPTIONAL_)) {

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

	if (systemFunctionCall()) {

		handleFunctionCallInExpr();

		return true;
	}
	else
		if (functionCall()) {

			handleFunctionCallInExpr();

			return true;
		}
		else
			if (memCmp()) {

				handleFunctionCallInExpr();

				return true;
			}
			else
				if (sizeOf()) {

					handleFunctionCallInExpr();

					return true;
				}
				else
					if (structStaticOrObjectAccess()) {
						return true;
					}
					else
						if (tk_identifier()) {
							return true;
						}
						else
							if (GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {

								sOperand = PREV_TOKEN_TEXT;
								m_vPostFix.push_back(sOperand);

								return true;
							}
							else
								if (GrammerUtils::match(TokenType_::Type::TK_FLOAT, OPTIONAL_)) {

									sOperand = PREV_TOKEN_TEXT;
									m_vPostFix.push_back(sOperand);

									return true;
								}
								else
									if (GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {

										sOperand = PREV_TOKEN_TEXT;
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
			if (GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, OPTIONAL_)) {

				std::string sOperand = PREV_TOKEN_TEXT;
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

bool TinyCReader::structStaticOrObjectAccess() {
	if (structStaticAccess()) {
		return true;
	}
	else
		if (structObjectAccess()) {
			return true;
		}
		else
			return false;

	return true;

}

bool TinyCReader::structStaticAccess() {
	if (!GrammerUtils::match(TokenType_::Type::TK_STATICMEMBERACCESS, MANDATORY_))
		return false;

	SAVE_PREV_TOKEN

		if (!GrammerUtils::match(':', MANDATORY_))
			return false;
	if (!GrammerUtils::match(':', MANDATORY_))
		return false;
	if (!structMemberVariableOrFunctionCall_RValue())
		return false;
	return true;

}

bool TinyCReader::structObjectAccess() {
	if (!GrammerUtils::match(TokenType_::Type::TK_MEMBERACCESS, MANDATORY_))
		return false;

	SAVE_PREV_TOKEN

		if (!GrammerUtils::match('-', MANDATORY_))
			return false;
	if (!GrammerUtils::match('>', MANDATORY_))
		return false;
	if (!structMemberVariableOrFunctionCall_RValue())
		return false;
	return true;

}

bool TinyCReader::structMemberVariableOrFunctionCall_RValue() {

	Token eSavedToken = SAVED_TOKEN;
	Tree* pTemp = nullptr;
	Tree* pObjectAccessNode = nullptr;

	std::string sObjectName = SAVED_TOKEN_TEXT;

	switch (eSavedToken.m_eTokenType)
	{
	case TokenType_::Type::TK_MEMBERACCESS:
	{
		std::string sFullyQualifiedObjectName = "";
		if (sObjectName == "this")
			sFullyQualifiedObjectName = "this";
		else
			sFullyQualifiedObjectName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sObjectName);

		pObjectAccessNode = makeLeaf(ASTNodeType::ASTNode_MEMBERACCESS, sFullyQualifiedObjectName.c_str());
		SET_INFO_FOR_KEY(pObjectAccessNode, "accessType", "object");
	}
	break;
	case TokenType_::Type::TK_STATICMEMBERACCESS:
	{
		pObjectAccessNode = makeLeaf(ASTNodeType::ASTNode_MEMBERACCESS, sObjectName.c_str());
		SET_INFO_FOR_KEY(pObjectAccessNode, "accessType", "static");
	}
	break;
	}

	{
		pObjectAccessNode->m_sAdditionalInfo.append(sObjectName);
		SET_INFO_FOR_KEY(pObjectAccessNode, "givenName", sObjectName);

		m_pASTCurrentNode->addChild(pObjectAccessNode);

		pTemp = m_pASTCurrentNode;
		m_pASTCurrentNode = pObjectAccessNode;
	}

	if (!structMemberVariableOrFunctionCall_RValue_1())
		return false;

	m_pASTCurrentNode = pTemp;

	return true;

}

bool TinyCReader::structMemberVariableOrFunctionCall_RValue_1() {
	if (structMemberFunctionCallInAnExpr()) {
		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, OPTIONAL_)) {

			{
				std::string sOperand = GET_INFO_FOR_KEY(m_pASTCurrentNode, "text");
				sOperand.append("->");
				sOperand.append(PREV_TOKEN_TEXT);

				GrammerUtils::m_vUserDefinedVariables.push_back(sOperand);

				m_vPostFix.push_back(sOperand);
			}

			return true;
		}
		else
			if (structMemberVariableArrayInAnExpr()) {
				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::structMemberVariableArrayInAnExpr() {
	if (!GrammerUtils::match(TokenType_::Type::TK_DEREFARRAY, MANDATORY_))
		return false;

	m_pASTCurrentNode->m_eASTNodeType = ASTNodeType::ASTNode_MEMBERACCESSDEREF;
	Tree* pIdentifierLeaf = makeLeaf(ASTNodeType::ASTNode_IDENTIFIER, PREV_TOKEN_TEXT);
	{
		SET_INFO_FOR_KEY(pIdentifierLeaf, "givenName", PREV_TOKEN_TEXT);
	}

	Tree* pArrayIndexExpressionLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");

	if (!GrammerUtils::match('[', MANDATORY_))
		return false;

	checkOpPrecedenceAndPush("(");

	if (!expr())
		return false;

	checkOpPrecedenceAndPush(")");

	pArrayIndexExpressionLeaf = createPostFixExpr(pArrayIndexExpressionLeaf);
	m_pASTCurrentNode->m_pLeftNode = pIdentifierLeaf;
	m_pASTCurrentNode->m_pRightNode = pArrayIndexExpressionLeaf;

	if (!GrammerUtils::match(']', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::structMemberFunctionCallInAnExpr() {
	if (!functionCall())
		return false;

	// The idea here is to create a temporary variable of the type returned by the function
	// & add it before the expression statement.
	// The temporary variable is then inserted in the expression.
	// Eg:
	// 		int32_t iRet = 10;
	//		iRet = 10 + obj1->retFunc(); // where retFunc return type is "int32_t".
	//		This will create a dummy code as follows:
	//			int32_t iRet = 10;
	//			int32_t iRet_retFunc = obj1->retFunc();	// This line of code will be inserted by the following piece of code.
	//			iRet = 10 + iRet_retFunc;

	Tree* pExpressionNode = m_pASTCurrentNode->m_pParentNode;		// In this case ASTNode_MEMBERACCESS's parent.
	Tree* pAssignNode = pExpressionNode->m_pParentNode;
	Tree* pBlockNode = pAssignNode->m_pParentNode;
	Tree* pFunctionCallNode = pExpressionNode->getLastStatement();
	{
		pFunctionCallNode->removeFromParent();
	}
	assert(pFunctionCallNode != nullptr);
	{
		std::string sFuncName = GET_INFO_FOR_KEY(pFunctionCallNode, "text");
		std::string sFullyQualifiedTempVariableName;
		sFullyQualifiedTempVariableName.append(GET_INFO_FOR_KEY(pAssignNode, "text"));
		sFullyQualifiedTempVariableName.append("_");
		sFullyQualifiedTempVariableName.append(sFuncName);

		Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedTempVariableName.c_str());
		{
			pPrimIntNode->m_sAdditionalInfo.append(sFullyQualifiedTempVariableName);
			SET_INFO_FOR_KEY(pPrimIntNode, "givenName", sFuncName);
			SET_INFO_FOR_KEY(pPrimIntNode, "type", "int32_t");
			SET_INFO_FOR_KEY(pPrimIntNode, "scope", getCurrentScopeString());
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

bool TinyCReader::unary_expr() {
	if (!unary_oper())
		return false;
	if (!expr())
		return false;
	return true;

}

bool TinyCReader::unary_oper() {
	if (GrammerUtils::match('-', OPTIONAL_)) {

		checkOpPrecedenceAndPush("NEGATE");

		return true;
	}
	else
		if (GrammerUtils::match('!', OPTIONAL_)) {

			checkOpPrecedenceAndPush("!");

			return true;
		}
		else
			if (GrammerUtils::match('~', OPTIONAL_)) {

				checkOpPrecedenceAndPush("~");

				return true;
			}
			else
				return false;

	return true;

}

bool TinyCReader::preFixInExpr() {
	if (GrammerUtils::match(TokenType_::Type::TK_PREFIXDECR, OPTIONAL_)) {

		if (m_pASTCurrentNode->m_pLeftNode == nullptr)
		{
			Tree* pPreFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_PREFIX, "");
			m_pASTCurrentNode->m_pLeftNode = pPreFixNode;
		}

		std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPreDecrNode = makeLeaf(ASTNodeType::ASTNode_PREDECR, sFullyQualifiedVariableName.c_str());
		{
			SET_INFO_FOR_KEY(pPreDecrNode, "givenName", sVariableName);

			m_pASTCurrentNode->m_pLeftNode->addChild(pPreDecrNode);
			m_vPostFix.push_back(sFullyQualifiedVariableName);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_PREFIXINCR, OPTIONAL_)) {

			if (m_pASTCurrentNode->m_pLeftNode == nullptr)
			{
				Tree* pPreFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_PREFIX, "");
				m_pASTCurrentNode->m_pLeftNode = pPreFixNode;
			}

			std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPreIncrNode = makeLeaf(ASTNodeType::ASTNode_PREINCR, sFullyQualifiedVariableName.c_str());
			{
				SET_INFO_FOR_KEY(pPreIncrNode, "givenName", sVariableName);

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
	if (!GrammerUtils::match(TokenType_::Type::TK_DEREFARRAY, MANDATORY_))
		return false;

	std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
	std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);

	if (!GrammerUtils::match('[', MANDATORY_))
		return false;

	checkOpPrecedenceAndPush("(");

	if (!expr())
		return false;

	checkOpPrecedenceAndPush(")");


	m_vPostFix.push_back(sFullyQualifiedVariableName);
	m_vPostFix.push_back("@");

	if (!GrammerUtils::match(']', MANDATORY_))
		return false;
	return true;

}

bool TinyCReader::postFixIncrDecrInExpr() {
	if (GrammerUtils::match(TokenType_::Type::TK_POSTFIXDECR, OPTIONAL_)) {

		if (m_pASTCurrentNode->m_pRightNode == nullptr)
		{
			Tree* pPostFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_POSTFIX, "");
			m_pASTCurrentNode->m_pRightNode = pPostFixNode;
		}

		std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
		std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
		Tree* pPostDecrNode = makeLeaf(ASTNodeType::ASTNode_POSTDECR, sFullyQualifiedVariableName.c_str());
		{
			SET_INFO_FOR_KEY(pPostDecrNode, "givenName", sVariableName);

			m_pASTCurrentNode->m_pRightNode->addChild(pPostDecrNode);
			m_vPostFix.push_back(sFullyQualifiedVariableName);
		}

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_POSTFIXINCR, OPTIONAL_)) {

			if (m_pASTCurrentNode->m_pRightNode == nullptr)
			{
				Tree* pPostFixNode = makeLeaf(ASTNodeType::ASTNode_EXPRESSION_POSTFIX, "");
				m_pASTCurrentNode->m_pRightNode = pPostFixNode;
			}

			std::string sVariableName = GrammerUtils::m_pPrevToken.m_sText;
			std::string sFullyQualifiedVariableName = getFullyQualifiedNameForVariable(m_pASTCurrentNode, sVariableName);
			Tree* pPostIncrNode = makeLeaf(ASTNodeType::ASTNode_POSTINCR, sFullyQualifiedVariableName.c_str());
			{
				SET_INFO_FOR_KEY(pPostIncrNode, "givenName", sVariableName);

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
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	checkOpPrecedenceAndPush("(");

	if (!expr())
		return false;
	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	checkOpPrecedenceAndPush(")");

	return true;

}

