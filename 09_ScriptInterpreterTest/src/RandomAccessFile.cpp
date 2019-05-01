#include "RandomAccessFile.h"
#include <io.h>
#include <iostream>

#define ACCESS_EXISTENCE_ONLY	0
#define ACCESS_WRITE_ONLY		2
#define ACCESS_READ_ONLY		4
#define ACCESS_READ_AND_WRITE	6

#define SUCCESS					0
#define NOT						!

RandomAccessFile::RandomAccessFile()
: m_pFileObj(nullptr)
, m_iFileOffset(0)
, m_sFileName("")
{
}

bool RandomAccessFile::touch(const char* sFileNameToCreate, int iPermission)
{
	errno_t fErr;

	_creat(sFileNameToCreate, iPermission);
	_get_errno(&fErr);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << sFileNameToCreate << " specifies an existing read-only file or specifies a directory instead of a file." << std::endl;

		if (fErr == EMFILE)
			std::cout << "No more file descriptors are available." << sFileNameToCreate << std::endl;
		
		if (fErr == ENOENT)
			std::cout << "Specified file could not be found. " << sFileNameToCreate << std::endl;

		return false;
	}

	return true;
}

bool RandomAccessFile::openForRead(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sFileName << std::endl;

		return false;
	}

	// Open file for reading
	fErr = fopen_s(&m_pFileObj, sFileName, "rb");

	if (fErr != SUCCESS)
	{
		std::cout << "could not open File / Directory " << sFileName << " for reading." << std::endl;
		return false;
	}

	m_iFileOffset = 0;
	m_sFileName = sFileName;

	return true;
}

bool RandomAccessFile::openForWrite(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file can be written to!
	fErr = _access(sFileName, ACCESS_WRITE_ONLY);
	if (fErr == EACCES)
	{
		std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		return false;
	}

	// Open file for writing.
	fErr = fopen_s(&m_pFileObj, sFileName, "wb+");

	if (fErr != SUCCESS)
	{
		std::cout << "could not open File / Directory " << sFileName << " for writing." << std::endl;
		return false;
	}

	m_iFileOffset = 0;
	m_sFileName = sFileName;

	return true;
}

bool RandomAccessFile::openForAppend(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sFileName << std::endl;

		return false;
	}

	// Open file for appending.
	fErr = fopen_s(&m_pFileObj, sFileName, "a+");

	if (fErr != SUCCESS)
	{
		std::cout << "could not open File / Directory " << sFileName << " for appending." << std::endl;
		return false;
	}

	m_iFileOffset = length();
	m_sFileName = sFileName;

	return true;
}

bool RandomAccessFile::renameFile(const char* sNewFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sNewFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sNewFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sNewFileName << std::endl;

		return false;
	}

	// try renaming the file.
	fErr = ::rename(m_sFileName.c_str(), sNewFileName);

	if (fErr != SUCCESS)
	{
		std::cout << "could not rename File / Directory " << m_sFileName << std::endl;
		return false;
	}

	m_sFileName = sNewFileName;

	return true;
}

bool RandomAccessFile::deleteFile(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if(fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sFileName << std::endl;

		return false;
	}

	// try deleting the file.
	fErr = ::remove(sFileName);

	if (fErr != SUCCESS)
	{
		std::cout << "could not delete File / Directory " << sFileName << std::endl;
		return false;
	}

	return true;
}

unsigned long RandomAccessFile::length()
{
	unsigned long iFileLength = 0;

	if (m_pFileObj != nullptr)
	{
		// save last offset position.
		unsigned long iLastOffsetPos = ftell(m_pFileObj);

		// get the length.
		fseek(m_pFileObj, 0l, SEEK_END);
		iFileLength = ftell(m_pFileObj);

		// restore offset pointer.
		fseek(m_pFileObj, iLastOffsetPos, SEEK_SET);
	}

	return iFileLength;
}

unsigned long RandomAccessFile::read(char* pBuf)
{
	return read(pBuf, 0, length());
}

