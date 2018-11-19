
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "RandomAccessFile.h"
#include "StringTokenizer.h"
#include "Token.h"

int main()
{
	RandomAccessFile* pRaf = new RandomAccessFile();
	if (pRaf->openForRead("testCases\\test.c"))
	{
		unsigned long iLength = pRaf->length();
		char* sBuff = new char[iLength + strlen("\r\n") + 1];
		memset(sBuff, 0, iLength);

		unsigned long iBytesRead = pRaf->read(sBuff);
		if (iBytesRead > 0)
		{
			StringTokenizer* st = StringTokenizer::create(sBuff);
			st->tokenize();
			while (st->hasMoreTokens())
			{
				Token tok = st->nextToken();
				if(	!(tok.getType() == TokenType::Type::TK_EOL || tok.getType() == TokenType::Type::TK_WHITESPACE) )
					std::cout << "Token :: " << TokenType::toString(tok.getType()) << "\t" << tok.getText() << std::endl;
			}
		}
		
		delete[] sBuff;
		sBuff = nullptr;
	}
	pRaf->close();

	exit(EXIT_SUCCESS);
}