#ifndef META_UTILITY_H
#define META_UTILITY_H

#include <stdint.h>
#include "typeinfo.h"


template <typename type>
void luaSetGlobal(lua_State *L, const type& _t, const char* _name){
	getMetaTypeByType<type>()->luaSet(L, (void*)&_t);
	lua_setglobal(L, _name);
}

template <typename type>
void luaGetGlobal(lua_State *L, type& _t, const char* _name){
	lua_getglobal(L, _name);
	getMetaTypeByType<type>()->luaGet(L, -1, (void*)&_t);
}


template <typename T>
class AutoLister{
public:
	AutoLister(){
		m_next = Head();
		Head() = static_cast<T*>(this);
	}

	T* Next() { return m_next; }
	const T* Next() const { return m_next; }

	static T*& Head() { static T* p = 0; return p; }

private:
	T* m_next;
};

#endif //META_UTILITY_H
