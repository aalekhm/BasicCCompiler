#include "NonTerminal.h"

NonTerminal::NonTerminal(Token tok)
: m_pToken(tok)
{
}

Token NonTerminal::get()
{
	return m_pToken;
}

NonTerminal* NonTerminal::create(Token tok)
{
	NonTerminal* pNonTerminal = new NonTerminal(tok);
	return pNonTerminal;
}

void NonTerminal::addToken(Token tok)
{
	m_vTokenList.push_back(tok);
}

const std::vector<Token>& NonTerminal::getTokenList()
{
	return m_vTokenList;
}

NonTerminal::~NonTerminal()
{

}
