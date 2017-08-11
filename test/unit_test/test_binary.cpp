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
#include <spoon/binary.hpp>


#define SPOON_TEST_BINARY_SIMPLE_HELPER(name, attribute_type, attribute_value, engine)                 \
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
    auto successful_serialized = spoon::serialize(binary_data, engine, value_to_serialize);            \
                                                                                                       \
    BOOST_TEST( successful_serialized == true,              "failed to serialize ");                   \
    BOOST_TEST( binary_data.size() == expected_binary_size, "failed serialized length is wrong" );     \
                                                                                                       \
    auto begin = binary_data.begin();                                                                  \
    auto end   = binary_data.end();                                                                    \
                                                                                                       \
    auto successful_deserialized = spoon::deserialize(begin, end, engine, value_to_deserialize);       \
    BOOST_TEST(successful_deserialized == true,     "failed to successful deserialized");              \
                                                                                                       \
    BOOST_TEST((begin == end), " failed deserialization iterator not at the end" );                    \
                                                                                                       \
    BOOST_TEST(value_to_serialize == value_to_deserialize, " failed attribute must be same ");         \
  }                                                                                                    \
                                                                                                       \
}                                                                                                      \



BOOST_AUTO_TEST_SUITE( test_spoon_binary )

BOOST_AUTO_TEST_CASE( test_serialize_bool8pos ) {


  {
    bool my_bool = true;
    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize(binary_data,  spoon::big_endian::bool8, my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{1} );
    BOOST_TEST(binary_data[0] == 0x01);
  }

  {
    bool my_bool = false;

    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize(binary_data, spoon::big_endian::bool8, my_bool);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{1} );
    BOOST_TEST(binary_data[0] == 0x00);
  }




  {
    uint32_t my = 23;

    std::vector<uint8_t> binary_data{};


    auto success = spoon::serialize(binary_data, spoon::big_endian::uint32, my);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == decltype(binary_data)::size_type{4} );
    BOOST_TEST(binary_data[3] == 23);
  }


}

//
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_1, uint8_t,    0,    spoon::big_endian::uint8)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_2, uint8_t,    127,    spoon::big_endian::uint8)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_3, uint8_t,    255,    spoon::big_endian::uint8)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_4, int8_t,     0,    spoon::uint8)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_5, int8_t,     -1,    spoon::uint8)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint8_6, int8_t,     -127,    spoon::uint8)
//
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_1, uint16_t,    0,    spoon::big_endian::uint16)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_2, uint16_t,    32999,    spoon::big_endian::uint16)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_3, uint16_t,    65000,    spoon::big_endian::uint16)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_4, int16_t,    -1,    spoon::uint16)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_5, int16_t,    -31000,    spoon::uint16)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint16_6, int16_t,    -127,    spoon::uint16)
//
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_1, uint32_t,    800000,    spoon::big_endian::uint32)
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_2, uint32_t,     42,    spoon::big_endian::uint32)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_3, int32_t,    -127,    spoon::big_endian::uint32)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint32_4, int32_t,    -800000,    spoon::big_endian::uint32)
//
SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint64_1, uint64_t,    99800000,    spoon::big_endian::uint64)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_uint64_2, int64_t,    -99800000,    spoon::big_endian::uint64)
//
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_float,   float,      -3.14f,    spoon::float32)
//SPOON_TEST_BINARY_SIMPLE_HELPER(simple_double,  double,     -3.14d,    spoon::float64)
//

BOOST_AUTO_TEST_SUITE_END()
