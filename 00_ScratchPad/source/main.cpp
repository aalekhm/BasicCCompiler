#include <iostream>
#include <vector>
#include <assert.h>

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

int main_0(int argc, char* argv[])
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

class GroundFloor
{
public:
	virtual void virtualGroundFn0() { };
	virtual void virtualGroundFn1() { };

	virtual void virtualGroundFn2() { };

	int32_t m_iInt0 = 100;
	int32_t m_iInt1 = 101;
};

class FirstFloor : public GroundFloor
{
public:
	virtual void virtualGroundFn0() { };
	virtual void virtualGroundFn1() { };

	virtual void virtualFirstFn0() { m_iInt0 = 202; };
	virtual void virtualFirstFn1() { m_iInt1 = 203; };

	int32_t m_iInt0 = 200;
	int32_t m_iInt1 = 201;
};

class SecondFloor : public FirstFloor
{
public:
	virtual void virtualGroundFn0() { 
		GroundFloor::virtualGroundFn0();
	};
	virtual void virtualGroundFn1() { };

	virtual void virtualFirstFn0() { m_iInt0 = 302; };
	virtual void virtualFirstFn1() { m_iInt1 = 303; };

	virtual void virtualSecondFn0() { };
	virtual void virtualSecondFn1() { };

	int32_t m_iInt0 = 300;
	int32_t m_iInt1 = 301;
};

class Shape
{
public:
	virtual double area() const { return 1.0; };
	virtual double perimeter() const { return 1.0; };

	virtual double dummy() const { return 1.0; };

	int32_t		m_iCoordinates = 1;
	int32_t		m_iOutline = 2;
	int32_t		m_iFill = 3;

	int32_t		m_iID = 1;
};

class Circle : public Shape
{
public:
	virtual double area() const { return 1.0; };
	virtual double perimeter() const { return 2.0; };
	virtual double diameter() const { return 2.0; };

	virtual double radius() const { return 1.0; printf("Circle::radius"); };

	int32_t		m_iRadius = 4;
	int32_t		m_iID = 3;
};

class Rectangle : public Shape
{
public:
	virtual double area() const { return 1.0; };
	//virtual double perimeter() const { return 1.0; };

	int32_t		m_iHeight = 5;
	int32_t		m_iWidth = 6;

	int32_t		m_iID = 4;
};

class Ellipse : public Circle
{
public:
	virtual double area() const { return 1.0; };
	virtual double perimeter() const { return 1.0; };
	virtual double dummy() const { return 1.0; };

	virtual double radius() const { return 2.0; printf("Ellipse::radius"); };

	virtual double dummyEllipse() const { return 1.0; };

	int32_t		m_iMinorAxis = 7;
	int32_t		m_iMajorAxis = 8;

	int32_t		m_iID = 5;
};

class IInterface1
{
public:
	virtual void interface1_Func1() = 0;
};

class IInterface2
{
public:
	virtual void interface2_Func1() = 0;
};

class InterfaceImpl : virtual IInterface1
{
public:
	virtual void interfaceImpl_Func1() {};
	virtual void interface1_Func1() {};

	int32_t m_iVar1 = 10;
};

int main_1(int argc, char* argv[])
{
	GroundFloor* gf = new GroundFloor();
	FirstFloor* ff = new FirstFloor();
	SecondFloor* sf = new SecondFloor();
	sf->virtualSecondFn0();

	GroundFloor* gff = new SecondFloor();
	gff->virtualGroundFn0();
	gff->GroundFloor::virtualGroundFn0();
	
	Shape* s = new Shape();
	Circle* c = new Circle();
	Rectangle* r = new Rectangle();
		r->area();
		r->perimeter();
	Ellipse* e = new Ellipse();
	e->radius();
	e->area();
	e->diameter();
	e->Shape::area();

	printf("%d, %d, %d, %d", s->m_iID, c->m_iID, r->m_iID, e->m_iID);
	
	Shape* se = new Ellipse();

	InterfaceImpl* ii = new InterfaceImpl();

	exit(EXIT_SUCCESS);
}

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

void* xrealloc(void* ptr, size_t num_bytes)
{
	ptr = realloc(ptr, num_bytes);
	if (!ptr)
	{
		perror("xrealloc failed!");
		exit(1);
	}

	return ptr;
}

void* xmalloc(size_t num_bytes)
{
	void* ptr = malloc(num_bytes);
	if (!ptr)
	{
		perror("xmalloc failed!");
		exit(1);
	}

	return ptr;
}

typedef struct BufHdr
{
	size_t	length;
	size_t	capacity;

	char	buf[0];
} BufHdr;

#define buf__hdr(b)			( (BufHdr*)( (char*)(b) - offsetof(BufHdr, buf) ) )
#define buf__fits(b, n)		( buf_len(b) + (n) <= buf_cap(b) )

#define buf__fit(b, n)		(	buf__fits( (b), (n) ) \
									? 0  \
									: ( (b) = buf__grow(	(b), \
															buf_len(b) + (n), sizeof((*b)) \
														) \
									) \
							) \

#define buf_len(b)			( (b) ? buf__hdr(b)->length : 0 )
#define buf_cap(b)			( (b) ? buf__hdr(b)->capacity : 0 )
#define buf_push(b, x)		( buf__fit((b), 1), (b)[buf__hdr(b)->length++] = (x) )
#define buf_free(b)			( (b) ? ( free(buf__hdr(b)), (b) = NULL ) : 0 )

template<class T>
T* buf__grow(T* buf, size_t new_len, size_t elem_size)
{
	size_t new_capacity = MAX(1 + 2 * buf_cap(buf), new_len);
	assert(new_len <= new_capacity);
	size_t new_size = offsetof(BufHdr, buf) + new_capacity * elem_size;
	BufHdr* new_hdr;
	if (buf)
	{
		new_hdr = (BufHdr*)xrealloc(buf__hdr(buf), new_size);
	}
	else
	{
		new_hdr = (BufHdr*)xmalloc(new_size);
		new_hdr->length = 0;
	}

	new_hdr->capacity = new_capacity;
	return (T*)new_hdr->buf;
}

void buf_test()
{
	int* asdf = NULL;
	assert(buf_len(asdf) == 0);
	enum { N = 1024 };
	for (int i = 0; i < N; i++)
	{
		buf_push(asdf, i);
	}

	assert(buf_len(asdf) == N);
	for (int i = 0; i < N; i++)
	{
		assert(asdf[i] == i);
	}

	buf_free(asdf);
	assert(asdf == NULL);
	assert(buf_len(asdf) == 0);
}

int main_2(int argc, char* argv[])
{
	buf_test();
	exit(EXIT_SUCCESS);
}

struct Example
{
	int32_t iInt1 = 11;
	int32_t iInt2 = {};
	int8_t iInt3 = {};
};

int main(int argc, char* argv[])
{
	Example eg;
	exit(EXIT_SUCCESS);
}