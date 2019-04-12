#ifndef META_FUNCTION_H
#define META_FUNCTION_H

#include <string>
#include <assert.h>
#include "MetaType.h"
#include "MetaVariable.h"
#include "MetaUtility.h"

/**
 * Class to describe a Function with 
 * - return type
 * - arg count
 * - arg types
 */ 
class FunctionSignature{
public:
	template <typename Ret>
	FunctionSignature(Ret(*)())
		: m_retType(getMetaTypeByType<Ret>())
		, m_argCount(0)
		, m_argTypes(nullptr){
	}

	template <typename Ret, typename Arg0>
	FunctionSignature(Ret(*)(Arg0))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>() };
		m_argTypes = args;
		m_argCount = 1;
	}

	template <typename Ret, typename Arg0, typename Arg1>
	FunctionSignature(Ret(*)(Arg0, Arg1))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>() };
		m_argTypes = args;
		m_argCount = 2;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
	FunctionSignature(Ret(*)(Arg0, Arg1, Arg2))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>() };
		m_argTypes = args;
		m_argCount = 3;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	FunctionSignature(Ret(*)(Arg0, Arg1, Arg2, Arg3))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>() };
		m_argTypes = args;
		m_argCount = 4;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	FunctionSignature(Ret(*)(Arg0, Arg1, Arg2, Arg3, Arg4))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>() };
		m_argTypes = args;
		m_argCount = 5;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	FunctionSignature(Ret(*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>(), getMetaTypeByType<Arg5>() };
		m_argTypes = args;
		m_argCount = 6;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	FunctionSignature(Ret(*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>(), getMetaTypeByType<Arg5>(), getMetaTypeByType<Arg6>() };
		m_argTypes = args;
		m_argCount = 7;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	FunctionSignature(Ret(*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>(), getMetaTypeByType<Arg5>(), getMetaTypeByType<Arg6>(), getMetaTypeByType<Arg7>() };
		m_argTypes = args;
		m_argCount = 8;
	}

    //On x64 there is a difference between __stdcall is the same as __cdecl so compiling these functions would result in an error
    //however it is useful to define these to make opengl function signatures possible for example
#ifndef X_64
	template <typename Ret>
	FunctionSignature(Ret(__stdcall*)())
		: m_retType(getMetaTypeByType<Ret>())
		, m_argCount(0)
		, m_argTypes(nullptr){
	}

	template <typename Ret, typename Arg0>
	FunctionSignature(Ret(__stdcall*)(Arg0))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>() };
		m_argTypes = args;
		m_argCount = 1;
	}

	template <typename Ret, typename Arg0, typename Arg1>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>() };
		m_argTypes = args;
		m_argCount = 2;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1, Arg2))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>() };
		m_argTypes = args;
		m_argCount = 3;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1, Arg2, Arg3))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>() };
		m_argTypes = args;
		m_argCount = 4;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1, Arg2, Arg3, Arg4))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>() };
		m_argTypes = args;
		m_argCount = 5;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>(), getMetaTypeByType<Arg5>() };
		m_argTypes = args;
		m_argCount = 6;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>(), getMetaTypeByType<Arg5>(), getMetaTypeByType<Arg6>() };
		m_argTypes = args;
		m_argCount = 7;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	FunctionSignature(Ret(__stdcall*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7))
		: m_retType(getMetaTypeByType<Ret>()){
		static const MetaType* args[] = { getMetaTypeByType<Arg0>(), getMetaTypeByType<Arg1>(), getMetaTypeByType<Arg2>(), getMetaTypeByType<Arg3>(),
			getMetaTypeByType<Arg4>(), getMetaTypeByType<Arg5>(), getMetaTypeByType<Arg6>(), getMetaTypeByType<Arg7>() };
		m_argTypes = args;
		m_argCount = 8;
	}
