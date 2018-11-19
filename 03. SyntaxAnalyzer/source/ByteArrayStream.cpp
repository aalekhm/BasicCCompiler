#include "ByteArrayStream.h"
#include <assert.h>

ByteArrayStream::ByteArrayStream(int8_t* pByteArray, uint64_t iArraySize)
: m_pByteArray(pByteArray)
, m_iCurrentPos(0)
, m_iArraySize(iArraySize)
, m_eEndianness(ENDIANNESS::LITTLE)
{}

void ByteArrayStream::setEndianness(ENDIANNESS eENDIANNESS)
{
	m_eEndianness = eENDIANNESS;
}

uint64_t ByteArrayStream::getCurrentOffset()
{
	return m_iCurrentPos;
}

void ByteArrayStream::setCurrentOffset(uint64_t iNewOffset)
{
	assert(iNewOffset <= m_iArraySize);
	m_iCurrentPos = iNewOffset;
}

void ByteArrayStream::reset()
{
	m_iCurrentPos = 0;
}