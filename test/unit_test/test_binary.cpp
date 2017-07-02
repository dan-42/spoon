/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_TEST_MODULE test spoon binary
#include <boost/test/included/unit_test.hpp>

#include <iostream>

#include <spoon.hpp>


#define SPOON_TEST_BINARY_SIMPLE_HELPER(name, attribute_type, attribute_value)                         \
                                                                                                       \
BOOST_AUTO_TEST_CASE( test_full_seserialize_test_##name ) {                                            \
                                                                                                       \
  {                                                                                                    \
    attribute_type  value_to_serialize = attribute_value;                                              \
    attribute_type  value_to_deserialize{};                                                            \
                                                                                                       \
    const size_t expected_binary_size  = sizeof(attribute_type);                                       \
                                                                                                       \
    std::vector<uint8_t> binary_data{};                                                                \
    auto successful_serialized = spoon::serialize(binary_data, attribute_type{attribute_value});       \
                                                                                                       \
    BOOST_TEST( successful_serialized == true,              "failed to serialize ");                   \
    BOOST_TEST( binary_data.size() == expected_binary_size, "failed serialized length is wrong" );     \
                                                                                                       \
    auto begin = binary_data.begin();                                                                  \
    auto end   = binary_data.end();                                                                    \
                                                                                                       \
    auto successful_deserialized = spoon::deserialize(begin, end, value_to_deserialize);               \
    BOOST_TEST(successful_deserialized == true,     "failed to successful deserialized");              \
                                                                                                       \
    BOOST_TEST((begin == end), " failed deserialization iterator not at the end" );                    \
                                                                                                       \
    BOOST_TEST(value_to_serialize == value_to_deserialize, " failed attribute must be same ");         \
  }                                                                                                    \
                                                                                                       \
}                                                                                                      \


#define SPOON_TEST_BINARY_HELPER(name, engine, attribute_type, attribute_value, binary_size)           \
                                                                                                       \
BOOST_AUTO_TEST_CASE( test_full_seserialize_test_##name ) {                                            \
                                                                                                       \
  {                                                                                                    \
    decltype(auto)  serializer   = spoon::serializer::engine;                                          \
    decltype(auto)  deserializer = spoon::deserializer::engine;                                        \
    attribute_type  value_to_serialize = attribute_value;                                              \
    attribute_type  value_to_deserialize{};                                                            \
                                                                                                       \
    const size_t expected_binary_size  = binary_size;                                                  \
                                                                                                       \
    std::vector<uint8_t> binary_data{};                                                                \
    auto successful_serialized = spoon::serialize_with(serializer, binary_data, value_to_serialize);   \
                                                                                                       \
    BOOST_TEST( successful_serialized == true,              "failed to serialize ");                   \
    BOOST_TEST( binary_data.size() == expected_binary_size, "failed serialized length is wrong" );     \
                                                                                                       \
    auto begin = binary_data.begin();                                                                  \
    auto end   = binary_data.end();                                                                    \
                                                                                                       \
    auto successful_deserialized = spoon::deserialize_with(deserializer, begin, end, value_to_deserialize); \
    BOOST_TEST(successful_deserialized == true,     "failed to successful deserialized");              \
                                                                                                       \
    BOOST_TEST((begin == end), " failed deserialization iterator not at the end" );                    \
                                                                                                       \
    BOOST_TEST(value_to_serialize == value_to_deserialize, " failed attribute must be same ");         \
  }                                                                                                    \
                                                                                                       \
}                                                                                                      \
                                                                                                       \

BOOST_AUTO_TEST_SUITE( test_spoon_binary )

BOOST_AUTO_TEST_CASE( test_serialize_bool8pos ) {

  using namespace spoon::serializer::binary;
  {
    bool my_bool = true;

    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize_with(bool8, binary_data,  my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{1} );
    BOOST_TEST(binary_data[0] == 0x01);
  }

  {
    bool my_bool = false;

    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize_with(bool8, binary_data, my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{1} );
    BOOST_TEST(binary_data[0] == 0x00);
  }




  {
      uint32_t my = 23;

      std::vector<uint8_t> binary_data{};


      auto success = spoon::serialize_with(big::uint32, binary_data, my);
      BOOST_TEST(success == true);

      BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{4} );
      BOOST_TEST(binary_data[3] == 23);
  }


}





