#pragma once
#include <string>
#include <assert.h>

namespace TokenType
{
	enum class Type
	{
		TK_INVALID = -1,
		TK_MUL,
		TK_MULEQ,
		TK_DIV,
		TK_DIVEQ,
		TK_MOD,
		TK_ADD,
		TK_ADDEQ,
		TK_SUB,
		TK_SUBEQ,
		TK_NEGATE,
		TK_NOT,
		TK_LT,
		TK_LTEQ,
		TK_GT,
		TK_GTEQ,
		TK_EQ,
		TK_NEQ,
		TK_ASSIGN,
		TK_LOGICALAND,
		TK_LOGICALOR,
		TK_BITWISEAND,
		TK_BITWISEOR,
		TK_BITWISEXOR,
		TK_BITWISENOT,
		TK_BITWISELEFTSHIFT,
		TK_BITWISERIGHTSHIFT,
		TK_IF,
		TK_ELSE,
		TK_FOR,
		TK_WHILE,
		TK_PRINT,
		TK_LPAREN,
		TK_RPAREN,
		TK_LBRACE,
		TK_RBRACE,
		TK_LSQBRACKET,
		TK_RSQBRACKET,
		TK_SEMICOL,
		TK_COMMA,
		TK_IDENTIFIER,
		TK_INTEGER,
		TK_FLOAT,
		TK_STRING,
		TK_CHARACTER,
		TK_EOL,
		TK_EOI,
		TK_SINGLELINECOMMENT,
		TK_MULTILINECOMMENT,
		TK_WHITESPACE,
		TK_BNFNONTERMINAL,
		TK_BNFASSIGNMENT,
		TK_BNFCODE,
		TK_FUNCTIONCALL,
		TK_PREFIXDECR,
		TK_PREFIXINCR,
		TK_POSTFIXDECR,
		TK_POSTFIXINCR,
		TK_SWITCH,
		TK_DEREF,
		TK_DEREFARRAY,
		TK_MEMBERACCESS,
		TK_STATICMEMBERACCESS,
		TK_SYSTEMFUNCTIONCALL,
		TK_UNKNOWN
	};

