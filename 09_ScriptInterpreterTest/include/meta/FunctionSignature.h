#pragma once
#include "MetaType.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<< 4. >>>>>>>>>>>>>>>>>>>>>>>>
class FunctionSignature
{
	public:
		const MetaType*		getRetType() const { return m_Ret; }
		const MetaType*		getArgType(int idx) const { return m_Args[idx]; }
		int					getArgCount() { return m_ArgCount; }

		// 1. Function with return type, but no arguments.
		template <typename Ret>
		FunctionSignature( Ret (*) () )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			m_Args = 0;
			m_ArgCount = 0;
		}

		// 2. Function with return type, 1 argument.
		template <typename Ret, typename Arg0>
		FunctionSignature( Ret (*) (Arg0) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>() };
			m_Args = args;
			m_ArgCount = 1;
		}

		// 3. Function with return type, 2 arguments.
		template <typename Ret, typename Arg0, typename Arg1>
		FunctionSignature( Ret (*) (Arg0, Arg1) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>() };
			m_Args = args;
			m_ArgCount = 2;
		}

		// 4. Function with return type, 3 arguments.
		template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
		FunctionSignature( Ret (*) (Arg0, Arg1, Arg2) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>() };
			m_Args = args;
			m_ArgCount = 3;
		}

		// 5. Function with return type, 4 arguments.
		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
		FunctionSignature( Ret (*) (Arg0, Arg1, Arg2, Arg3) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>() };
			m_Args = args;
			m_ArgCount = 4;
		}

		// 6. Function with return type, 5 arguments.
		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		FunctionSignature( Ret (*) (Arg0, Arg1, Arg2, Arg3, Arg4) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>() };
			m_Args = args;
			m_ArgCount = 5;
		}

		// 7. Function with return type, 6 arguments.
		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		FunctionSignature( Ret (*) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>(), &GetMetaTypeByType<Arg5>() };
			m_Args = args;
			m_ArgCount = 6;
		}

		// 8. Function with return type, 7 arguments.
		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		FunctionSignature( Ret (*) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>(), &GetMetaTypeByType<Arg5>(), &GetMetaTypeByType<Arg6>() };
			m_Args = args;
			m_ArgCount = 7;
		}

		// 9. Function with return type, 8 arguments.
		template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
		FunctionSignature( Ret (*) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) )
		{
			m_Ret = &GetMetaTypeByType<Ret>();
			static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>(), &GetMetaTypeByType<Arg5>(), &GetMetaTypeByType<Arg6>(), &GetMetaTypeByType<Arg7>() };
			m_Args = args;
			m_ArgCount = 8;
		}

	private:
		const MetaType*		m_Ret;
		const MetaType**	m_Args;
		int					m_ArgCount;
};