unsigned long RandomAccessFile::read(char* pBuf, int iOffset, int iLength)
{
	int iCount = 0;
	int iBytesToRead = iLength, iBytesRead = 0;

	if (iBytesToRead <= 0)
		return -1;

	if(m_pFileObj != nullptr)
	{	
		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		while (	NOT isEOF()
				&&
				iBytesRead < iBytesToRead
		) {
			iBytesRead = fread(pBuf + iOffset, sizeof(char), iBytesToRead, m_pFileObj);
			m_iFileOffset += iBytesRead;
		}

		pBuf[iLength] = '\0';
	}

	return iBytesRead;
}

char RandomAccessFile::readChar() 
{
	if (m_pFileObj != NULL) 
	{
		unsigned char ch;

		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		ch = fgetc(m_pFileObj);

		m_iFileOffset += 1;

		return ch;
	}

	return NULL;
}

std::string RandomAccessFile::readLine()
{
	if (NOT isEOF())
	{
		int iPrevFilePointer = m_iFileOffset;

		unsigned char c = readChar();
		while (NOT(c == '\r' || c == '\n') && NOT isEOF())
		{
			c = readChar();
		}

		int iLineLength = m_iFileOffset - iPrevFilePointer - 1;
		if (c == '\r')
		{
			m_iFileOffset++;
		}

		if (iLineLength > 0)
		{
			void* pLine = calloc(iLineLength, sizeof(pLine));

			fseek(m_pFileObj, iPrevFilePointer, SEEK_SET);

			int iRead = fread(pLine, sizeof(char), iLineLength, m_pFileObj);
			fseek(m_pFileObj, m_iFileOffset, SEEK_SET);

			std::string sLine = (char*)pLine;
			delete[] pLine;

			return sLine;
		}
	}

	return "";
}

unsigned long RandomAccessFile::write(const char* sBuf)
{
	int iBytesWritten = 0;
	if (m_pFileObj != nullptr)
	{
		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		iBytesWritten = fwrite(sBuf, sizeof(char), strlen(sBuf), m_pFileObj);

		m_iFileOffset += iBytesWritten;
	}

	return iBytesWritten;
}

unsigned long RandomAccessFile::write(const char* sBuf, unsigned long iOffset, unsigned long iLen)
{
	int iBytesWritten = 0;
	if (m_pFileObj != nullptr)
	{
		char* pStartBufOffset = (char*)sBuf + iOffset;

		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		iBytesWritten = fwrite(pStartBufOffset, sizeof(char), iLen, m_pFileObj);

		m_iFileOffset += iBytesWritten;
	}

	return iBytesWritten;
}

void RandomAccessFile::writeLine(const char* sBuf)
{
	if (m_pFileObj != nullptr)
	{
		write(sBuf);
		write("\r\n");
	}
}

bool RandomAccessFile::isEOF()
{
	if (m_pFileObj != nullptr) 
	{
		return feof(m_pFileObj) ? true : false;
	}

	return true;
}

void RandomAccessFile::writeByte(int byte0) {
	if (m_pFileObj != NULL) 
	{
		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		fputc(byte0, m_pFileObj);

		m_iFileOffset += 1;
	}
}

void RandomAccessFile::writeShort(int shortValue) 
{
	if (m_pFileObj != NULL) 
	{
		writeByte(shortValue & 0xff);
		writeByte((shortValue >> 8) & 0xff);
	}
}

void RandomAccessFile::writeInt(unsigned long intValue)
{
	if (m_pFileObj != NULL) 
	{
		writeShort(intValue & 0xffff);
		writeShort((intValue >> 16) & 0xffff);
	}
}

void RandomAccessFile::writeLong(unsigned long long longValue) 
{
	if (m_pFileObj != NULL) 
	{
		writeInt(longValue & 0xffffffff);
		writeInt((longValue >> 32) & 0xffffffff);
	}
}

void RandomAccessFile::close()
{
	if (m_pFileObj != nullptr)
	{
		errno_t fErr;
		fErr = fclose(m_pFileObj);

		if (fErr == 0)
		{
			std::cout << "File " << m_sFileName << " closed successfully\n" << std::endl;
		}
		else
		{
			std::cout << "Unable to close File \n" << m_sFileName << std::endl;
		}
	}

	m_pFileObj = nullptr;
	m_iFileOffset = 0;
	m_sFileName = "";
}

RandomAccessFile::~RandomAccessFile()
{
	close();
}
