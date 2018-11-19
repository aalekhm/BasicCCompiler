#pragma once
#include <vector>
#include "Token.h"

class NonTerminal
{
	public:
		static NonTerminal*			create(Token tok);
		void						addToken(Token tok);
		Token						get();
		const std::vector<Token>&	getTokenList();
	protected:
	private:
									NonTerminal() = delete;
									NonTerminal(Token tok);
		virtual						~NonTerminal();

		Token						m_pToken;
		std::vector<Token>			m_vTokenList;
};