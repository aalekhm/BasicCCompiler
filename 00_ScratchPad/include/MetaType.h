#pragma once
#include <iostream>

struct MetaType
{
	virtual const char* Name() const = 0;
	virtual size_t SizeOf() const = 0;

	virtual double ToNumber(void* pAddress) const = 0;
	virtual const char* ToString(void* pAddress) const = 0;
	virtual void Cast(void* pTo, void* pFrom, const MetaType& fromType) const = 0;
};

struct VoidMetaType : public MetaType
{
	virtual const char* Name() const { return "void"; }
	virtual size_t SizeOf() const { return 0; }

	virtual double ToNumber(void* pAddress) const { return 0; };
	virtual const char* ToString(void* pAddress) const { return ""; }

	virtual void Cast(void* pTo, void* pFrom, const MetaType& metaType) const { }
} g_voidMetaType; 

struct IntMetaType : public MetaType
{
	virtual const char* Name() const { return "int"; }
	virtual size_t SizeOf() const { return sizeof(int); }
	
	virtual double ToNumber(void* pAddress) const { return *(int*)pAddress; };
	virtual const char* ToString(void* pAddress) const
	{
		static char buf[64];
		sprintf_s(buf, "%d", *(int*)pAddress);

		return buf;
	}
	virtual void Cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(int*)pTo = (int)metaType.ToNumber(pFrom);
	}
} g_intMetaType;

struct ShortMetaType : public MetaType
{
	virtual const char* Name() const { return "short"; }
	virtual size_t SizeOf() const { return sizeof(short); }

	virtual double ToNumber(void* pAddress) const { return *(short*)pAddress; };
	virtual const char* ToString(void* pAddress) const
	{
		static char buf[64];
		sprintf_s(buf, "%d", *(short*)pAddress);

		return buf;
	}
	virtual void Cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(short*)pTo = (short)metaType.ToNumber(pFrom);
	}
} g_shortMetaType;

struct DoubleMetaType : public MetaType
{
	virtual const char* Name() const { return "double"; }
	virtual size_t SizeOf() const { return sizeof(double); }

	virtual double ToNumber(void* pAddress) const { return *(double*)pAddress; };
	virtual const char* ToString(void* pAddress) const
	{
		static char buf[64];
		sprintf_s(buf, "%f", *(double*)pAddress);

		return buf;
	}
	virtual void Cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(double*)pTo = (double)metaType.ToNumber(pFrom);
	}
} g_doubleMetaType;

struct FloatMetaType : public MetaType
{
	virtual const char* Name() const { return "float"; }
	virtual size_t SizeOf() const { return sizeof(float); }

	virtual double ToNumber(void* pAddress) const { return *(float*)pAddress; };
	virtual const char* ToString(void* pAddress) const
	{
		static char buf[64];
		sprintf_s(buf, "%f", *(float*)pAddress);

		return buf;
	}
	virtual void Cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(float*)pTo = (float)metaType.ToNumber(pFrom);
	}
} g_floatMetaType;

struct CharStarMetaType : public MetaType
{
	virtual const char* Name() const { return "char*"; }
	virtual size_t SizeOf() const { return sizeof(char*); }

	virtual double ToNumber(void* pAddress) const { return atof(*(char**)pAddress); };
	virtual const char* ToString(void* pAddress) const { return *(const char**)pAddress; }
	virtual void Cast(void* pTo, void* pFrom, const MetaType& metaType) const
	{
		*(const char**)pTo = metaType.ToString(pFrom);
	}
} g_charStarMetaType;

// Template
template <typename T>	const MetaType& GetMetaTypeByType();
// Specializations
template <>				const MetaType& GetMetaTypeByType<void>() {			return g_voidMetaType; }
template <>				const MetaType& GetMetaTypeByType<int>() {			return g_intMetaType; }
template <>				const MetaType& GetMetaTypeByType<short>() {		return g_shortMetaType; }
template <>				const MetaType& GetMetaTypeByType<double>() {		return g_doubleMetaType; }
template <>				const MetaType& GetMetaTypeByType<float>() {		return g_floatMetaType; }
template <>				const MetaType& GetMetaTypeByType<const char*>() {	return g_charStarMetaType; }

const MetaType& GetMetaType(void) { return g_voidMetaType; }
const MetaType& GetMetaType(int) { return g_intMetaType; }
const MetaType& GetMetaType(short) { return g_shortMetaType; }
const MetaType& GetMetaType(double) { return g_doubleMetaType; }
const MetaType& GetMetaType(float) { return g_floatMetaType; }
const MetaType& GetMetaType(const char*) { return g_charStarMetaType; }

void PrintMetaTypeInfo(const MetaType& type)
{
	std::cout	<< "name: " << type.Name() << "\n"
				<< "size: " << type.SizeOf() << std::endl;
}

template <typename To>
To Cast(void* pFrom, const MetaType& mv)
{
	To to = {};
	GetMetaType(to).Cast(&to, pFrom, mv);

	return to;
}