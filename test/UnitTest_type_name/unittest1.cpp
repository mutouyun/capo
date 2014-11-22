#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_type_name
{		
    class Foo {};

    template <typename T>
    auto func(T&&)->T;

    TEST_CLASS(UnitTest1)
	{
	public:
        TEST_METHOD(TestMethod_cv_qualified_compound_types)
		{
            Assert::AreEqual("void const volatile *"          , capo::type_name<const volatile void *>().c_str());
            Assert::AreEqual("void const volatile * (&) [10]" , capo::type_name<const volatile void *(&)[10]>().c_str());
            Assert::AreEqual("int [1] [2] [3]"                , capo::type_name<int[1][2][3]>().c_str());
            Assert::AreEqual("int const [3] [10]"             , capo::type_name<const int[3][10]>().c_str());
            Assert::AreEqual("int (unsigned int)"             , capo::type_name<int(unsigned)>().c_str());
        }

        TEST_METHOD(TestMethod_function)
        {
            Assert::AreEqual("int (*) (int const * (*) [3] [10], class UnitTest_type_name::Foo &&, int, unsigned int)",
              capo::type_name<int(*)(const int *(*)[3][10], Foo&&, int, unsigned)>().c_str());
            Assert::AreEqual("char (* (* const) (int const (&) [10])) [10]",
              capo::type_name<char(*(*const)(const int(&)[10]))[10]>().c_str());
        }

        TEST_METHOD(TestMethod_type_with_class)
        {
            Assert::AreEqual("int class UnitTest_type_name::Foo::* const (&) []", 
              capo::type_name<int Foo::* const(&)[]>().c_str());
            Assert::AreEqual("void (class UnitTest_type_name::Foo::*) ()", 
              capo::type_name<void(Foo::*)(void)>().c_str());
            Assert::AreEqual("void (class UnitTest_type_name::Foo::*) () const volatile", 
              capo::type_name<void(Foo::*)(void) volatile const>().c_str());
            Assert::AreEqual("void (class UnitTest_type_name::Foo::* (*) []) (int) const", 
              capo::type_name<void(Foo::*(*)[])(int) const>().c_str());
            Assert::AreEqual("int (class UnitTest_type_name::Foo::* const) (int, class UnitTest_type_name::Foo &&, int) volatile", 
              capo::type_name<int (Foo::* const)(int, Foo&&, int) volatile>().c_str());
        }

        TEST_METHOD(TestMethod_reference)
        {
            Assert::AreEqual("class UnitTest_type_name::Foo (class UnitTest_type_name::Foo &&)", 
              capo::type_name<decltype(func<Foo>)>().c_str());
            Assert::AreEqual("class UnitTest_type_name::Foo & (class UnitTest_type_name::Foo &)", 
              capo::type_name<decltype(func<Foo&>)>().c_str());
            Assert::AreEqual("class UnitTest_type_name::Foo && (class UnitTest_type_name::Foo &&)", 
              capo::type_name<decltype(func<Foo&&>)>().c_str());
        }
    };
}