#endif

	virtual const MetaType* RetType() const { return m_retType; }
	virtual const MetaType* ArgType(size_t _idx) const { if (_idx < m_argCount) { return m_argTypes[_idx]; }else{ assert(false && "FunctionSignature ArgType index out of range"); return nullptr; } }
	virtual size_t ArgCount() const { return m_argCount; }
protected:
	FunctionSignature() {};

	const MetaType* m_retType;
	const MetaType** m_argTypes;
	size_t m_argCount;
};

//===================================================================================================================================================================================
//function Wrappers to call the actual function

static void Apply(void(*function)(), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 0);
	function();
}

template <typename Ret>
static void Apply(Ret(*fun)(), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 0);
	Ret r = fun();
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Arg0>
static void Apply(void(*fun)(Arg0), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 1);
	Arg0 a0 = Cast<Arg0>(args[0]);
	fun(a0);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
}

template <typename Ret, typename Arg0>
static void Apply(Ret(*fun)(Arg0), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 1);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Ret r = fun(a0);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
}

template <typename Arg0, typename Arg1>
static void Apply(void(*fun)(Arg0, Arg1), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 2);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	fun(a0, a1);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
}

template <typename Ret, typename Arg0, typename Arg1>
static void Apply(Ret(*fun)(Arg0, Arg1), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 2);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Ret r = fun(a0, a1);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
}

template <typename Arg0, typename Arg1, typename Arg2>
static void Apply(void(*fun)(Arg0, Arg1, Arg2), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 3);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	fun(a0, a1, a2);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
static void Apply(Ret(*fun)(Arg0, Arg1, Arg2), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 3);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Ret r = fun(a0, a1, a2);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply(void(*fun)(Arg0, Arg1, Arg2, Arg3), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 4);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	fun(a0, a1, a2, a3);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply(Ret(*fun)(Arg0, Arg1, Arg2, Arg3), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 4);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Ret r = fun(a0, a1, a2, a3);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply(void(*fun)(Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 5);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	fun(a0, a1, a2, a3, a4);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply(Ret(*fun)(Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 5);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Ret r = fun(a0, a1, a2, a3, a4);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply(void(*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 6);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	fun(a0, a1, a2, a3, a4, a5);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply(Ret(*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 6);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	Ret r = fun(a0, a1, a2, a3, a4, a5);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply(void(*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 7);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	Arg6 a6 = Cast<Arg6>(args[6]);
	fun(a0, a1, a2, a3, a4, a5, a6);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
	getMetaTypeByType<Arg6>()->Dtor(&a6);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply(Ret(*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 7);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	Arg6 a6 = Cast<Arg6>(args[6]);
	Ret r = fun(a0, a1, a2, a3, a4, a5, a6);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
	getMetaTypeByType<Arg6>()->Dtor(&a6);
}


//===================================================__stdcall==========================================================
#ifndef X_64
static void Apply(void(__stdcall*fun)(), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 0);
	fun();
}

template <typename Ret>
static void Apply(Ret(__stdcall*fun)(), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 0);
	Ret r = fun();
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Arg0>
static void Apply(void(__stdcall*fun)(Arg0), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 1);
	Arg0 a0 = Cast<Arg0>(args[0]);
	fun(a0);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
}

template <typename Ret, typename Arg0>
static void Apply(Ret(__stdcall*fun)(Arg0), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 1);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Ret r = fun(a0);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
}

template <typename Arg0, typename Arg1>
static void Apply(void(__stdcall*fun)(Arg0, Arg1), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 2);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	fun(a0, a1);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
}

template <typename Ret, typename Arg0, typename Arg1>
static void Apply(Ret(__stdcall*fun)(Arg0, Arg1), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 2);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Ret r = fun(a0, a1);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
}

