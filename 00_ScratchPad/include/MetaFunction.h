#pragma once
#include "AutoLister.h"
#include "FunctionSignature.h"
#include "MetaType.h"
#include "Variable.h"
#include "Apply.h"

// We need a templated wrapper as we to cast the "void function pointer - void(*)()" with the "actual function pointer - <Fun>".
template <typename Fun>
void ApplyWrapper(void (*fun) (), Variable ret, Variable* args, int argCount)
{
	Apply( (Fun)fun, ret, args, argCount );
}

struct MetaFunction : public AutoLister<MetaFunction>
{
	public:
		template <typename Fun>
		MetaFunction(const char* name, Fun fun)
		: m_name(name)
		, m_sig(fun)
		, m_fun( (void(*)()) fun)
		, m_applyWraper(ApplyWrapper<Fun>)
		{}

		const char*	Name() const { return m_name; }

		const MetaType*	RetType() const { return m_sig.RetType(); }
		const MetaType*	ArgType(int idx) const { return m_sig.ArgType(idx); }
		int ArgCount() const { return m_sig.ArgCount(); }

		void Apply(Variable ret, Variable* args, int argCount) const 
		{
			m_applyWraper(m_fun, ret, args, argCount);
		}
	private:
		const char* m_name;
		FunctionSignature m_sig;
		void (*m_fun) ();			// void function pointer.
		void (*m_applyWraper) ( void (*fun) (), Variable, Variable*, int );
};

#define META_REGISTER_FUN(f) \
	MetaFunction g_metaFunction_##f(#f, f)