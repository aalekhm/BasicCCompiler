#include <iostream>
#include "MetaVariable.h"
#include "MetaFunction.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<< 1. >>>>>>>>>>>>>>>>>>>>>>>>
// MetaType - Encapsulates the various 'Data' types.
// <<<<<<<<<<<<<<<<<<<<<<<<<< 2. >>>>>>>>>>>>>>>>>>>>>>>>
// MetaVariable - Encapsulated a variables name, value, data type.
// <<<<<<<<<<<<<<<<<<<<<<<<<< 3. >>>>>>>>>>>>>>>>>>>>>>>>
// AutoLister - A Utility struct to save all the registered items in a link list.
// <<<<<<<<<<<<<<<<<<<<<<<<<< 4. >>>>>>>>>>>>>>>>>>>>>>>>
// FunctionSignature - Encapsulates the various 'Function' types.
// <<<<<<<<<<<<<<<<<<<<<<<<<< 5. >>>>>>>>>>>>>>>>>>>>>>>>
// Variable
// <<<<<<<<<<<<<<<<<<<<<<<<<< 6. >>>>>>>>>>>>>>>>>>>>>>>>
// Apply
// <<<<<<<<<<<<<<<<<<<<<<<<<< 7. >>>>>>>>>>>>>>>>>>>>>>>>
// MetaFunction

void void_Foo_void() 
{ 
	std::cout << "In void_Foo_void()" << std::endl; 
}

void void_Foo_int32(int32_t i) 
{ 
	std::cout << "In void_Foo_int32(" << i << ")" << std::endl; 
}

float float_Foo2_int16_float(int16_t i, float f) 
{ 
	std::cout << "In float_Foo2_int16_float(" << i << ", " << f << ")" << std::endl; return 2 * f; 
}

void void_Func_int32_charStar(int32_t iVal, const char* sStr)
{
	printf("In void_Func_int32_charStar(%d), sStr(%s)\n", iVal, sStr);
}


int main(int argc, char* argv[])
{
	// <<<<<<<<<<<<<<<<<<<<<<<<<< 1. >>>>>>>>>>>>>>>>>>>>>>>>
	{
		int8_t i8 = 123;
		int16_t i16 = 1234;
		int32_t i32 = 12345;
		int64_t i64 = 123456;
		double fDouble = 12.34f;
		float fFloat = 56.78f;
		const char* sStr = "Hello World!";

		META_REGISTER_VAR(i8);
		META_REGISTER_VAR(i16);
		META_REGISTER_VAR(i32);
		META_REGISTER_VAR(i64);
		META_REGISTER_VAR(fDouble);
		META_REGISTER_VAR(fFloat);
		META_REGISTER_VAR(sStr);

		MetaPrintVariables(std::cout);
		std::cout << std::endl;
	}

	// <<<<<<<<<<<<<<<<<<<<<<<<<< 7. >>>>>>>>>>>>>>>>>>>>>>>>
	{
		META_REGISTER_FUN(void_Foo_void);
		META_REGISTER_FUN(void_Foo_int32);
		META_REGISTER_FUN(float_Foo2_int16_float);
		META_REGISTER_FUN(void_Func_int32_charStar);

		MetaPrintFunctions(std::cout);
		std::cout << std::endl;
	}

	// Applying Function
	{
		GetFunctionByName("void_Foo_void")->apply(nullptr, nullptr, 0);

		int32_t iInt32 = 123456;
		GetFunctionByName("void_Foo_int32")->apply(nullptr, &Variable(iInt32), 1);

		int16_t iInt16 = 1234;
		float fRet = 0;
		float fFloat = 12.34;
		Variable args[] = { iInt16, fFloat };
		GetFunctionByName("float_Foo2_int16_float")->apply(fRet, args, 2);
		
		const char* sStr = "HellowWorld";
		Variable args_1[] = { iInt32, sStr };
		GetFunctionByName("void_Func_int32_charStar")->apply(nullptr, args_1, 2);
	}

	exit(EXIT_SUCCESS);
}