template <typename Arg0, typename Arg1, typename Arg2>
static void Apply(void(__stdcall*fun)(Arg0, Arg1, Arg2), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 3);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	fun(a0, a1, a2);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
static void Apply(Ret(__stdcall*fun)(Arg0, Arg1, Arg2), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 3);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Ret r = fun(a0, a1, a2);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply(void(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 4);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	fun(a0, a1, a2, a3);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply(Ret(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 4);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Ret r = fun(a0, a1, a2, a3);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply(void(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 5);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	fun(a0, a1, a2, a3, a4);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply(Ret(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 5);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Ret r = fun(a0, a1, a2, a3, a4);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply(void(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 6);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	fun(a0, a1, a2, a3, a4, a5);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply(Ret(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 6);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	Ret r = fun(a0, a1, a2, a3, a4, a5);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply(void(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 7);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	Arg6 a6 = Cast<Arg6>(args[6]);
	fun(a0, a1, a2, a3, a4, a5, a6);
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
	getMetaTypeByType<Arg6>()->Dtor(&a6);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply(Ret(__stdcall*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable *args, size_t argCount){
	assert(argCount == 7);
	Arg0 a0 = Cast<Arg0>(args[0]);
	Arg1 a1 = Cast<Arg1>(args[1]);
	Arg2 a2 = Cast<Arg2>(args[2]);
	Arg3 a3 = Cast<Arg3>(args[3]);
	Arg4 a4 = Cast<Arg4>(args[4]);
	Arg5 a5 = Cast<Arg5>(args[5]);
	Arg6 a6 = Cast<Arg6>(args[6]);
	Ret r = fun(a0, a1, a2, a3, a4, a5, a6);
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
	getMetaTypeByType<Arg0>()->Dtor(&a0);
	getMetaTypeByType<Arg1>()->Dtor(&a1);
	getMetaTypeByType<Arg2>()->Dtor(&a2);
	getMetaTypeByType<Arg3>()->Dtor(&a3);
	getMetaTypeByType<Arg4>()->Dtor(&a4);
	getMetaTypeByType<Arg5>()->Dtor(&a5);
	getMetaTypeByType<Arg6>()->Dtor(&a6);
}
#endif


template <typename Fun>
static void ApplyWrapperFunction(void (*fun) (void), Variable ret, Variable *args, size_t argCount)
{
	Apply((Fun)fun, ret, args, argCount);
}

struct MetaFunction : public AutoLister<MetaFunction>{
	template <typename Fun>
	MetaFunction(const char* name, Fun fun)
		: m_name(name)
		, m_sig(fun)
		, m_fun((void(*)())fun)
		, m_applyWrapper(ApplyWrapperFunction<Fun>){
	}

	const char* name() const { return m_name; }
	const MetaType* retType() const { return m_sig.RetType(); }
	const MetaType* argType(size_t _idx) const { return m_sig.ArgType(_idx); }
	size_t argCount() const { return m_sig.ArgCount(); }

	void apply(Variable ret, Variable *args, size_t argCount) const{
		m_applyWrapper((void(*)())m_fun, ret, args, argCount);
	}

	friend std::ostream& operator<<(std::ostream& os, const MetaFunction& _str);
private:
	const char* m_name;
	FunctionSignature m_sig;
	void(* m_fun)();
	void(* m_applyWrapper) (void(*fun)(), Variable, Variable*, size_t);
};

static std::ostream& operator<<(std::ostream& os, const MetaFunction& _fun){
	os << _fun.retType()->name() << " " << _fun.m_name << "(";
	for (size_t i = 0; i < _fun.argCount() - 1; i++){
		os << _fun.argType(i)->name() << ", ";
	}
	if (_fun.argCount() >= 1)
		os << _fun.argType(_fun.argCount() - 1)->name() << ");";
	return os;
}

#define META_REGISTER_FUN(fun, name) \
	MetaFunction g_MetaFunction_##fun(name, fun)

MetaFunction* GetFunctionByName(const char* sFunctionName)
{
	for (MetaFunction* mf = MetaFunction::Head(); mf; mf = mf->Next())
	{
		std::string sFunName = mf->name();
		if (sFunName == sFunctionName)
			return mf;
	}

	return nullptr;
}

#endif //META_FUNCTION_H
