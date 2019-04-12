#ifndef META_VARIABLE_H
#define META_VARIABLE_H

#include "MetaType.h"

#define META_REGISTER_VAR(v) \
	MetaVariable g_metaVariable_##v(#v, v);


struct MetaVariable : public AutoLister<MetaVariable>{
	template <typename Var>
	MetaVariable(const char* _name, const Var& _var)
		: m_name(_name)
		, m_type(getMetaTypeByType<Var>())
	{
		m_address = (void*)(new char[m_type->sizeOf()]);
		memcpy(m_address, &_var, m_type->sizeOf());
	}
	const char* m_name;
	void* m_address;
	const MetaType* m_type;
};

struct Variable{
	Variable() : m_address(nullptr), m_type(getMetaTypeByType<void>()) {}
	Variable(const Variable& _var) : m_address(_var.m_address), m_type(_var.m_type) {}
	template <typename type>
	Variable(const type& t) : m_address((void*)&t), m_type(getMetaTypeByType<type>()){}
	void* m_address;
	const MetaType* m_type;
};


template <typename T, typename S>
T Cast(const S& _s){
	if (getMetaTypeByType<T>()->isBaseType() && _s.m_type->isBaseType())
		return Static_Cast<T>(_s);
	else
		return *Reinterpret_Cast<T>(_s);
}

template <typename T, typename S>
inline T* Reinterpret_Cast(const S& _s){
	T* to = reinterpret_cast<T*>(_s.m_address);
	return to;
}

template <typename T, typename S>
inline T Static_Cast(const S& _s){
	T to;
	getMetaTypeByType<T>()->cast(&to, _s.m_address, _s.m_type);
	return to;
}

#endif //META_VARIABLE_H
