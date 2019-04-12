#include "ByteArrayOutputStream.h"
#include <assert.h>

ByteArrayOutputStream::ByteArrayOutputStream(int8_t* pByteArray, uint64_t iArraySize)
: ByteArrayStream(pByteArray, iArraySize)
{}

void ByteArrayOutputStream::writeByte(int8_t iByte)
{
	assert(m_iCurrentPos < m_iArraySize);
	m_pByteArray[m_iCurrentPos++] = iByte;
}

void ByteArrayOutputStream::writeByteAtPos(int8_t iByte, uint32_t iCurrentPos)
{
	assert(iCurrentPos < m_iArraySize);
	m_pByteArray[iCurrentPos] = iByte;
}

void ByteArrayOutputStream::writeShort(int16_t iShort)
{
	assert((m_iCurrentPos + 1) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeByte(iShort & 0xff);
		//writeByte((iShort >> 8) & 0xff);

		int16_t* pShort = (int16_t*)&m_pByteArray[m_iCurrentPos];
		*pShort = iShort;
		m_iCurrentPos += sizeof(int16_t);
	}
	else
	{
		writeByte((iShort >> 8) & 0xff);
		writeByte(iShort & 0xff);
	}
}

void ByteArrayOutputStream::writeShortAtPos(int16_t iShort, uint32_t iCurrentPos)
{
	assert((iCurrentPos + 1) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeByteAtPos(iShort & 0xff, iCurrentPos);
		//iCurrentPos += sizeof(int8_t);
		//writeByteAtPos((iShort >> 8) & 0xff, iCurrentPos);

		int16_t* pShort = (int16_t*)&m_pByteArray[iCurrentPos];
		*pShort = iShort;
	}
	else
	{
		writeByteAtPos((iShort >> 8) & 0xff, iCurrentPos);
		iCurrentPos += sizeof(int8_t);
		writeByteAtPos(iShort & 0xff, iCurrentPos);
	}
}

void ByteArrayOutputStream::writeInt(int32_t iInt)
{
	assert((m_iCurrentPos + 3) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeShort(iInt & 0xffff);
		//writeShort((iInt >> 16) & 0xffff);

		int32_t* pInt = (int32_t*)&m_pByteArray[m_iCurrentPos];
		*pInt = iInt;
		m_iCurrentPos += sizeof(int32_t);
	}
	else
	{
		writeShort((iInt >> 16) & 0xffff);
		writeShort(iInt & 0xffff);
	}
}

void ByteArrayOutputStream::writeIntAtPos(int32_t iInt, uint32_t iCurrentPos)
{
	assert((iCurrentPos + 3) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeShortAtPos(iLong & 0xffff, iCurrentPos);
		//iCurrentPos += sizeof(int16_t);
		//writeShortAtPos((iLong >> 16) & 0xffff, iCurrentPos);

		int32_t* pInt = (int32_t*)&m_pByteArray[iCurrentPos];
		*pInt = iInt;
	}
	else
	{
		writeShortAtPos((iInt >> 16) & 0xffff, iCurrentPos);
		iCurrentPos += sizeof(int16_t);
		writeShortAtPos(iInt & 0xffff, iCurrentPos);
	}
}

void ByteArrayOutputStream::writeLong(int64_t iLong)
{
	assert((m_iCurrentPos + 7) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeInt(iLong & 0xffffffff);
		//writeInt((iLong >> 32) & 0xffffffff);

		int64_t* pLong = (int64_t*)&m_pByteArray[m_iCurrentPos];
		*pLong = iLong;
		m_iCurrentPos += sizeof(int64_t);
	}
	else
	{
		writeInt((iLong >> 32) & 0xffffffff);
		writeInt(iLong & 0xffffffff);
	}
}

void ByteArrayOutputStream::writeLongAtPos(int64_t iLong, uint32_t iCurrentPos)
{
	assert((iCurrentPos + 7) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeIntAtPos(iLong & 0xffffffff, iCurrentPos);
		//iCurrentPos += sizeof(int32_t);
		//writeIntAtPos((iLong >> 32) & 0xffffffff, iCurrentPos);

		int64_t* pLong = (int64_t*)&m_pByteArray[iCurrentPos];
		*pLong = iLong;
	}
	else
	{
		writeIntAtPos((iLong >> 32) & 0xffffffff, iCurrentPos);
		iCurrentPos += sizeof(int32_t);
		writeIntAtPos(iLong & 0xffffffff, iCurrentPos);
	}
}

void ByteArrayOutputStream::writeFloat(float fFloat)
{
	assert((m_iCurrentPos + 3) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeShort(iInt & 0xffff);
		//writeShort((iInt >> 16) & 0xffff);

		float* pFloat = (float*)&m_pByteArray[m_iCurrentPos];
		*pFloat = fFloat;
		m_iCurrentPos += sizeof(float);
	}
	else
	{
		//writeShort((fFloat >> 16) & 0xffff);
		//writeShort(fFloat & 0xffff);
	}
}

void ByteArrayOutputStream::writeFloatAtPos(float fFloat, uint32_t iCurrentPos)
{
	assert((iCurrentPos + 3) < m_iArraySize);

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//writeShortAtPos(iLong & 0xffff, iCurrentPos);
		//iCurrentPos += sizeof(int16_t);
		//writeShortAtPos((iLong >> 16) & 0xffff, iCurrentPos);

		float* pFloat = (float*)&m_pByteArray[iCurrentPos];
		*pFloat = fFloat;
	}
	else
	{
		//writeShortAtPos((fFloat >> 16) & 0xffff, iCurrentPos);
		//iCurrentPos += sizeof(int16_t);
		//writeShortAtPos(fFloat & 0xffff, iCurrentPos);
	}
}