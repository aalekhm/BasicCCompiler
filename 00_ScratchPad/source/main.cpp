#include <iostream>
#include <vector>
#include <assert.h>
#include "Singleton.h"

enum class ENDIANNESS
{
	LITTLE,
	BIG
};

class ByteArrayInputStream
{
	public:
		ByteArrayInputStream(int8_t* pByteArray, uint64_t iArraySize)
		: m_pByteArray(pByteArray)
		, m_iCurrentPos(0)
		, m_iArraySize(iArraySize)
		, m_eEndianness(ENDIANNESS::LITTLE)
		{}

		inline void setEndianness(ENDIANNESS eENDIANNESS)
		{
			m_eEndianness = eENDIANNESS;
		}

		inline uint8_t readByte()
		{
			assert(m_iCurrentPos < m_iArraySize);
			uint8_t iByte = (uint8_t)m_pByteArray[m_iCurrentPos++];
			return iByte;
		}

		inline int16_t readShort()
		{
			assert((m_iCurrentPos + 1) < m_iArraySize);

			uint16_t iShort = 0;

			if (m_eEndianness == ENDIANNESS::LITTLE)
			{
				iShort |= (uint16_t)readByte() & 0xff;
				iShort |= (uint16_t)readByte() << 8;
			}
			else
			{
				iShort |= (uint16_t)readByte() << 8;
				iShort |= (uint16_t)readByte() & 0xff;
			}

			return iShort;
		}

		inline uint32_t readInt()
		{
			assert((m_iCurrentPos + 3) < m_iArraySize);

			uint32_t iInt = 0;

			if (m_eEndianness == ENDIANNESS::LITTLE)
			{
				iInt |= (uint32_t)readShort() & 0xffff;
				iInt |= (uint32_t)readShort() << 16;
			}
			else
			{
				iInt |= (uint32_t)readShort() << 16;
				iInt |= (uint32_t)readShort() & 0xffff;
			}

			return iInt;
		}

		inline uint64_t readLong()
		{
			assert((m_iCurrentPos + 7) < m_iArraySize);

			uint64_t iLong = 0;

			if (m_eEndianness == ENDIANNESS::LITTLE)
			{
				iLong |= (uint64_t)readInt() & 0xffffffff;
				iLong |= (uint64_t)readInt() << 32;
			}
			else
			{
				iLong |= (uint64_t)readInt() << 32;
				iLong |= (uint64_t)readInt() & 0xffffffff;
			}

			return iLong;
		}
	protected:
	private:
		int8_t*		m_pByteArray;
		int8_t		m_iCurrentPos;
		uint64_t	m_iArraySize;
		ENDIANNESS	m_eEndianness;
};

class ByteArrayOuputStream
{
	public:
		ByteArrayOuputStream(int8_t* pByteArray, uint64_t iArraySize)
		: m_pByteArray(pByteArray)
		, m_iCurrentPos(0)
		, m_iArraySize(iArraySize)
		, m_eEndianness(ENDIANNESS::LITTLE)
		{}

		inline void setEndianness(ENDIANNESS eENDIANNESS)
		{
			m_eEndianness = eENDIANNESS;
		}
		
		inline void writeByte(uint8_t iByte)
		{
			assert(m_iCurrentPos < m_iArraySize);
			m_pByteArray[m_iCurrentPos++] = iByte;
		}

		inline void writeShort(uint16_t iShort)
		{
			assert((m_iCurrentPos + 1) < m_iArraySize);

			if (m_eEndianness == ENDIANNESS::LITTLE)
			{
				writeByte(iShort & 0xff);
				writeByte((iShort >> 8) & 0xff);
			}
			else
			{
				writeByte((iShort >> 8) & 0xff);
				writeByte(iShort & 0xff);
			}
		}

		inline void writeInt(uint32_t iInt)
		{
			assert((m_iCurrentPos + 3) < m_iArraySize);

			if (m_eEndianness == ENDIANNESS::LITTLE)
			{
				writeShort(iInt & 0xffff);
				writeShort((iInt >> 16) & 0xffff);
			}
			else
			{
				writeShort((iInt >> 16) & 0xffff);
				writeShort(iInt & 0xffff);
			}
		}

		inline void writeLong(uint64_t iLong)
		{
			assert((m_iCurrentPos + 7) < m_iArraySize);

			if (m_eEndianness == ENDIANNESS::LITTLE)
			{
				writeInt(iLong & 0xffffffff);
				writeInt((iLong >> 32) & 0xffffffff);
			}
			else
			{
				writeInt((iLong >> 32) & 0xffffffff);
				writeInt(iLong & 0xffffffff);
			}
		}
	protected:
	private:
		int8_t*		m_pByteArray;
		int8_t		m_iCurrentPos;
		uint64_t	m_iArraySize;
		ENDIANNESS	m_eEndianness;
};

#define BUFF_SIZE 512

class A : public Singleton<A>
{

};

int main(int argc, char* argv[])
{
	int8_t sBuff[BUFF_SIZE];
	memset(sBuff, 0, sizeof(int8_t) * BUFF_SIZE);

	ByteArrayOuputStream* baos = new ByteArrayOuputStream(sBuff, sizeof(int8_t) * BUFF_SIZE);
	{
		baos->setEndianness(ENDIANNESS::BIG);

		baos->writeByte(0xEF);
		baos->writeShort(0xDFAB);
		baos->writeInt(0xFE679823);
		baos->writeLong(0x78653211FE679823);
	}

	ByteArrayInputStream* bais = new ByteArrayInputStream(sBuff, sizeof(int8_t) * BUFF_SIZE);
	{
		bais->setEndianness(ENDIANNESS::BIG);

		uint8_t iByte = bais->readByte();
		uint16_t iShort = bais->readShort();
		uint32_t iInt = bais->readInt();
		uint64_t iLong = bais->readLong();

		bool b = true;
	}

	int8_t* pHeap = new int8_t[128];
	int32_t* pIntHeap = (int32_t*)pHeap;
	*(pIntHeap + 0) = 10;
	*(pIntHeap + 1) = 255;
	*(pIntHeap + 2) = 144385;
	*(pIntHeap + 3) = -1;

	exit(EXIT_SUCCESS);
}