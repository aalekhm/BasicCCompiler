#pragma once
#include "MetaType.h"

struct Variable
{
	Variable() 
	: v(0)
	, m(&GetMetaTypeByType<void>())
	{}

	template <typename T>
	Variable(const T& t) 
	: v((void*)&t)
	, m(&GetMetaType(t)) 
	{}

	void*	v;
	const	MetaType*	m;
};