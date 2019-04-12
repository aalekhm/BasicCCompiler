#ifndef METATYPE_H
#define METATYPE_H

#include <sstream>
extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "MetaUtility.h"
#include "typeInfo.h"

const struct MetaType : public AutoLister<MetaType>{
	virtual const char* name() const = 0;
	virtual size_t sizeOf() const = 0;
	virtual bool isBaseType() const = 0;

	virtual const char* toString(void* v) const = 0;
	virtual float64 toNumber(void* v) const = 0;

	virtual void cast(void *to, void *from, const MetaType* fromType) const = 0;

	virtual void* PlacementNew(void* buffer) const = 0;
	virtual void Dtor(void* v) const = 0;
	virtual void* New() const = 0;
	virtual void Delete(void* v) const = 0;

	virtual const MetaType* DereferenceType() const = 0;
	virtual const MetaType* AddressType() const = 0;
	//virtual void Dereference(size_t offset, void* v) const = 0;
	//virtual void Address(void* v) const = 0;

	virtual void luaSet(lua_State *L, void *v) const = 0;
	virtual void luaGet(lua_State *L, int32 index, void *v) const = 0;
};

template <typename T>
const MetaType* getMetaTypeByType();

// TODO typedef platform specific pointer lengths
// TODO define addressTypes at compiletime with set recursion depth

/**
 * VoidMetaType represents - nothing.
 * nonetheless this is useful for defining void functions etc.
 * the addressType on the other hand is a void pointer, which represents a pointer to arbitrary data
 */
const struct VoidMetaType : public MetaType{
	virtual const char* name() const { return "void"; };
	virtual size_t sizeOf() const { return 0; };
	virtual bool isBaseType() const { return true; };

	virtual const char* toString(void* v) const { return ""; };
	virtual float64 toNumber(void* v) const { return 0; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{

	}
	virtual void* PlacementNew(void* buffer) const { return buffer; };
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types

	virtual void* New() const{ return nullptr; };
	virtual void Delete(void* v) const{};

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'void\'");
		return this;
	};
	virtual const MetaType* AddressType() const { 
		static const MetaType* VoidMeta = this;
		static const struct VoidStarMetaType : public MetaType{
			virtual const char* name() const { return "void*"; };
			virtual size_t sizeOf() const { return sizeof(void*); };
			virtual bool isBaseType() const { return true; };

			virtual const char* toString(void* v) const { return std::to_string((unsigned long long)v).c_str(); };
			virtual float64 toNumber(void* v) const { return (float64)*(unsigned long long*)v; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
				unsigned long long ull = std::stoull(fromType->toString(*(void**)from));
				memcpy(to, &ull, sizeOf());
			}
			virtual void* PlacementNew(void* buffer) const { return buffer; };
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types

			virtual void* New() const{ return nullptr; };
			virtual void Delete(void* v) const{};

			virtual const MetaType* DereferenceType() const {
				return VoidMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* VoidStarMeta = this;
				static const struct VoidStarStarMetaType : public MetaType{
					virtual const char* name() const { return "void**"; };
					virtual size_t sizeOf() const { return sizeof(void**); };
					virtual bool isBaseType() const { return true; };

					virtual const char* toString(void* v) const { return std::to_string((unsigned long long)v).c_str(); };
					virtual float64 toNumber(void* v) const { return (float64)*(unsigned long long*)v; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
						unsigned long long ull = std::stoull(fromType->toString(*(void***)from));
						memcpy(to, &ull, sizeOf());
					}
					virtual void* PlacementNew(void* buffer) const { return buffer; };
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types

					virtual void* New() const{ return nullptr; };
					virtual void Delete(void* v) const{};

					virtual const MetaType* DereferenceType() const {
						return VoidStarMeta;
					};
					virtual const MetaType* AddressType() const { //end recursion
						assert(false && "Too many stars");
						return this;
					};

					// TEST if this sets and returns properly
					virtual void luaSet(lua_State *L, void *v) const{ 
						lua_pushlightuserdata(L, v);
					}
					virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
						void** dest = reinterpret_cast<void**>(lua_touserdata(L, index));
						memcpy(v, &dest, sizeOf());
					}
				} l_VoidStarStarMetaType;
				return &l_VoidStarStarMetaType;
			};

			// TEST if this sets and returns properly
			virtual void luaSet(lua_State *L, void *v) const{
				lua_pushlightuserdata(L, v);
			}
			virtual void luaGet(lua_State *L, int32 index, void *v) const{
				void* dest = lua_touserdata(L, index);
				memcpy(v, &dest, sizeOf());
			}
		} l_VoidStarMetaType;
		return &l_VoidStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{
		lua_pushnil(L);
	}
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		// INVESTIGATE See if this should pop the lua stack once, probably not
	}
} g_VoidMetaType;

