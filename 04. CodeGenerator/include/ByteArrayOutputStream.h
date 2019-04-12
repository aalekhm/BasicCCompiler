#pragma once
#include "ByteArrayStream.h"

class ByteArrayOutputStream : public ByteArrayStream
{
	public:
		ByteArrayOutputStream(int8_t* pByteArray, uint64_t iArraySize);

		void			writeByte(int8_t iByte);
		void			writeShort(int16_t iShort);
		void			writeInt(int32_t iInt);
		void			writeLong(int64_t iLong);
		void			writeFloat(float fFloat);

		void			writeByteAtPos(int8_t iByte, uint32_t iCurrentPos);
		void			writeShortAtPos(int16_t iShort, uint32_t iCurrentPos);
		void			writeIntAtPos(int32_t iInt, uint32_t iCurrentPos);
		void			writeLongAtPos(int64_t iLong, uint32_t iCurrentPos);
		void			writeFloatAtPos(float iFloat, uint32_t iCurrentPos);
	protected:
	private:
};