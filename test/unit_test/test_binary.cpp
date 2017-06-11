/**
 * ninja-wooki, is a BACnet stack C++ library
 *
 * Copyright (C) 2015 Daniel Friedrich
 *
 * This file is part of ninja-wooki.
 *
 * ninja-wooki is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ninja-wooki is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Mupen64PlusAE. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Daniel Friedrich
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
    const size_t expected_binary_size  = sizeof(attribute_type);                                      \
                                                                                                       \
    std::vector<uint8_t> binary_data{};                                                                \
    auto successful_serialized = spoon::serialize(binary_data, value_to_serialize);                    \
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

BOOST_AUTO_TEST_CASE( test_serialize_bool_pos ) {

  using namespace spoon::serializer::binary;
  {
    bool my_bool = true;

    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize_with(byte_, binary_data,  my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{1} );
    BOOST_TEST(binary_data[0] == 0x01);
  }

  {
    bool my_bool = false;

    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize_with(byte_, binary_data, my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{1} );
    BOOST_TEST(binary_data[0] == 0x00);
  }




  {
      uint32_t my = 23;

      std::vector<uint8_t> binary_data{};


      auto success = spoon::serialize_with(big::word32_, binary_data, my);
      BOOST_TEST(success == true);

      BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{4} );
      BOOST_TEST(binary_data[3] == 23);
  }


}





BOOST_AUTO_TEST_CASE( test_deserialize_bool_pos ) {

  using spoon::deserializer::binary::byte_;
  {
    bool my_bool = false;

    std::vector<uint8_t> binary_data{};
    binary_data.push_back(0x01);

    auto begin = binary_data.begin();
    auto end   = binary_data.end();

    auto success = spoon::deserialize_with(byte_, begin, end, my_bool);
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

     auto success = spoon::deserialize_with(byte_, begin, end, my_bool);
     BOOST_TEST(success == true);

     BOOST_TEST(( begin == end), "" );
     BOOST_TEST(my_bool == false);
   }


}






BOOST_AUTO_TEST_CASE( test_full_seserialize_test ) {

  {
    decltype(auto)  serializer   = spoon::serializer::binary::byte_;
    decltype(auto)  deserializer = spoon::deserializer::binary::byte_;
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



SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word8_1, uint8_t,    0)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word8_2, uint8_t,    127)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word8_3, uint8_t,    255)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word8_4, int8_t,     0)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word8_5, int8_t,     -1)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word8_6, int8_t,     -127)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word16_1, uint16_t,    0)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word16_2, uint16_t,    32999)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word16_3, uint16_t,    65000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word16_4, int16_t,    -1)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word16_5, int16_t,    -31000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word16_6, int16_t,    -127)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word32_1, uint32_t,    800000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word32_2, uint32_t,     42)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word32_3, int32_t,    -127)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word32_4, int32_t,    -800000)

SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word64_1, uint64_t,    99800000)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_word64_2, int64_t,    -99800000)


SPOON_TEST_BINARY_SIMPLE_HELPER(simple_float, float,    -3.14f)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_double, double,    -3.14d)

// word8_
SPOON_TEST_BINARY_HELPER(word8_1, binary::big::word8_, uint8_t, 0,     1)
SPOON_TEST_BINARY_HELPER(word8_2, binary::big::word8_, uint8_t, 23,    1)
SPOON_TEST_BINARY_HELPER(word8_3, binary::big::word8_, uint8_t, 128,   1)
SPOON_TEST_BINARY_HELPER(word8_4, binary::big::word8_, uint8_t, 255,   1)
SPOON_TEST_BINARY_HELPER(word8_5, binary::big::word8_,  int8_t, -1,    1)
SPOON_TEST_BINARY_HELPER(word8_6, binary::big::word8_,  int8_t, -127,  1)

//word16
SPOON_TEST_BINARY_HELPER(word16_1, binary::big::word16_, uint16_t, 0,       2)
SPOON_TEST_BINARY_HELPER(word16_2, binary::big::word16_, uint16_t, 23,      2)
SPOON_TEST_BINARY_HELPER(word16_3, binary::big::word16_, uint16_t, 65534,   2)
SPOON_TEST_BINARY_HELPER(word16_4, binary::big::word16_, uint16_t, 65535,   2)
SPOON_TEST_BINARY_HELPER(word16_5, binary::big::word16_, uint16_t, -1   ,   2)
SPOON_TEST_BINARY_HELPER(word16_6, binary::big::word16_, uint16_t, -32001,  2)
SPOON_TEST_BINARY_HELPER(word16_7, binary::big::word16_,  int16_t, -32001,  2)

//word24
SPOON_TEST_BINARY_HELPER(word24_1, binary::big::word24_, uint32_t, 0,       3)
SPOON_TEST_BINARY_HELPER(word24_2, binary::big::word24_, uint32_t, 23,      3)
SPOON_TEST_BINARY_HELPER(word24_3, binary::big::word24_, uint32_t, 0x0FFFFF,3)
SPOON_TEST_BINARY_HELPER(word24_4, binary::big::word24_, uint32_t, 0xFFFFFF,3)

//word32
SPOON_TEST_BINARY_HELPER(word32_1, binary::big::word32_, uint32_t, 0,       4)


//word40
SPOON_TEST_BINARY_HELPER(word40_1, binary::big::word40_, uint64_t, 0,       5)


//word48
SPOON_TEST_BINARY_HELPER(word48_1, binary::big::word48_, uint64_t, 0,       6)


//word56
SPOON_TEST_BINARY_HELPER(word56_1, binary::big::word56_, uint64_t, 0,       7)


//word64
SPOON_TEST_BINARY_HELPER(word64_1, binary::big::word64_, uint64_t, 0,       8)


//float
SPOON_TEST_BINARY_HELPER(float_1, binary::big::float_, float, 3.14f,      4)

//double_
SPOON_TEST_BINARY_HELPER(double_1, binary::big::double_, double, 3.14,      8)

BOOST_AUTO_TEST_SUITE_END()