	inline std::string toString(Type eType)
	{
		switch (eType)
		{
			case Type::TK_INVALID:				return "TK_INVALID";
			case Type::TK_MUL:					return "TK_MUL";
			case Type::TK_MULEQ:				return "TK_MULEQ";
			case Type::TK_DIV:					return "TK_DIV";
			case Type::TK_DIVEQ:				return "TK_DIVEQ";
			case Type::TK_MOD:					return "TK_MOD";
			case Type::TK_ADD:					return "TK_ADD";
			case Type::TK_ADDEQ:				return "TK_ADDEQ";
			case Type::TK_SUB:					return "TK_SUB";
			case Type::TK_SUBEQ:				return "TK_SUBEQ";
			case Type::TK_NEGATE:				return "TK_NEGATE";
			case Type::TK_NOT:					return "TK_NOT";
			case Type::TK_LT:					return "TK_LT";
			case Type::TK_LTEQ:					return "TK_LTEQ";
			case Type::TK_GT:					return "TK_GT";
			case Type::TK_GTEQ:					return "TK_GTEQ";
			case Type::TK_EQ:					return "TK_EQ";
			case Type::TK_NEQ:					return "TK_NEQ";
			case Type::TK_ASSIGN:				return "TK_ASSIGN";
			case Type::TK_LOGICALAND:			return "TK_LOGICALAND";
			case Type::TK_LOGICALOR:			return "TK_LOGICALOR";
			case Type::TK_BITWISEAND:			return "TK_BITWISEAND";
			case Type::TK_BITWISEOR:			return "TK_BITWISEOR";
			case Type::TK_BITWISEXOR:			return "TK_BITWISEXOR";
			case Type::TK_BITWISENOT:			return "TK_BITWISENOT";
			case Type::TK_BITWISELEFTSHIFT:		return "TK_BITWISELEFTSHIFT";
			case Type::TK_BITWISERIGHTSHIFT:	return "TK_BITWISERIGHTSHIFT";
			case Type::TK_IF:					return "TK_IF";
			case Type::TK_ELSE:					return "TK_ELSE";
			case Type::TK_FOR:					return "TK_FOR";
			case Type::TK_WHILE:				return "TK_WHILE";
			case Type::TK_PRINT:				return "TK_PRINT";
			case Type::TK_LPAREN:				return "TK_LPAREN";
			case Type::TK_RPAREN:				return "TK_RPAREN";
			case Type::TK_LBRACE:				return "TK_LBRACE";
			case Type::TK_RBRACE:				return "TK_RBRACE";
			case Type::TK_LSQBRACKET:			return "TK_LSQBRACKET";
			case Type::TK_RSQBRACKET:			return "TK_RSQBRACKET";
			case Type::TK_SEMICOL:				return "TK_SEMICOL";
			case Type::TK_COMMA:				return "TK_COMMA";
			case Type::TK_IDENTIFIER:			return "TK_IDENTIFIER";
			case Type::TK_INTEGER:				return "TK_INTEGER";
			case Type::TK_FLOAT:				return "TK_FLOAT";
			case Type::TK_STRING:				return "TK_STRING";
			case Type::TK_CHARACTER:			return "TK_CHARACTER";
			case Type::TK_EOL:					return "TK_EOL";
			case Type::TK_EOI:					return "TK_EOI";
			case Type::TK_SINGLELINECOMMENT:	return "TK_SINGLELINECOMMENT";
			case Type::TK_MULTILINECOMMENT:		return "TK_MULTILINECOMMENT";
			case Type::TK_WHITESPACE:			return "TK_WHITESPACE";
			case Type::TK_BNFNONTERMINAL:		return "TK_BNFNONTERMINAL";
			case Type::TK_BNFASSIGNMENT:		return "TK_BNFASSIGNMENT";
			case Type::TK_BNFCODE:				return "TK_BNFCODE";
			case Type::TK_FUNCTIONCALL:			return "TK_FUNCTIONCALL";
			case Type::TK_PREFIXDECR:			return "TK_PREFIXDECR";
			case Type::TK_PREFIXINCR:			return "TK_PREFIXINCR";
			case Type::TK_POSTFIXDECR:			return "TK_POSTFIXDECR";
			case Type::TK_POSTFIXINCR:			return "TK_POSTFIXINCR";
			case Type::TK_SWITCH:				return "TK_SWITCH";
			case Type::TK_DEREF:				return "TK_DEREF";
			case Type::TK_DEREFARRAY:			return "TK_DEREFARRAY";
			case Type::TK_MEMBERACCESS:			return "TK_MEMBERACCESS";
			case Type::TK_STATICMEMBERACCESS:	return "TK_STATICMEMBERACCESS";
			case Type::TK_SYSTEMFUNCTIONCALL:	return "TK_SYSTEMFUNCTIONCALL";

			case Type::TK_UNKNOWN:				return "TK_UNKNOWN";
		}

		return "";
	}

