#pragma once
#include "ByteArrayStream.h"

class ByteArrayInputStream : public ByteArrayStream
{
	public:
		ByteArrayInputStream(int8_t* pByteArray, uint64_t iArraySize);

		int8_t				readByte();
		int16_t				readShort();
		int32_t				readInt();
		int64_t				readLong();

		int8_t				readByteAtPos(int64_t iOffset);
		int16_t				readShortAtPos(int64_t iOffset);
		int32_t				readIntAtPos(int64_t iOffset);
		int64_t				readLongAtPos(int64_t iOffset);
	protected:
	private:
};