const struct BoolMetaType : public MetaType{
	virtual const char* name() const { return "bool"; };
	virtual size_t sizeOf() const { return sizeof(bool); };
	virtual bool isBaseType() const { return true; };

	virtual const char* toString(void* v) const { return (*(bool*)v) ? "true" : "false"; };
	virtual float64 toNumber(void* v) const { return (*(bool*)v) ? 1 : 0; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		bool i = 0 != fromType->toNumber(from);
		memcpy(to, &i, sizeof(bool));
	}

	virtual void* PlacementNew(void* buffer) const {
		memset(buffer, 0, sizeof(bool));
		return buffer;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new bool; };
	virtual void Delete(void* v) const{ delete (bool*)v; }; 

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'bool\'");
		return this;
	};
	virtual const MetaType* AddressType() const { 
		static const MetaType* BoolMeta = this;
		static const struct BoolStarMetaType : public MetaType{
			virtual const char* name() const { return "bool*"; };
			virtual size_t sizeOf() const { return sizeof(bool*); };
			virtual bool isBaseType() const { return true; };

			virtual const char* toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const {
				return new(buffer) bool*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new bool*; };
			virtual void Delete(void* v) const{ delete (bool**)v; };

			virtual const MetaType* DereferenceType() const {
				return BoolMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* BoolStarMeta = this;
				static const struct BoolStarStarMetaType : public MetaType{
					virtual const char* name() const { return "bool**"; };
					virtual size_t sizeOf() const { return sizeof(bool**); };
					virtual bool isBaseType() const { return true; };

					virtual const char* toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new(buffer) bool**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new bool**; };
					virtual void Delete(void* v) const{ delete (bool***)v; };

					virtual const MetaType* DereferenceType() const {
						return BoolStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too many stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{
					}
					virtual void luaGet(lua_State *L, int32 index, void *v) const{
					}
				} l_BoolStarStarMetaType;
				return &l_BoolStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{
			}
			virtual void luaGet(lua_State *L, int32 index, void *v) const{
			}
		} l_BoolStarMetaType;
		return &l_BoolStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{
		lua_pushboolean(L, 0 != toNumber(v));
	}
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		bool i = (lua_toboolean(L, index) != 0);
		memcpy(v, &i, sizeof(bool));
	}
} g_BoolMetaType;

