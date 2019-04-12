#pragma once
#include "MetaType.h"

class FunctionSignature
{
	public:
		template <typename Ret>
		FunctionSignature( Ret (*)() )
		{
			m_ret = &GetMetaTypeByType<Ret>();
			m_args = 0;
			m_argCount = 0;
		}

		template <typename Ret, typename Arg0>
		FunctionSignature( Ret (*) (Arg0) )
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>() };
			m_args = args;
			m_argCount = 1;
		}

		template <typename Ret, typename Arg0, typename Arg1>
		FunctionSignature( Ret (*) (Arg0, Arg1))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>() };
			m_args = args;
			m_argCount = 2;
		}

		template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
		FunctionSignature( Ret(*) (Arg0, Arg1, Arg2))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>() };
			m_args = args;
			m_argCount = 3;
		}

		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
		FunctionSignature( Ret(*) (Arg0, Arg1, Arg2, Arg3))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>() };
			m_args = args;
			m_argCount = 4;
		}

		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		FunctionSignature( Ret(*) (Arg0, Arg1, Arg2, Arg3, Arg4))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>() };
			m_args = args;
			m_argCount = 5;
		}

		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		FunctionSignature( Ret(*) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>(), &GetMetaTypeByType<Arg5>() };
			m_args = args;
			m_argCount = 6;
		}

		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		FunctionSignature( Ret(*) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>(), &GetMetaTypeByType<Arg5>(), &GetMetaTypeByType<Arg6>() };
			m_args = args;
			m_argCount = 7;
		}

		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
		FunctionSignature( Ret(*) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7))
		{
			m_ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>(), &GetMetaTypeByType<Arg5>(), &GetMetaTypeByType<Arg6>(), &GetMetaTypeByType<Arg7>() };
			m_args = args;
			m_argCount = 8;
		}

		const MetaType* RetType() const { return m_ret; }
		const MetaType* ArgType(int idx) const { return m_args[idx]; }
		int ArgCount() const { return m_argCount; }

	private:
		const MetaType* m_ret;
		const MetaType** m_args;
		int m_argCount;
};

void PrintFunctionInfo(const FunctionSignature& sig)
{
	std::cout << "Ret: " << sig.RetType()->Name() << std::endl;
	std::cout << "Arg Count: " << sig.ArgCount() << std::endl;
	for (int i = 0; i < sig.ArgCount(); i++)
	{
		std::cout << "Arg" << i << ": " << sig.ArgType(i)->Name() << std::endl;
	}
}
