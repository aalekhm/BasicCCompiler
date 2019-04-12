#pragma once
#include "Variable.h"
#include "MetaType.h"

static void Apply(void (*fun) (), Variable ret, Variable* args, int argCount)
{
	fun();
}

template <typename Ret>
static void Apply( Ret(*fun) (), Variable ret, Variable* args, int argCount)
{
	Ret r = fun();
	ret.m->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0>
static void Apply( void (*fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	Arg0 a0 = Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType);
	fun( a0 );
}

template <typename Ret, typename Arg0>
static void Apply( Ret(*fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType));
	ret.m->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1>
static void Apply( void (*fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1>
static void Apply(Ret(*fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2>
static void Apply( void (*fun) (Arg0, Arg1, Arg2), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
		Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
				Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
		Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
		Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
				Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
				Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
		Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
		Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
		Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
				Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
				Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
				Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
		Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
		Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
		Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType),
		Cast<Arg5>(args[5].m_Var, *args[5].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
				Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
				Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
				Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType),
				Cast<Arg5>(args[5].m_Var, *args[5].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
		Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
		Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
		Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType),
		Cast<Arg5>(args[5].m_Var, *args[5].m_MetaType),
		Cast<Arg6>(args[6].m_Var, *args[6].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
				Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
				Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
				Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType),
				Cast<Arg5>(args[5].m_Var, *args[5].m_MetaType),
				Cast<Arg6>(args[6].m_Var, *args[6].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
		Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
		Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
		Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
		Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType),
		Cast<Arg5>(args[5].m_Var, *args[5].m_MetaType),
		Cast<Arg6>(args[6].m_Var, *args[6].m_MetaType),
		Cast<Arg7>(args[7].m_Var, *args[7].m_MetaType)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].m_Var, *args[0].m_MetaType),
				Cast<Arg1>(args[1].m_Var, *args[1].m_MetaType),
				Cast<Arg2>(args[2].m_Var, *args[2].m_MetaType),
				Cast<Arg3>(args[3].m_Var, *args[3].m_MetaType),
				Cast<Arg4>(args[4].m_Var, *args[4].m_MetaType),
				Cast<Arg5>(args[5].m_Var, *args[5].m_MetaType),
				Cast<Arg6>(args[6].m_Var, *args[6].m_MetaType),
				Cast<Arg7>(args[7].m_Var, *args[7].m_MetaType)
	);
	ret.m_MetaType->cast(ret.m_Var, &r, GetMetaType(r));
}