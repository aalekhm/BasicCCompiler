#pragma once
#include "MetaType.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<< 5. >>>>>>>>>>>>>>>>>>>>>>>>
struct Variable
{
	Variable()
	: m_Var(0)
	, m_MetaType(&GetMetaTypeByType<void>())
	{ }

	template <typename T>
	Variable(const T& t)
	: m_Var((void*)&t)
	, m_MetaType(&GetMetaType(t))
	{ }

	void*			m_Var;
	const MetaType*	m_MetaType;
};