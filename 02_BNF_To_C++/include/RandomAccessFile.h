#pragma once

#include <stdio.h>
#include <string>

enum class PERM
{
	PERM_READ = _S_IREAD,
	PERM_WRITE = _S_IWRITE,
	PERM_EXEC = _S_IEXEC,
	PERM_ALL = PERM_READ | PERM_WRITE | PERM_EXEC
};

class RandomAccessFile
{
	public:
							RandomAccessFile();
		virtual				~RandomAccessFile();

		bool				openForRead(const char* sFileName);
		bool				openForWrite(const char* sFileName);
		bool				openForAppend(const char* sFileName);
		bool				renameFile(const char* sNewFileName);
		static bool			deleteFile(const char* sFileName);
		static bool			touch(const char* sFileNameToCreate, int iPermission = (int)PERM::PERM_ALL);

		unsigned long		length();

		unsigned long		read(char* pBuf);
		unsigned long		read(char* pBuf, int iOffset, int iLength);
		char				readChar();
		std::string			readLine();

		unsigned long 		write(const char* sBuf);
		unsigned long		write(const char* sBuf, unsigned long iOffset, unsigned long iLen);
		void				writeLine(const char* sBuf);

		void				writeByte(int byte0);
		void				writeShort(int shortValue);
		void				writeInt(unsigned long);
		void				writeLong(unsigned long long longValue);

		bool				isEOF();

		void				close();
	protected:
	private:
		FILE*				m_pFileObj;
		unsigned long long	m_iFileOffset;
		std::string			m_sFileName;
};