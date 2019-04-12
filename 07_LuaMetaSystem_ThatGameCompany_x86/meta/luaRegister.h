#ifndef LUA_REGISTER_H
#define LUA_REGISTER_H

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <assert.h>
#include "MetaFunction.h"
#include "MetaClassType.h"

//Function to create a Cls Object in lua
template <typename Cls>
static int MetaLuaCConstructor(lua_State* L){
	Variable ret;
	ret.m_type = getMetaTypeByType<Cls>();
	Cls* ptr = (Cls*)lua_newuserdata(L, ret.m_type->sizeOf());
	ret.m_address = ret.m_type->PlacementNew(ptr);

	if (ret.m_type->sizeOf() > 0){
		ret.m_type->luaSet(L, ret.m_address);
	}

	return ret.m_type->sizeOf() > 0 ? 1 : 0;
}

//Memberfunction Callback from lua
template <typename Cls>
static int MetaLuaCMemberFunction(lua_State* L){
	void *upval = lua_touserdata(L, lua_upvalueindex(1));
	assert(upval);

	MetaMemberFunction<Cls>* fun = reinterpret_cast<MetaMemberFunction<Cls>*>(lua_touserdata(L, lua_upvalueindex(2)));

	size_t argCount = fun->argCount();
	Variable ret;
	ret.m_type = fun->retType();
	ret.m_address = ret.m_type->PlacementNew(alloca(ret.m_type->sizeOf()));

	Variable* args = reinterpret_cast<Variable*>(alloca(argCount * sizeof(Variable)));
	for (size_t i = 0; i < argCount; i++){
		args[i].m_type = fun->argType(static_cast<unsigned int>(i));
		args[i].m_address = args[i].m_type->PlacementNew(alloca(args[i].m_type->sizeOf()));
		args[i].m_type->luaGet(L, 1 + static_cast<unsigned int>(i), args[i].m_address);
	}

	fun->apply(upval, ret, args, argCount);

	if (ret.m_type->sizeOf() > 0){
		ret.m_type->luaSet(L, ret.m_address);
	}

	ret.m_type->Dtor(ret.m_address);
	for (size_t i = 0; i < argCount; i++){
		args[i].m_type->Dtor(args[i].m_address);
	}
	return ret.m_type->sizeOf() > 0 ? 1 : 0;
}

//standard lua function callback
static int MetaLuaCFunction(lua_State* L){
	void *upval = lua_touserdata(L, lua_upvalueindex(1));
	assert(upval);

	const MetaFunction* mf = reinterpret_cast<const MetaFunction*>(upval);
	Variable ret;
	ret.m_type = mf->retType();
	ret.m_address = ret.m_type->PlacementNew(alloca(ret.m_type->sizeOf()));

	size_t argCount = mf->argCount();
	Variable* args = reinterpret_cast<Variable*>(alloca(argCount * sizeof(Variable)));
	for (int i = 0; i < (int)argCount; i++){
		args[i].m_type = mf->argType(i);
		args[i].m_address = args[i].m_type->PlacementNew(alloca(args[i].m_type->sizeOf()));
		args[i].m_type->luaGet(L, 1 + i, args[i].m_address);
	}

	mf->apply(ret, args, argCount);

	if (ret.m_type->sizeOf() > 0){
		ret.m_type->luaSet(L, ret.m_address);
	}

	ret.m_type->Dtor(ret.m_address);
	for (size_t i = 0; i < argCount; i++){
		args[i].m_type->Dtor(args[i].m_address);
	}

	return (ret.m_type->sizeOf() > 0 ? 1 : 0);
}

/**
 * Using the Autolister pattern, this functions registers the MetaFunctions, MetaVariables and MetaTypes (Classes)
 */
static void MetaBind(lua_State* L){
	for (const MetaFunction*p = MetaFunction::Head(); p; p = p->Next()){
		const char* name = p->name();
		MetaFunction* fun = (MetaFunction*)p;

		lua_pushlightuserdata(L, fun);
		lua_pushcclosure(L, MetaLuaCFunction, 1);
		lua_setglobal(L, name);
	}

	for (const MetaType* v = MetaType::Head(); v; v = v->Next()){
		if (!v->isBaseType()){
			const char* name = v->name();
			v->luaSet(L, v->PlacementNew(alloca(v->sizeOf())));
			lua_setglobal(L, name);
		}
	}

	for (const MetaVariable* mv = MetaVariable::Head(); mv; mv = mv->Next()){
		mv->m_type->luaSet(L, mv->m_address);
		lua_setglobal(L, mv->m_name);
	}
}

static int luaReadOnlyError(lua_State* L){
	lua_pushstring(L, "Error! Attempt to modify read-only value");
	lua_error(L);
	return 0;
}

#endif //LUA_REGISTER_H