BOOST_AUTO_TEST_CASE( test_deserialize_bool8pos ) {

  using spoon::deserializer::binary::bool8;
  {
    bool my_bool = false;

    std::vector<uint8_t> binary_data{};
    binary_data.push_back(0x01);

    auto begin = binary_data.begin();
    auto end   = binary_data.end();

    auto success = spoon::deserialize_with(bool8, begin, end, my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST((begin == end), " failed iterator not same" );
    BOOST_TEST(my_bool == true, " failed attribute mus be false");
  }

   {
     bool my_bool = true;

     std::vector<uint8_t> binary_data{};
     binary_data.push_back(0x00);

     auto begin = binary_data.begin();
     auto end   = binary_data.end();

     auto success = spoon::deserialize_with(bool8, begin, end, my_bool);
     BOOST_TEST(success == true);

     BOOST_TEST(( begin == end), "" );
     BOOST_TEST(my_bool == false);
   }


}






BOOST_AUTO_TEST_CASE( test_full_seserialize_test ) {

  {
    decltype(auto)  serializer   = spoon::serializer::binary::bool8;
    decltype(auto)  deserializer = spoon::deserializer::binary::bool8;
    bool   value_to_serialize = false;
    bool  value_to_deserialize{};

    const size_t expected_binary_size  = 1;

    std::vector<uint8_t> binary_data{};
    auto successful_serialized = spoon::serialize_with(serializer, binary_data, value_to_serialize);

    BOOST_TEST( successful_serialized == true,              "failed to serialize ");
    BOOST_TEST( binary_data.size() == expected_binary_size, "failed serialized length is wrong" );



    auto begin = binary_data.begin();
    auto end   = binary_data.end();

    auto successful_deserialized = spoon::deserialize_with(deserializer, begin, end, value_to_deserialize);
    BOOST_TEST(successful_deserialized == true,     "failed to successful deserialized");

    BOOST_TEST((begin == end), " failed deserialization iterator not at the end" );

    BOOST_TEST(value_to_serialize == value_to_deserialize, " failed attribute must be same ");
  }

}



SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_1, uint8_t,    0)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_2, uint8_t,    127)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_3, uint8_t,    255)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_4, int8_t,     0)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_5, int8_t,     -1)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_6, int8_t,     -127)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_1, uint16_t,    0)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_2, uint16_t,    32999)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_3, uint16_t,    65000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_4, int16_t,    -1)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_5, int16_t,    -31000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_6, int16_t,    -127)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_1, uint32_t,    800000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_2, uint32_t,     42)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_3, int32_t,    -127)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_4, int32_t,    -800000)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint64_1, uint64_t,    99800000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint64_2, int64_t,    -99800000)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_float,   float,      -3.14f)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_double,  double,     -3.14d)



// uint8_
SPOON_TEST_BINARY_HELPER(uint8_1, binary::uint8, uint8_t, 0,     1)
SPOON_TEST_BINARY_HELPER(uint8_2, binary::uint8, uint8_t, 23,    1)
SPOON_TEST_BINARY_HELPER(uint8_3, binary::uint8, uint8_t, 128,   1)
SPOON_TEST_BINARY_HELPER(uint8_4, binary::uint8, uint8_t, 255,   1)
SPOON_TEST_BINARY_HELPER(uint8_5, binary::int8,  int8_t, -1,    1)
SPOON_TEST_BINARY_HELPER(uint8_6, binary::int8,  int8_t, -127,  1)

//uint16
SPOON_TEST_BINARY_HELPER(uint16_1, binary::big::uint16, uint16_t, 0,       2)
SPOON_TEST_BINARY_HELPER(uint16_2, binary::big::uint16, uint16_t, 23,      2)
SPOON_TEST_BINARY_HELPER(uint16_3, binary::big::uint16, uint16_t, 65534,   2)
SPOON_TEST_BINARY_HELPER(uint16_4, binary::big::uint16, uint16_t, 65535,   2)
SPOON_TEST_BINARY_HELPER(uint16_5, binary::big::uint16, uint16_t, -1   ,   2)
SPOON_TEST_BINARY_HELPER(uint16_6, binary::big::uint16, uint16_t, -32001,  2)
SPOON_TEST_BINARY_HELPER(uint16_7, binary::big::int16,  int16_t, -32001,  2)

//uint24
SPOON_TEST_BINARY_HELPER(uint24_1, binary::big::uint24, uint32_t, 0,       3)
SPOON_TEST_BINARY_HELPER(uint24_2, binary::big::uint24, uint32_t, 23,      3)
SPOON_TEST_BINARY_HELPER(uint24_3, binary::big::uint24, uint32_t, 0x0FFFFF,3)
SPOON_TEST_BINARY_HELPER(uint24_4, binary::big::uint24, uint32_t, 0xFFFFFF,3)

//uint32
SPOON_TEST_BINARY_HELPER(uint32_1, binary::big::uint32, uint32_t, 0,       4)


//uint40
SPOON_TEST_BINARY_HELPER(uint40_1, binary::big::uint40, uint64_t, 0,       5)


//uint48
SPOON_TEST_BINARY_HELPER(uint48_1, binary::big::uint48, uint64_t, 0,       6)


//uint56
SPOON_TEST_BINARY_HELPER(uint56_1, binary::big::uint56, uint64_t, 0,       7)


//uint64
SPOON_TEST_BINARY_HELPER(uint64_1, binary::big::uint64, uint64_t, 0,       8)


//float
SPOON_TEST_BINARY_HELPER(float_1, binary::big::float32, float, 3.14f,      4)

//double_
SPOON_TEST_BINARY_HELPER(double_1, binary::big::float64, double, 3.14,      8)

BOOST_AUTO_TEST_SUITE_END()
