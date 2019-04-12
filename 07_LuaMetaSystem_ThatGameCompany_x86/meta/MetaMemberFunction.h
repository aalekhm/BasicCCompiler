#ifndef META_MEMBER_FUNCTION
#define META_MEMBER_FUNCTION

#include "MetaFunction.h"


template <typename Cls>
class MemberFunctionSignature : public FunctionSignature{
public:
	template <typename Ret>
	MemberFunctionSignature(Ret(Cls::*)())
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		m_argCount = 0;
		m_argTypes = nullptr;
	}

	template <typename Ret>
	MemberFunctionSignature(Ret(Cls::*)() const)
		: m_constFunc(true){
		m_retType = getMetaTypeByType< Ret >();
		m_argCount = 0;
		m_argTypes = nullptr;
	}

	template <typename Ret, typename Arg0>
	MemberFunctionSignature(Ret(Cls::*)(Arg0))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >() };
		m_argTypes = args;
		m_argCount = 1;
	}

	template <typename Ret, typename Arg0>
	MemberFunctionSignature(Ret(Cls::*)(Arg0) const)
		: m_constFunc(true){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >() };
		m_argTypes = args;
		m_argCount = 1;
	}

	template <typename Ret, typename Arg0, typename Arg1>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >() };
		m_argTypes = args;
		m_argCount = 2;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >() };
		m_argTypes = args;
		m_argCount = 3;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >() };
		m_argTypes = args;
		m_argCount = 4;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >() };
		m_argTypes = args;
		m_argCount = 5;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >(), getMetaTypeByType< Arg5 >() };
		m_argTypes = args;
		m_argCount = 6;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >(), getMetaTypeByType< Arg5 >(), getMetaTypeByType< Arg6 >() };
		m_argTypes = args;
		m_argCount = 7;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >(), getMetaTypeByType< Arg5 >(), getMetaTypeByType< Arg6 >(), getMetaTypeByType< Arg7 >() };
		m_argTypes = args;
		m_argCount = 8;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >(), getMetaTypeByType< Arg5 >(), getMetaTypeByType< Arg6 >(), getMetaTypeByType< Arg7 >(), getMetaTypeByType< Arg8 >()};
		m_argTypes = args;
		m_argCount = 9;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >(), getMetaTypeByType< Arg5 >(), getMetaTypeByType< Arg6 >(), getMetaTypeByType< Arg7 >(), getMetaTypeByType< Arg8 >(),
			getMetaTypeByType< Arg9 >()};
		m_argTypes = args;
		m_argCount = 10;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	MemberFunctionSignature(Ret(Cls::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10))
		: m_constFunc(false){
		m_retType = getMetaTypeByType< Ret >();
		static const MetaType* args[] = { getMetaTypeByType< Arg0 >(), getMetaTypeByType< Arg1 >(), getMetaTypeByType< Arg2 >(), getMetaTypeByType< Arg3 >(),
			getMetaTypeByType< Arg4 >(), getMetaTypeByType< Arg5 >(), getMetaTypeByType< Arg6 >(), getMetaTypeByType< Arg7 >(), getMetaTypeByType< Arg8 >(),
			getMetaTypeByType< Arg9 >(), getMetaTypeByType< Arg10 >()};
		m_argTypes = args;
		m_argCount = 11;
	}

	bool isConst() const { return m_constFunc; }


private:
	bool m_constFunc;
};

template <typename Cls>
void Apply(void (Cls::*fun)(), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 0);
	(cls->*fun)();
}

template <typename Cls, typename Ret>
void Apply(Ret(Cls::*fun)(), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 0);
	Ret r = (cls->*fun)();
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0>
void Apply(void (Cls::*fun)(Arg0), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 1);
	(cls->*fun)(Cast<Arg0>(args[0]));
}