const struct IntMetaType : public MetaType{
	virtual const char* name() const { return "int32"; };
	virtual size_t sizeOf() const { return sizeof(int32); };
	virtual bool isBaseType() const { return true; };

	virtual const char* toString(void* v) const { 
        std::string str = std::to_string(*(int32*)v);
        char* dest = new char[str.size()+1];
        memcpy(dest, str.c_str(), str.size()+1);
        return dest;
    };
	virtual float64 toNumber(void* v) const { return *(int32*)v; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		int32 i = static_cast<int32>(fromType->toNumber(from));
		memcpy(to, &i, sizeof(int32));
	}

	virtual void* PlacementNew(void* buffer) const {
		return new (buffer)int32;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new int32; };
	virtual void Delete(void* v) const{ delete reinterpret_cast<int32*>(v); };


	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'int32\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* IntMeta = this;
		static const struct IntStarMetaType : public MetaType{
			virtual const char* name() const { return "int32*"; };
			virtual size_t sizeOf() const { return sizeof(int32*); };
			virtual bool isBaseType() const { return true; };

			virtual const char* toString(void* v) const { 
				std::stringstream ss;
				ss << v;
				return ss.str().c_str();
			};
			virtual float64 toNumber(void* v) const { return (float64)reinterpret_cast<size_t>(v); };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{

			}

			virtual void* PlacementNew(void* buffer) const {
				return new (buffer)int32*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new int32*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<int32**>(v); };


			virtual const MetaType* DereferenceType() const {
				return IntMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* IntStarMeta = this;
				static const struct IntStarStarMetaType : public MetaType{
					virtual const char* name() const { return "int32**"; };
					virtual size_t sizeOf() const { return sizeof(int32**); };
					virtual bool isBaseType() const { return true; };

					virtual const char* toString(void* v) const {
						std::stringstream ss;
						ss << v;
						return ss.str().c_str();
					};
					virtual float64 toNumber(void* v) const { return (float64)reinterpret_cast<size_t>(v); };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new (buffer)int32**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new int32**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<int32***>(v); };


					virtual const MetaType* DereferenceType() const {
						return IntStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{ lua_pushlightuserdata(L, v); }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{
						int32* d = reinterpret_cast<int32*>(lua_touserdata(L, index));
						memcpy(v, &d, sizeOf());
					}
				} l_IntStarStarMetaType;
				return &l_IntStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{ lua_pushlightuserdata(L, v); }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{
				int32* d = reinterpret_cast<int32*>(lua_touserdata(L, index));
				memcpy(v, &d, sizeOf());
			}
		} l_IntStarMetaType;
		return &l_IntStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		int32 d = static_cast<int32>(lua_tonumber(L, index));
		memcpy(v, &d, sizeof(int32));
	};
} g_IntMetaType;

const struct UnsignedIntMetaType : public MetaType{
	virtual const char* name() const { return "uint32"; };
	virtual size_t sizeOf() const { return sizeof(uint32); };
	virtual bool isBaseType() const { return true; };

	virtual const char* toString(void* v) const { 
        std::string str = std::to_string(*(uint32*)v);
        char* dest = new char[str.size()+1];
        memcpy(dest, str.c_str(), str.size()+1);
        return dest;
    };
	virtual float64 toNumber(void* v) const { return *(uint32*)v; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		int32 i = static_cast<uint32>(fromType->toNumber(from));
		memcpy(to, &i, sizeof(uint32));
	}

	virtual void* PlacementNew(void* buffer) const {
		return new (buffer)uint32;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new uint32; };
	virtual void Delete(void* v) const{ delete (uint32*)v; };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'uint32\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* UnsignedIntMeta = this;
		static const struct UnsignedIntStarMetaType : public MetaType{
			virtual const char* name() const { return "uint32*"; };
			virtual size_t sizeOf() const { return sizeof(uint32*); };
			virtual bool isBaseType() const { return true; };

			virtual const char* toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const {
				return new (buffer)uint32*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new uint32*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<uint32**>(v); };

			virtual const MetaType* DereferenceType() const {
				return UnsignedIntMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* UnsignedIntStarMeta = this;
				static const struct UnsignedIntStarStarMetaType : public MetaType{
					virtual const char* name() const { return "uint**"; };
					virtual size_t sizeOf() const { return sizeof(uint32**); };
					virtual bool isBaseType() const { return true; };

					virtual const char* toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new (buffer)uint32**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new uint32**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<uint32***>(v); };

					virtual const MetaType* DereferenceType() const {
						return UnsignedIntStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{ }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{ }
				} l_UnsignedIntStarStarMetaType;
				return &l_UnsignedIntStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{ }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{ }
		} l_UnsignedIntStarMetaType;
		return &l_UnsignedIntStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		uint32 d = static_cast<uint32>(lua_tonumber(L, index));
		memcpy(v, &d, sizeof(uint32));
	}
} g_UnsignedIntMetaType;

