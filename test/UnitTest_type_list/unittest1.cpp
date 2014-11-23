#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_type_list
{
    using namespace capo;
    using types_t = types<short, int, unsigned char, long long, float&, const double, long*>;

    template <typename T, typename U>
    struct is_large
         : std::integral_constant<bool, (sizeof(T) > sizeof(U))>
    {};

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(type_list_at)
		{
            {
                using t_t = types_front_t<types_t>;
                Assert::AreEqual(type_name<short>(), type_name<t_t>());
            }
            {
                using t_t = types_back_t<types_t>;
                Assert::AreEqual(type_name<long*>(), type_name<t_t>());
            }
            {
                using t_t = types_at_t<types_t, 3>;
                Assert::AreEqual(type_name<long long>(), type_name<t_t>());
            }
            {
                //using t_t = types_at_t<types_t, types_size<types_t>::value>;
            }
            {
                //using t_t = types_at_t<int, 3>;
            }
		}

		TEST_METHOD(type_list_link)
		{
            {
                using t_t = types_link_t<void, types_t>;
                Assert::AreEqual("struct capo::type_list<void,short,int,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_link_t<types_t, void>;
                Assert::AreEqual("struct capo::type_list<short,int,unsigned char,__int64,float &,double const ,long *,void>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_link_t<types_t, types_t>;
                Assert::AreEqual("struct capo::type_list<short,int,unsigned char,__int64,float &,double const ,long *,"
                                                        "short,int,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_link_t<std::tuple<void*, void**, void***>, types_t>;
                Assert::AreEqual("class std::tuple<void *,void * *,void * * *,short,int,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
		}

		TEST_METHOD(type_list_assign)
		{
            {
                using t_t = types_assign_t<3, int>;
                Assert::AreEqual("struct capo::type_list<int,int,int>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_assign_t<2, types_t>;
                Assert::AreEqual("struct capo::type_list<short,int,unsigned char,__int64,float &,double const ,long *,"
                                                        "short,int,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_assign_t<0, types_t>;
                Assert::AreEqual("struct capo::type_list<>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_assign_t<0, std::tuple<void*>>;
                Assert::AreEqual("class std::tuple<>", type_name<t_t>().c_str());
            }
		}

		TEST_METHOD(type_list_insert)
		{
            {
                using t_t = types_insert_t<types_t, 0, void>;
                Assert::AreEqual("struct capo::type_list<void,short,int,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_insert_t<types_t, types_size<types_t>::value, void>;
                Assert::AreEqual("struct capo::type_list<short,int,unsigned char,__int64,float &,double const ,long *,void>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_insert_t<types_t, 3, void>;
                Assert::AreEqual("struct capo::type_list<short,int,unsigned char,void,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_insert_t<types_t, 2, types<void, void*, void**>>;
                Assert::AreEqual("struct capo::type_list<short,int,void,void *,void * *,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                //using t_t = types_insert_t<types_t, types_size<types_t>::value + 1, void>;
            }
            {
                //using t_t = types_insert_t<int, 1, void>;
            }
		}

		TEST_METHOD(type_list_erase)
		{
            {
                using t_t = types_erase_t<types_t, 0>;
                Assert::AreEqual("struct capo::type_list<int,unsigned char,__int64,float &,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_erase_t<types_t, 2, 3>;
                Assert::AreEqual("struct capo::type_list<short,int,double const ,long *>", type_name<t_t>().c_str());
            }
            {
                using t_t = types_erase_t<types_t, types_size<types_t>::value - 1>;
                Assert::AreEqual("struct capo::type_list<short,int,unsigned char,__int64,float &,double const >", type_name<t_t>().c_str());
            }
            {
                //using t_t = types_erase_t<types_t, -1, 0>;
            }
            {
                //using t_t = types_erase_t<types_t, types_size<types_t>::value>;
            }
            {
                //using t_t = types_erase_t<types<void>, 0, 3>;
            }
            {
                //using t_t = types_erase_t<types<void>, 0, 0>;
            }
            {
                //using t_t = types_erase_t<types<>, 0, 3>;
            }
            {
                //using t_t = types_erase_t<int, 0>;
            }
        }
    };
}