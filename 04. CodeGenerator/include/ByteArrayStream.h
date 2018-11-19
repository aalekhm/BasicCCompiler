#pragma once
#include <stdint.h>

enum class ENDIANNESS
{
	LITTLE,
	BIG
};

class ByteArrayStream
{
	public:
		ByteArrayStream(int8_t* pByteArray, uint64_t iArraySize);

		void			setEndianness(ENDIANNESS eENDIANNESS);

		uint64_t		getCurrentOffset();
		void			setCurrentOffset(uint64_t iNewOffset);

		void			reset();
	protected:
		int8_t*			m_pByteArray;
		uint64_t		m_iCurrentPos;
		uint64_t		m_iArraySize;
		ENDIANNESS		m_eEndianness;
	private:
};