	inline TokenType::Type fromString(std::string sTokenType)
	{
			 if(sTokenType == "TK_MUL")					return Type::TK_MUL;
		else if(sTokenType == "TK_MULEQ")				return Type::TK_MULEQ;
		else if(sTokenType == "TK_DIV")					return Type::TK_DIV;
		else if(sTokenType == "TK_DIVEQ")				return Type::TK_DIVEQ;
		else if(sTokenType == "TK_MOD")					return Type::TK_MOD;
		else if(sTokenType == "TK_ADD")					return Type::TK_ADD;
		else if(sTokenType == "TK_ADDEQ")				return Type::TK_ADDEQ;
		else if(sTokenType == "TK_SUB")					return Type::TK_SUB;
		else if(sTokenType == "TK_SUBEQ")				return Type::TK_SUBEQ;
		else if(sTokenType == "TK_NEGATE")				return Type::TK_NEGATE;
		else if(sTokenType == "TK_NOT")					return Type::TK_NOT;
		else if(sTokenType == "TK_LT")					return Type::TK_LT;
		else if(sTokenType == "TK_LTEQ")				return Type::TK_LTEQ;
		else if(sTokenType == "TK_GT")					return Type::TK_GT;
		else if(sTokenType == "TK_GTEQ")				return Type::TK_GTEQ;
		else if(sTokenType == "TK_EQ")					return Type::TK_EQ;
		else if(sTokenType == "TK_NEQ")					return Type::TK_NEQ;
		else if(sTokenType == "TK_ASSIGN")				return Type::TK_ASSIGN;
		else if(sTokenType == "TK_LOGICALAND")			return Type::TK_LOGICALAND;
		else if(sTokenType == "TK_LOGICALOR")			return Type::TK_LOGICALOR;
		else if(sTokenType == "TK_BITWISEAND")			return Type::TK_BITWISEAND;
		else if(sTokenType == "TK_BITWISEOR")			return Type::TK_BITWISEOR;
		else if(sTokenType == "TK_BITWISEXOR")			return Type::TK_BITWISEXOR;
		else if(sTokenType == "TK_BITWISENOT")			return Type::TK_BITWISENOT;
		else if(sTokenType == "TK_BITWISELEFTSHIFT")	return Type::TK_BITWISELEFTSHIFT;
		else if(sTokenType == "TK_BITWISERIGHTSHIFT")	return Type::TK_BITWISERIGHTSHIFT;
		else if(sTokenType == "TK_IF")					return Type::TK_IF;
		else if(sTokenType == "TK_ELSE")				return Type::TK_ELSE;
		else if(sTokenType == "TK_FOR")					return Type::TK_FOR;
		else if(sTokenType == "TK_WHILE")				return Type::TK_WHILE;
		else if(sTokenType == "TK_PRINT")				return Type::TK_PRINT;
		else if(sTokenType == "TK_LPAREN")				return Type::TK_LPAREN;
		else if(sTokenType == "TK_RPAREN")				return Type::TK_RPAREN;
		else if(sTokenType == "TK_LBRACE")				return Type::TK_LBRACE;
		else if(sTokenType == "TK_RBRACE")				return Type::TK_RBRACE;
		else if(sTokenType == "TK_LSQBRACKET")			return Type::TK_LSQBRACKET;
		else if(sTokenType == "TK_RSQBRACKET")			return Type::TK_RSQBRACKET;
		else if(sTokenType == "TK_SEMICOL")				return Type::TK_SEMICOL;
		else if(sTokenType == "TK_COMMA")				return Type::TK_COMMA;
		else if(sTokenType == "TK_IDENTIFIER")			return Type::TK_IDENTIFIER;
		else if(sTokenType == "TK_INTEGER")				return Type::TK_INTEGER;
		else if(sTokenType == "TK_FLOAT")				return Type::TK_FLOAT;
		else if(sTokenType == "TK_STRING")				return Type::TK_STRING;
		else if(sTokenType == "TK_CHARACTER")			return Type::TK_CHARACTER;
		else if(sTokenType == "TK_EOL")					return Type::TK_EOL;
		else if(sTokenType == "TK_EOI")					return Type::TK_EOI;
		else if(sTokenType == "TK_SINGLELINECOMMENT")	return Type::TK_SINGLELINECOMMENT;
		else if(sTokenType == "TK_MULTILINECOMMENT")	return Type::TK_MULTILINECOMMENT;
		else if(sTokenType == "TK_WHITESPACE")			return Type::TK_WHITESPACE;
		else if(sTokenType == "TK_BNFNONTERMINAL")		return Type::TK_BNFNONTERMINAL;
		else if(sTokenType == "TK_BNFASSIGNMENT")		return Type::TK_BNFASSIGNMENT;
		else if(sTokenType == "TK_BNFCODE")				return Type::TK_BNFCODE;
		else if(sTokenType == "TK_FUNCTIONCALL")		return Type::TK_FUNCTIONCALL;
		else if(sTokenType == "TK_PREFIXDECR")			return Type::TK_PREFIXDECR;
		else if(sTokenType == "TK_PREFIXINCR")			return Type::TK_PREFIXINCR;
		else if(sTokenType == "TK_POSTFIXDECR")			return Type::TK_POSTFIXDECR;
		else if(sTokenType == "TK_POSTFIXINCR")			return Type::TK_POSTFIXINCR;
		else if(sTokenType == "TK_SWITCH")				return Type::TK_SWITCH;
		else if(sTokenType == "TK_DEREF")				return Type::TK_DEREF;
		else if(sTokenType == "TK_DEREFARRAY")			return Type::TK_DEREFARRAY;
		else if(sTokenType == "TK_MEMBERACCESS")		return Type::TK_MEMBERACCESS;
		else if (sTokenType == "TK_STATICMEMBERACCESS")	return Type::TK_STATICMEMBERACCESS;
		else if (sTokenType == "TK_SYSTEMFUNCTIONCALL")	return Type::TK_SYSTEMFUNCTIONCALL;

		else if(sTokenType == "TK_UNKNOWN")				return Type::TK_UNKNOWN;
		else											return Type::TK_INVALID;
	}
}

struct Token
{
	Token(TokenType::Type eTokenType, std::string sText, int iLine, int iColumn)
		: m_eTokenType(eTokenType)
		, m_sText(sText)
		, m_iLine(iLine)
		, m_iColumn(iColumn)
	{}

	TokenType::Type	m_eTokenType;
	std::string			m_sText;
	int					m_iLine;
	int					m_iColumn;

	TokenType::Type	getType() { return m_eTokenType; }
	const char*			getText() { return m_sText.c_str(); }
};