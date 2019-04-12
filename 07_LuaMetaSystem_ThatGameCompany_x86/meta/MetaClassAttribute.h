#ifndef META_CLASS_ATTRIBUTE
#define META_CLASS_ATTRIBUTE

#include <stddef.h>
#include "MetaType.h"


template <typename Cls>
class MetaClassAttribute{
public:
	template <typename Attrib>
	MetaClassAttribute(const char* _name, Attrib Cls::*_mem)
		: m_type(getMetaTypeByType< Attrib >())
		, m_name(_name) 
		, m_offset(reinterpret_cast<size_t>(&(((Cls*)nullptr)->*_mem))){
		
	}

	size_t offset() const { return m_offset; }
	const char* name() const { return m_name; }
	const MetaType* type() const { return m_type; }
private:
	size_t m_offset;
	const char* m_name;
	const MetaType* m_type;
};


#endif //META_CLASS_ATTRIBUTE