const struct FloatMetaType : public MetaType{
	virtual const char* name() const { return "float32"; };
	virtual size_t sizeOf() const { return sizeof(float32); };
	virtual bool isBaseType() const { return true; };

	virtual const char* toString(void* v) const { 
        std::string str = std::to_string(*(float32*)v);
        char* dest = new char[str.size()+1];
        memcpy(dest, str.c_str(), str.size()+1);
        return dest;
    };
	virtual float64 toNumber(void* v) const { return *(float32*)v; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		float32 f = static_cast<float32>(fromType->toNumber(from));
		memcpy(to, &f, sizeof(float32));
	}

	virtual void* PlacementNew(void* buffer) const {
		return new (buffer)float32;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new float32; };
	virtual void Delete(void* v) const{ delete (float32*)v; };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'float32\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* FloatMeta = this;
		static const struct FloatStarMetaType : public MetaType{
			virtual const char* name() const { return "float32*"; };
			virtual size_t sizeOf() const { return sizeof(float32*); };
			virtual bool isBaseType() const { return true; };

			virtual const char* toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const {
				return new (buffer)float32*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new float32*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<float32**>(v); };

			virtual const MetaType* DereferenceType() const {
				return FloatMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* FloatStarMeta = this;
				static const struct FloatStarStarMetaType : public MetaType{
					virtual const char* name() const { return "float32**"; };
					virtual size_t sizeOf() const { return sizeof(float32**); };
					virtual bool isBaseType() const { return true; };

					virtual const char* toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new (buffer)float32**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new float32**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<float32***>(v); };

					virtual const MetaType* DereferenceType() const {
						return FloatStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{  }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
				} l_FloatStarStarMetaType;
				return &l_FloatStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{  }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
		} l_FloatStarMetaType;
		return &l_FloatStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		float32 d = static_cast<float32>(lua_tonumber(L, index));
		memcpy(v, &d, sizeof(float32));
	}
} g_FloatMetaType;

const struct DoubleMetaType : public MetaType{
	virtual const char* name() const { return "float64"; }
	virtual size_t sizeOf() const { return sizeof(float64); }
	virtual bool isBaseType() const { return true; };

	virtual const char* toString(void* v) const { 
        std::string str = std::to_string(*(float64*)v);
        char* dest = new char[str.size()+1];
        memcpy(dest, str.c_str(), str.size()+1);
        return dest;
    }
	virtual float64 toNumber(void* v) const { return *(float64*)v; }

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		float64 d = fromType->toNumber(from);
		memcpy(to, &d, sizeof(float64));
	}

	virtual void* PlacementNew(void* buffer) const{
		return new (buffer)float64;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new float64; };
	virtual void Delete(void* v) const{ delete reinterpret_cast<float64*>(v); };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'float64\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* DoubleMeta = this;
		static const struct DoubleStarMetaType : public MetaType{
			virtual const char* name() const { return "float64*"; }
			virtual size_t sizeOf() const { return sizeof(float64*); }
			virtual bool isBaseType() const { return true; };

			virtual const char* toString(void* v) const { return ""; }
			virtual float64 toNumber(void* v) const { return 0; }

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const{
				return new (buffer)float64*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new float64*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<float64**>(v); };

			virtual const MetaType* DereferenceType() const {
				return DoubleMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* DoubleStarMeta = this;
				static const struct DoubleStarStarMetaType : public MetaType{
					virtual const char* name() const { return "float64**"; }
					virtual size_t sizeOf() const { return sizeof(float64**); }
					virtual bool isBaseType() const { return true; };

					virtual const char* toString(void* v) const { return ""; }
					virtual float64 toNumber(void* v) const { return 0; }

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const{
						return new (buffer)float64**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new float64**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<float64***>(v); };

					virtual const MetaType* DereferenceType() const {
						return DoubleStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{  }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
				} l_DoubleStarStarMetaType;
				return &l_DoubleStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{  }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
		} l_DoubleStarMetaType;
		return &l_DoubleStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
		float64 d = lua_tonumber(L, index);
		memcpy(v, &d, sizeof(float64));
	}
} g_DoubleMetaType;

