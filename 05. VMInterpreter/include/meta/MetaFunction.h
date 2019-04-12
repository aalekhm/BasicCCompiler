#pragma once
#include "AutoLister.h"
#include "FunctionSignature.h"
#include "Variable.h"
#include "Apply.h"

// We need a templated wrapper as we to cast the "void function pointer - void(*)()" with the "actual function pointer - <Fun>".
template <typename Fun>
void ApplyWrapper( void (*fun) (), Variable ret, Variable* args, int iArgCount)
{
	Apply( (Fun)fun, ret, args, iArgCount );
}

// <<<<<<<<<<<<<<<<<<<<<<<<<< 7. >>>>>>>>>>>>>>>>>>>>>>>>
struct MetaFunction : public AutoLister<MetaFunction>
{
	public:
		template <typename Fun>
		MetaFunction(const char* sName, Fun fun)
		: m_sName(sName)
		, m_Sig(fun)
		, m_Fun( (void(*)()) fun)
		, m_ApplyWrapper(ApplyWrapper<Fun>)
		{}

		const char*			getName() { return m_sName; }
		const MetaType*		getRetType() const { return m_Sig.getRetType(); }
		const MetaType*		getArgType(int idx) { return m_Sig.getArgType(idx); }
		int					getArgCount() { return m_Sig.getArgCount(); }

		void				call(Variable ret, Variable* args, int iArgCount)
		{
			m_ApplyWrapper(m_Fun, ret, args, iArgCount);
		}
	private:
		const char*			m_sName;
		FunctionSignature	m_Sig;
		void				(*m_Fun)();		// void function pointer.
		void				(*m_ApplyWrapper)( void (*fun) (), Variable vRet, Variable* vArgs, int iArgCount);
};

void MetaPrintFunctions(std::ostream& os)
{
	for (MetaFunction* mf = MetaFunction::Head(); mf; mf = mf->Next())
	{
		std::cout << mf->getRetType()->name() << " " << mf->getName() << "(";
		for (int i = 0; i < mf->getArgCount(); i++)
		{
			std::cout << mf->getArgType(i)->name();
			if(i < mf->getArgCount()-1)
				std::cout << ", ";
		}
		std::cout << ")" << std::endl;
	}
}

MetaFunction* GetFunctionByName(const char* sFunctionName)
{
	for (MetaFunction* mf = MetaFunction::Head(); mf; mf = mf->Next())
	{
		std::string sFunName = mf->getName();
		if (sFunName == sFunctionName)
			return mf;
	}

	return nullptr;
}

#define META_REGISTER_FUN(f) \
	MetaFunction g_MetaFunction_##f(#f, f)