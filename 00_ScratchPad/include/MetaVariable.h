#pragma once
#include "MetaType.h"
#include "AutoLister.h"

#define META_REGISTER_VAR(v) \
	MetaVariable g_MetaVariable_##v(#v, &v)		// Eg:
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
	MetaVariable(const char* name_, Var* varAddress)
		: name(name_)
		, address(varAddress)
		, type(GetMetaType(*varAddress))
	{

	}
};

void MetaPrintVariables(std::ostream& os)
{
	for (MetaVariable* mv = MetaVariable::Head(); mv; mv = mv->Next())
	{
		std::cout << mv->type.Name() << " " << mv->name << " = " << mv->type.ToString(mv->address) << std::endl;
	}
}