const struct CharMetaType : public MetaType{
    virtual const char* name() const { return "char8"; }
    virtual size_t sizeOf() const { return sizeof(char8); }
    virtual bool isBaseType() const { return true; }

    virtual const char* toString(void* v) const { return (char8*)v; }
    virtual float64 toNumber(void* v) const { return static_cast<float64>(*(char8*)v); }

    virtual void cast(void* to, void* from, const MetaType* fromType) const{
        float64 d = fromType->toNumber(from);
        memcpy(to, &d, sizeOf());
    }

    virtual void* PlacementNew(void* buffer) const{
        return new(buffer) char8;
    }
    virtual void Dtor(void* v) const{}
    virtual void* New() const{ return new char8; }
    virtual void Delete(void* v) const {}

    virtual const MetaType* DereferenceType() const{
        assert(false && "Cannot Dereference Base Type \'char8\'");
        return nullptr;
    }
    virtual const MetaType* AddressType() const{
        static const MetaType* CharMetaType = this;
        const struct CharStarMetaType : public MetaType{
            virtual const char* name() const { return "char8*"; };
            virtual size_t sizeOf() const { return sizeof(char8*); };
            virtual bool isBaseType() const { return true; };

            virtual const char* toString(void* v) const { return (*(char8**)v); };
            virtual float64 toNumber(void* v) const { return std::stod(*(char8**)v); };

            virtual void cast(void *to, void *from, const MetaType* fromType) const{
                const char* str = fromType->toString(from);
                size_t len = strlen(str)+1;
                char* destbuffer = new char[len];
                memcpy(destbuffer, str, len);
                memcpy(to, &destbuffer, sizeof(const char*));
            }

            virtual void* PlacementNew(void* buffer) const{
                return new (buffer) const char*;
            };

            virtual void Dtor(void* v) const { Delete(v); }; 
            virtual void* New() const{ return new char8*; };
            virtual void Delete(void* v) const{ delete [] *(char8**)v; };

            virtual const MetaType* DereferenceType() const {
                return CharMetaType;
            };
            virtual const MetaType* AddressType() const { 
                assert(false && "Too Many Stars");
                return nullptr;
            };

            virtual void luaSet(lua_State *L, void *v) const{ lua_pushstring(L, *(char8**)v); }
            virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
                const char8* ptr = lua_tostring(L, index); 
                size_t len = strlen(ptr)+1;
                char8* destbuffer = new char8[len];
                memcpy(destbuffer, ptr, len);
                memcpy(v, &destbuffer, sizeOf());
            }

        } l_CharStarMetaType;
        return &l_CharStarMetaType;
    }

    virtual void luaSet(lua_State *L, void* v) const {}
    virtual void luaGet(lua_State *L, int32 index, void* v) const{}

} g_CharMetaType;

/**
 * The Basic Types are the only ones constructed at global scope
 * to get the Metatype of void* for example you need to call 
 * getMetaTypeByType<void>()->AddressType(). In order to accomplish this,
 * this "struct function" defines a Type for a given pointer type
 */
template <typename T>
struct RemoveOneStar{
	typedef T type;
};

template <typename T>
struct RemoveOneStar < T* > {
	typedef typename T type;
};

template <typename T>
struct RemoveOneQualifier{
    typedef typename T type;
};

template <typename T>
struct RemoveOneQualifier < T& > {
	typedef typename T type;
};


template <typename T>
struct RemoveOneQualifier < const T > {
	typedef typename T type;
};

template <typename T>
struct RemoveOneQualifier < T&& > {
	typedef typename T type;
};

template <>
static const MetaType* getMetaTypeByType<void>(){
	return &g_VoidMetaType;
}

template <>
static const MetaType* getMetaTypeByType<bool>(){
	return &g_BoolMetaType;
}

template <>
static const MetaType* getMetaTypeByType<int32>(){
	return &g_IntMetaType;
}

template <>
static const MetaType* getMetaTypeByType<uint32>(){
	return &g_UnsignedIntMetaType;
}

template <>
static const MetaType* getMetaTypeByType<float32>(){
	return &g_FloatMetaType;
}

template <>
static const MetaType* getMetaTypeByType<float64>(){
	return &g_DoubleMetaType;
}

template <>
static const MetaType* getMetaTypeByType<char8>(){
	return &g_CharMetaType;
}

template <typename T>
static const MetaType* getMetaTypeByType<T*>(){
	return getMetaTypeByType<RemoveOneStar<T>::type>()->AddressType();
}


template <>
static const MetaType* getMetaTypeByType<char8 const>(){
    return getMetaTypeByType<char8>();
}



#endif //META_TYPE_H
