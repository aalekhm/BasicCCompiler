#include "ByteArrayInputStream.h"
#include <assert.h>

ByteArrayInputStream::ByteArrayInputStream(int8_t* pByteArray, uint64_t iArraySize)
: ByteArrayStream(pByteArray, iArraySize)
{}

int8_t ByteArrayInputStream::readByte()
{
	assert(m_iCurrentPos < m_iArraySize);
	int8_t iByte = (int8_t)m_pByteArray[m_iCurrentPos++];
	return iByte;
}

int16_t ByteArrayInputStream::readShort()
{
	assert((m_iCurrentPos + 1) < m_iArraySize);

	int16_t iShort = 0;

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//iShort |= (int16_t)readByte() & 0xff;
		//iShort |= (int16_t)readByte() << 8;

		int16_t* pShort = (int16_t*)&m_pByteArray[m_iCurrentPos];
		iShort = *pShort;
		m_iCurrentPos += sizeof(int16_t);
	}
	else
	{
		iShort |= (int16_t)readByte() << 8;
		iShort |= (int16_t)readByte() & 0xff;
	}

	return iShort;
}

int32_t ByteArrayInputStream::readInt()
{
	assert((m_iCurrentPos + 3) < m_iArraySize);

	int32_t iInt = 0;

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//iInt |= (int32_t)readInt() & 0xffff;
		//iInt |= (int32_t)readInt() << 16;

		int32_t* pInt = (int32_t*)&m_pByteArray[m_iCurrentPos];
		iInt = *pInt;
		m_iCurrentPos += sizeof(int32_t);
	}
	else
	{
		iInt |= (int32_t)readShort() << 16;
		iInt |= (int32_t)readShort() & 0xffff;
	}

	return iInt;
}

int64_t ByteArrayInputStream::readLong()
{
	assert((m_iCurrentPos + 7) < m_iArraySize);

	int64_t iLong = 0;

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//iLong |= (int64_t)readInt() & 0xffffffff;
		//iLong |= (int64_t)readInt() << 32;

		int64_t* pLong = (int64_t*)&m_pByteArray[m_iCurrentPos];
		iLong = *pLong;
		m_iCurrentPos += sizeof(int64_t);
	}
	else
	{
		iLong |= (int64_t)readInt() << 32;
		iLong |= (int64_t)readInt() & 0xffffffff;
	}

	return iLong;
}

int8_t ByteArrayInputStream::readByteAtPos(int64_t iOffset)
{
	assert(iOffset < m_iArraySize);

	int8_t iByte = (int8_t)m_pByteArray[iOffset];
	return iByte;
}

int16_t	ByteArrayInputStream::readShortAtPos(int64_t iOffset)
{
	assert((iOffset + 1) < m_iArraySize);

	int16_t iShort = 0;

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//iShort |= (int16_t)readByteAtPos(iOffset) & 0xff;
		//iOffset += sizeof(int8_t);
		//iShort |= (int16_t)readByteAtPos(iOffset) << 8;

		int16_t* pShort = (int16_t*)&m_pByteArray[iOffset];
		iShort = *pShort;
	}
	else
	{
		iShort |= (int16_t)readByteAtPos(iOffset) << 8;
		iOffset += sizeof(int8_t);
		iShort |= (int16_t)readByteAtPos(iOffset) & 0xff;
	}

	return iShort;
}

int32_t	ByteArrayInputStream::readIntAtPos(int64_t iOffset)
{
	assert((iOffset + 3) < m_iArraySize);

	int32_t iInt = 0;

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//iInt |= (int32_t)readShortAtPos(iOffset) & 0xffff;
		//iOffset += sizeof(int16_t);
		//iInt |= (int32_t)readShortAtPos(iOffset) << 16;

		int32_t* pInt = (int32_t*)&m_pByteArray[iOffset];
		iInt = *pInt;
	}
	else
	{
		iInt |= (int32_t)readShortAtPos(iOffset) << 16;
		iOffset += sizeof(int16_t);
		iInt |= (int32_t)readShortAtPos(iOffset) & 0xffff;
	}

	return iInt;
}

int64_t	ByteArrayInputStream::readLongAtPos(int64_t iOffset)
{
	assert((iOffset + 7) < m_iArraySize);

	int64_t iLong = 0;

	if (m_eEndianness == ENDIANNESS::LITTLE)
	{
		//iLong |= (int64_t)readIntAtPos(iOffset) & 0xffffffff;
		//iOffset += sizeof(int32_t);
		//iLong |= (int64_t)readIntAtPos(iOffset) << 32;

		int64_t* pLong = (int64_t*)&m_pByteArray[iOffset];
		iLong = *pLong;
	}
	else
	{
		iLong |= (int64_t)readIntAtPos(iOffset) << 32;
		iOffset += sizeof(int32_t);
		iLong |= (int64_t)readIntAtPos(iOffset) & 0xffffffff;
	}

	return iLong;
}
