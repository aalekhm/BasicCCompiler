#pragma once

#include "MetaType.h"
#include "AutoLister.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<< 2. >>>>>>>>>>>>>>>>>>>>>>>>
#define META_REGISTER_VAR(x) \
		MetaVariable g_MetaVariable##x(#x, &x)	// Eg:
												//		int i = 10;
												//		META_REGISTER_VAR(i);
												//		expands to:
												//		MetaVariable g_MetaVariable_i("i", &i);

struct MetaVariable : public AutoLister<MetaVariable>
{
	const char* const	name;
	void* const			address;
	const MetaType&		type;

	template <typename Var>
	MetaVariable(const char* name_, Var* address_)
	: name(name_)
	, address(address_)
	, type(GetMetaType(*address_))
	{}
};

void MetaPrintVariables(std::ostream& os)
{
	for (MetaVariable* mv = MetaVariable::Head(); mv; mv = mv->Next())
	{
		std::cout << mv->type.name() << " " << mv->name << " = " << mv->type.toString(mv->address) << std::endl;
	}
}
