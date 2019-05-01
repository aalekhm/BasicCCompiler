#pragma once
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <iostream>

// <<<<<<<<<<<<<<<<<<<<<<<<<< 1. >>>>>>>>>>>>>>>>>>>>>>>>
struct MetaType
{
	virtual const char* name() const = 0;
	virtual size_t		sizeOf() const = 0;

	virtual double		toNumber(void* pAddress) const = 0;
	virtual const char*	toString(void* pAddress) const = 0;
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const = 0;

	virtual void*		newAlloc() const = 0;
	virtual void		deleteAlloc(void* pAddress) const = 0;
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const = 0;
};

struct Void_MetaType : public MetaType
{
	virtual const char* name() const { return "void"; }
	virtual size_t		sizeOf() const { return 0; }

	virtual double		toNumber(void* pAddress) const { return (double)0; }
	virtual const char*	toString(void* pAddress) const { return ""; }
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const { };

	virtual void*		newAlloc() const { return nullptr; };
	virtual void		deleteAlloc(void* pAddress) const { };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_voidMetaType;

struct Int8_MetaType : public MetaType
{
	virtual const char* name() const { return "int8_t"; }
	virtual size_t		sizeOf() const { return sizeof(int8_t); }

	virtual double		toNumber(void* pAddress) const { return (double)*(int8_t*)pAddress; }
	virtual const char*	toString(void* pAddress) const 
	{ 
		static char sBuf[64];
		sprintf_s(sBuf, "%d", *(int8_t*)pAddress);

		return sBuf; 
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const 
	{
		*(int8_t*)pTo = (int8_t)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_int8MetaType;

struct Int16_MetaType : public MetaType
{
	virtual const char* name() const { return "int16_t"; }
	virtual size_t		sizeOf() const { return sizeof(int16_t); }

	virtual double		toNumber(void* pAddress) const { return (double)*(int16_t*)pAddress; }
	virtual const char*	toString(void* pAddress) const
	{
		static char sBuf[64];
		sprintf_s(sBuf, "%d", *(int16_t*)pAddress);

		return sBuf;
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(int16_t*)pTo = (int16_t)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_int16MetaType;

struct Int32_MetaType : public MetaType
{
	virtual const char* name() const { return "int32_t"; }
	virtual size_t		sizeOf() const { return sizeof(int32_t); }

	virtual double		toNumber(void* pAddress) const { return (double)*(int32_t*)pAddress; }
	virtual const char*	toString(void* pAddress) const
	{
		static char sBuf[64];
		sprintf_s(sBuf, "%d", *(int32_t*)pAddress);

		return sBuf;
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(int32_t*)pTo = (int32_t)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_int32MetaType;

struct UInt32_MetaType : public MetaType
{
	virtual const char* name() const { return "uint32_t"; }
	virtual size_t		sizeOf() const { return sizeof(uint32_t); }

	virtual double		toNumber(void* pAddress) const { return (double)*(uint32_t*)pAddress; }
	virtual const char*	toString(void* pAddress) const
	{
		static char sBuf[64];
		sprintf_s(sBuf, "%u", *(uint32_t*)pAddress);

		return sBuf;
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(uint32_t*)pTo = (uint32_t)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_uint32MetaType; 

struct Int64_MetaType : public MetaType
{
	virtual const char* name() const { return "int64_t"; }
	virtual size_t		sizeOf() const { return sizeof(int64_t); }

	virtual double		toNumber(void* pAddress) const { return (double)*(int64_t*)pAddress; }
	virtual const char*	toString(void* pAddress) const
	{
		static char sBuf[64];
		sprintf_s(sBuf, "%lld", *(int64_t*)pAddress);

		return sBuf;
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(int64_t*)pTo = (int64_t)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_int64MetaType;

struct Double_MetaType : public MetaType
{
	virtual const char* name() const { return "double"; }
	virtual size_t		sizeOf() const { return sizeof(double); }

	virtual double		toNumber(void* pAddress) const { return *(double*)pAddress; }
	virtual const char*	toString(void* pAddress) const
	{
		static char sBuf[64];
		sprintf_s(sBuf, "%f", *(double*)pAddress);

		return sBuf;
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(double*)pTo = (double)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_doubleMetaType;

struct Float_MetaType : public MetaType
{
	virtual const char* name() const { return "float"; }
	virtual size_t		sizeOf() const { return sizeof(float); }

	virtual double		toNumber(void* pAddress) const { return (double)*(float*)pAddress; }
	virtual const char*	toString(void* pAddress) const
	{
		static char sBuf[64];
		sprintf_s(sBuf, "%f", *(float*)pAddress);

		return sBuf;
	}
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(float*)pTo = (float)metaType.toNumber(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_floatMetaType;

struct CharStar_MetaType : public MetaType
{
	virtual const char* name() const { return "char*"; }
	virtual size_t		sizeOf() const { return sizeof(char*); }

	virtual double		toNumber(void* pAddress) const { return atof(*(char**)pAddress); }
	virtual const char*	toString(void* pAddress) const { return *(const char**)pAddress; }
	virtual void		cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(const char**)pTo = metaType.toString(pFrom);
	};

	virtual void*		newAlloc() const { return ::malloc(sizeOf()); };
	virtual void		deleteAlloc(void* pAddress) const { delete pAddress; };
	virtual void		setValueFrom(void* pDstAddress, void* pSrcAddress) const
	{
		size_t iSize = sizeOf();
		memcpy_s(pDstAddress, iSize, pSrcAddress, iSize);
	};
} g_charStarMetaType;

const MetaType&		GetMetaType(void) {			return g_voidMetaType; }
const MetaType&		GetMetaType(int8_t) {		return g_int8MetaType; }
const MetaType&		GetMetaType(int16_t) {		return g_int16MetaType; }
const MetaType&		GetMetaType(int32_t) {		return g_int32MetaType; }
const MetaType&		GetMetaType(uint32_t) {		return g_uint32MetaType; }
const MetaType&		GetMetaType(int64_t) {		return g_int64MetaType; }
const MetaType&		GetMetaType(double) {		return g_doubleMetaType; }
const MetaType&		GetMetaType(float) {		return g_floatMetaType; }
const MetaType&		GetMetaType(const char*) {	return g_charStarMetaType; }

template <typename T>	const MetaType&		GetMetaTypeByType();
// Specialization
template <>				const MetaType&		GetMetaTypeByType<void>()			{ return g_voidMetaType; }
template <>				const MetaType&		GetMetaTypeByType<int8_t>()			{ return g_int8MetaType; }
template <>				const MetaType&		GetMetaTypeByType<int16_t>()		{ return g_int16MetaType; }
template <>				const MetaType&		GetMetaTypeByType<int32_t>()		{ return g_int32MetaType; }
template <>				const MetaType&		GetMetaTypeByType<uint32_t>()		{ return g_uint32MetaType; }
template <>				const MetaType&		GetMetaTypeByType<int64_t>()		{ return g_int64MetaType; }
template <>				const MetaType&		GetMetaTypeByType<double>()			{ return g_doubleMetaType; }
template <>				const MetaType&		GetMetaTypeByType<float>()			{ return g_floatMetaType; }
template <>				const MetaType&		GetMetaTypeByType<const char*>()	{ return g_charStarMetaType; }

void PrintMetaTypeInfo(const MetaType& metaType)
{
	std::cout	<< "name : " << metaType.name()
				<< "size : " << metaType.sizeOf()
				<< std::endl;
}

template <typename To>
To Cast(void* pFrom, const MetaType& metaType)
{
	To to = {};
	GetMetaType(to).cast(&to, pFrom, metaType);

	return to;
}