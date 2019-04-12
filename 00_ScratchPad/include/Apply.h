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
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0>
static void Apply( void (*fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	Arg0 a0 = Cast<Arg0>(args[0].v, *args[0].m);
	fun( a0 );
}

template <typename Ret, typename Arg0>
static void Apply( Ret(*fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m));
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1>
static void Apply( void (*fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1>
static void Apply(Ret(*fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2>
static void Apply( void (*fun) (Arg0, Arg1, Arg2), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m),
		Cast<Arg2>(args[2].v, *args[2].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m),
				Cast<Arg2>(args[2].v, *args[2].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m),
		Cast<Arg2>(args[2].v, *args[2].m),
		Cast<Arg3>(args[3].v, *args[3].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m),
				Cast<Arg2>(args[2].v, *args[2].m),
				Cast<Arg3>(args[3].v, *args[3].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m),
		Cast<Arg2>(args[2].v, *args[2].m),
		Cast<Arg3>(args[3].v, *args[3].m),
		Cast<Arg4>(args[4].v, *args[4].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m),
				Cast<Arg2>(args[2].v, *args[2].m),
				Cast<Arg3>(args[3].v, *args[3].m),
				Cast<Arg4>(args[4].v, *args[4].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m),
		Cast<Arg2>(args[2].v, *args[2].m),
		Cast<Arg3>(args[3].v, *args[3].m),
		Cast<Arg4>(args[4].v, *args[4].m),
		Cast<Arg5>(args[5].v, *args[5].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m),
				Cast<Arg2>(args[2].v, *args[2].m),
				Cast<Arg3>(args[3].v, *args[3].m),
				Cast<Arg4>(args[4].v, *args[4].m),
				Cast<Arg5>(args[5].v, *args[5].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m),
		Cast<Arg2>(args[2].v, *args[2].m),
		Cast<Arg3>(args[3].v, *args[3].m),
		Cast<Arg4>(args[4].v, *args[4].m),
		Cast<Arg5>(args[5].v, *args[5].m),
		Cast<Arg6>(args[6].v, *args[6].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m),
				Cast<Arg2>(args[2].v, *args[2].m),
				Cast<Arg3>(args[3].v, *args[3].m),
				Cast<Arg4>(args[4].v, *args[4].m),
				Cast<Arg5>(args[5].v, *args[5].m),
				Cast<Arg6>(args[6].v, *args[6].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
static void Apply( void (*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Variable ret, Variable* args, int argCount)
{
	fun(Cast<Arg0>(args[0].v, *args[0].m),
		Cast<Arg1>(args[1].v, *args[1].m),
		Cast<Arg2>(args[2].v, *args[2].m),
		Cast<Arg3>(args[3].v, *args[3].m),
		Cast<Arg4>(args[4].v, *args[4].m),
		Cast<Arg5>(args[5].v, *args[5].m),
		Cast<Arg6>(args[6].v, *args[6].m),
		Cast<Arg7>(args[7].v, *args[7].m)
	);
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
static void Apply(Ret(*fun) (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Variable ret, Variable* args, int argCount)
{
	Ret r = fun(Cast<Arg0>(args[0].v, *args[0].m),
				Cast<Arg1>(args[1].v, *args[1].m),
				Cast<Arg2>(args[2].v, *args[2].m),
				Cast<Arg3>(args[3].v, *args[3].m),
				Cast<Arg4>(args[4].v, *args[4].m),
				Cast<Arg5>(args[5].v, *args[5].m),
				Cast<Arg6>(args[6].v, *args[6].m),
				Cast<Arg7>(args[7].v, *args[7].m)
	);
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}