template <typename Cls, typename Ret, typename Arg0>
void Apply(Ret(Cls::*fun)(Arg0), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 1);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1>
void Apply(void (Cls::*fun)(Arg0, Arg1), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 2);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1>
void Apply(Ret(Cls::*fun)(Arg0, Arg1), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 2);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1, typename Arg2>
void Apply(void (Cls::*fun)(Arg0, Arg1, Arg2), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 3);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1, typename Arg2>
void Apply(Ret(Cls::*fun)(Arg0, Arg1, Arg2), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 3);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Apply(void (Cls::*fun)(Arg0, Arg1, Arg2, Arg3), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 4);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Apply(Ret(Cls::*fun)(Arg0, Arg1, Arg2, Arg3), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 4);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Apply(void (Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 5);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Apply(Ret(Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 5);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Apply(void (Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 6);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]),
		Cast<Arg5>(args[5]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void Apply(Ret(Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 6);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]),
		Cast<Arg5>(args[5]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Apply(void (Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 7);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]),
		Cast<Arg5>(args[5]), Cast<Arg6>(args[6]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void Apply(Ret(Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 7);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]),
		Cast<Arg5>(args[5]), Cast<Arg6>(args[6]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Apply(void (Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 8);
	(cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]),
		Cast<Arg5>(args[5]), Cast<Arg6>(args[6]), Cast<Arg7>(args[7]));
}

template <typename Cls, typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void Apply(Ret(Cls::*fun)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Cls* cls, Variable ret, Variable* args, size_t argCount){
	assert(argCount == 8);
	Ret r = (cls->*fun)(Cast<Arg0>(args[0]), Cast<Arg1>(args[1]), Cast<Arg2>(args[2]), Cast<Arg3>(args[3]), Cast<Arg4>(args[4]),
		Cast<Arg5>(args[5]), Cast<Arg6>(args[6]), Cast<Arg7>(args[7]));
	ret.m_type->cast(ret.m_address, &r, getMetaTypeByType<Ret>());
}

template <typename Cls, typename Fun>
void ApplyWrapper(void(Cls::*fun)(), Cls* cls, Variable Ret, Variable* args, size_t argCount){
	Apply((Fun)fun, cls, Ret, args, argCount);
}

template <typename Cls>
class MetaMemberFunction{
public:
	template <typename Fun>
	MetaMemberFunction(const char* _name, Fun _fun)
		: m_sig(_fun)
		, m_name(_name)
		, m_applyWrapper(ApplyWrapper<Cls, Fun>){}

	virtual bool isConst() const { return m_sig.isConst(); };
	virtual const char* name() const { return m_name; }
	virtual const MetaType* retType() const { return m_sig.RetType(); }
	virtual const MetaType* argType(unsigned _idx) const { return m_sig.ArgType(_idx); }
	virtual size_t argCount() const { return m_sig.ArgCount(); }

	virtual void apply(void *cls, Variable ret, Variable* args, size_t argCount) const = 0;
protected:
	void(*m_applyWrapper) (void(Cls::*fun)(), Cls* cls, Variable, Variable*, size_t);
	const char* m_name;
	MemberFunctionSignature<Cls> m_sig;
};

template <typename Cls>
class NonConstMetaMemberFunction : public MetaMemberFunction<Cls>{
public:
	template <typename Ret, typename ...Args>
	NonConstMetaMemberFunction(const char* _name, Ret(Cls::*_fun)(Args...))
		: MetaMemberFunction<Cls>(_name, _fun)
		, m_fun((void (Cls::*) ())_fun){

	}

	template <typename BaseCls>
	NonConstMetaMemberFunction(const NonConstMetaMemberFunction<BaseCls>* _baseClassFunction)
		: MetaMemberFunction<Cls>(_baseClassFunction->m_name, _baseClassFunction->m_fun)
		, m_fun((void (Cls::*) ())_baseClassFunction->m_fun){

	}

	NonConstMetaMemberFunction(const NonConstMetaMemberFunction<Cls>& _fun)
		: m_name(_fun.m_name)
		, m_sig(_fun.m_sig)
		, m_fun(_fun.m_sig){}


	void apply(void *cls, Variable ret, Variable* args, size_t argCount) const{
		m_applyWrapper(m_fun, reinterpret_cast<Cls*>(cls), ret, args, argCount);
	}
private:
	void (Cls::*m_fun) ();
};

template <typename Cls>
class ConstMetaMemberFunction : public MetaMemberFunction<Cls>{
public:
	template <typename Ret, typename ...Args>
	ConstMetaMemberFunction(const char* _name, Ret(Cls::*_fun)(Args...)const)
		: MetaMemberFunction<Cls>(_name, _fun)
		, m_fun(reinterpret_cast< void(Cls::*)()const >(_fun)){
	}

	void apply(void *cls, Variable ret, Variable* args, size_t argCount) const{
		m_applyWrapper(const_cast<void(Cls::*)()>(m_fun), reinterpret_cast<Cls*>(cls), ret, args, argCount);
	}

private:
	void (Cls::*m_fun) () const;
};

#endif //META_MEMBER_FUNCTION
