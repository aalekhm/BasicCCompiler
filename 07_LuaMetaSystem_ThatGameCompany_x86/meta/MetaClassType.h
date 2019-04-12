#ifndef LUA_META_CLASS_TYPE_H
#define LUA_META_CLASS_TYPE_H

#include <map>
#include <assert.h>
#include "MetaType.h"
#include "MetaMemberFunction.h"
#include "MetaClassAttribute.h"
#include "MetaUtility.h"
#include "luaRegister.h"

template <typename Cls>
class ClassMetaType : public MetaType{
public:
	ClassMetaType(const char* _name, size_t _numFun, MetaMemberFunction<Cls>** _funs)
		: m_name(_name)
		, m_numFunctions(_numFun)
		, m_Functions(_funs)
		, m_numAttributes(0){}

	template <typename Ret, typename ...Args>
	void addMember(const char* _name, Ret(Cls::*_fun)(Args...)){
		m_numFunctions = m_numFunctions + 1;
		MetaMemberFunction<Cls> **temp = new MetaMemberFunction<Cls>*[m_numFunctions];
		memcpy(temp, m_Functions, (m_numFunctions - 1)*sizeof(MetaMemberFunction<Cls>*));
		temp[m_numFunctions - 1] = new NonConstMetaMemberFunction<Cls>(_name, _fun);
		delete[] m_Functions;
		m_Functions = temp;
	}

	template <typename Ret, typename ...Args>
	void addMember(const char* _name, Ret (Cls::*_fun)(Args...) const){
		m_numFunctions = m_numFunctions + 1;
		MetaMemberFunction<Cls> **temp = new MetaMemberFunction<Cls>*[m_numFunctions];
		memcpy(temp, m_Functions, (m_numFunctions - 1)*sizeof(MetaMemberFunction<Cls>*));
		temp[m_numFunctions - 1] = new ConstMetaMemberFunction<Cls>(_name, _fun);
		delete[] m_Functions;
		m_Functions = temp;
	}

	template <typename Attrib>
	void addAttribute(const char* _name, const Attrib& _attrib){
		MetaClassAttribute<Cls>* attrib = new MetaClassAttribute<Cls>(_name, _attrib);
		assert(m_Attributes.find(attrib->offset()) == m_Attributes.end());
		m_Attributes.insert(std::pair<size_t, MetaClassAttribute<Cls>*>(attrib->offset(), attrib));
		m_numAttributes++;
	}

	size_t sizeOf() const { return sizeof(Cls); }
	const char* name() const { return m_name; }
	virtual bool isBaseType() const { return false; };
	size_t funcCount() const { return m_numFunctions; }
	const MetaMemberFunction<Cls>* metaFunction(size_t _idx) const { return m_Functions[_idx]; }
	size_t attribCount() const { return m_numAttributes; }
	const MetaClassAttribute<Cls>* metaAttribute(size_t _idx) const { return m_Attributes[_idx]; } //TODO

	
	virtual const char* toString(void *v) const {
		const char* str = "";
		for (std::map<size_t, MetaClassAttribute<Cls>*>::const_iterator it = m_Attributes.begin(); it != m_Attributes.end(); it++){
			str += it->second->type()->toString((void*)((char*)v + it->second->offset())) + ";";
		}
		return str;
	}
	virtual float64 toNumber(void* v) const { return 0; }; //TODO

	virtual void cast(void *to, void *from, const MetaType* fromType) const{ } //TODO
	virtual void luaSet(lua_State *L, void *v) const{
		lua_createtable(L, static_cast<unsigned int>(m_numAttributes), 1);
		int table = lua_gettop(L);

		luaL_newmetatable(L, m_name.c_str());
		int metatable = lua_gettop(L);

		lua_pushstring(L, "__metatable"); //hide metatable
		lua_pushboolean(L, false);
		lua_settable(L, metatable);

		lua_pushstring(L, "__index"); //restrict access to members only
		lua_pushvalue(L, table);
		lua_settable(L, metatable);

		lua_pushstring(L, "__newindex"); //make table immutable (but not the objects in it)
		lua_pushcfunction(L, luaReadOnlyError);
		lua_settable(L, metatable);

		lua_pushstring(L, "new"); //call default constructor
		lua_pushcclosure(L, MetaLuaCConstructor<Cls>, 0);
		lua_settable(L, table);

		for (size_t i = 0; i < m_numFunctions; i++){ //add functions
			lua_pushstring(L, m_Functions[i]->name().c_str());
			lua_pushlightuserdata(L, v);
			lua_pushlightuserdata(L, m_Functions[i]);
			lua_pushcclosure(L, MetaLuaCMemberFunction<Cls>, 2);
			lua_settable(L, table);
		}

		//add attributes
		for (std::map<size_t, MetaClassAttribute<Cls>*>::const_iterator it = m_Attributes.begin(); it != m_Attributes.end(); it++){
			lua_pushstring(L, it->second->name().c_str());
			it->second->type()->luaSet(L, (char*)v + it->second->offset());
			lua_settable(L, table);
		}
		
		//assign metatable
		lua_setmetatable(L, table);
	}
	virtual void luaGet(lua_State *L, int index, void *v) const{
		int i = 0;
		for (std::map<size_t, MetaClassAttribute<Cls>*>::const_iterator it = m_Attributes.begin(); it != m_Attributes.end(); it++){
			lua_pushstring(L, it->second->name().c_str());
			lua_gettable(L, index-i);
			it->second->type()->luaGet(L, -1, (char*)v + it->second->offset());
			i++;
		}
	}

	virtual void* New() const{ return new Cls(); };
	virtual void Delete(void* v) const{ delete (Cls*)v; };
	virtual void* PlacementNew(void* buffer) const { return new (buffer) Cls(); };
	virtual void Dtor(void* v) const { reinterpret_cast<Cls*>(v)->~Cls(); }; 

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type");
		return this;
	};
	virtual const MetaType* AddressType() const { return this; };
private:
	const char* m_name;
	size_t m_numFunctions;
	MetaMemberFunction<Cls> **m_Functions;
	size_t m_numAttributes;
	std::map<size_t, MetaClassAttribute<Cls>*> m_Attributes;
};


template <typename Cls>
const ClassMetaType<Cls>* getMetaTypeByClass(){
	return reinterpret_cast<const ClassMetaType<Cls>*>(getMetaTypeByType<Cls>());
}

#define BEGIN_META_CLASS_DEFINITION(NAMESPACE, META_CLASS) \
	static ClassMetaType<NAMESPACE::META_CLASS>* define##NAMESPACE##META_CLASS(){ \
		ClassMetaType<NAMESPACE::META_CLASS>* cls = new ClassMetaType<NAMESPACE::META_CLASS>(#META_CLASS, 0, nullptr);

#define ADD_MEMBER_FUNCTION(_str, _fun) cls->addMember(_str, _fun);
#define ADD_ATTRIBUTE(_str, _attrib) cls->addAttribute(_str, _attrib)

#define END_META_CLASS_DEFINITION(NAMESPACE, META_CLASS) return cls; } \
	static const ClassMetaType<NAMESPACE::META_CLASS>* g_Meta##NAMESPACE_##META_CLASS = define##NAMESPACE##META_CLASS();\
	template <>\
	static const MetaType* getMetaTypeByType<NAMESPACE::META_CLASS>() { return g_Meta##NAMESPACE_##META_CLASS; } 


#endif //LUA_META_CLASS_TYPE_H
