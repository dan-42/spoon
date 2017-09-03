/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#define BOOST_TEST_MODULE test spoon binary
#include <boost/test/included/unit_test.hpp>

#include <iostream>

#include <spoon.hpp>
#include <spoon/any.hpp>
#include <spoon/binary.hpp>

#include <boost/variant.hpp>

//using varinat_t = mapbox::util::variant<bool, uint32_t, double>;
using varinat_t = boost::variant<bool, uint32_t, uint16_t>;



BOOST_AUTO_TEST_SUITE( test_spoon_variant )



BOOST_AUTO_TEST_CASE( test_spoon_variant_simple_1 ) {
/*
//using variant_t = boost::variant<bool, uint32_t, int16_t, double>;
  using variant_t = boost::variant<int32_t, int16_t>;

variant_t var = int32_t{1337};

std::vector<uint8_t> binary_data{};
auto sink = std::back_insert_iterator<decltype(binary_data)>(binary_data);

//auto success = boost::spirit::karma::generate(sink, (boost::spirit::karma::bool8 | boost::spirit::karma::big_dword | boost::spirit::karma::big_word  | boost::spirit::karma::big_bin_double), var);
auto success = boost::spirit::karma::generate(sink, (boost::spirit::karma::big_dword | boost::spirit::karma::big_word), var);
BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{4});
    BOOST_TEST(binary_data[0] == 0x00);
    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x05);
    BOOST_TEST(binary_data[3] == 0x39);
*/
}


BOOST_AUTO_TEST_CASE( test_spoon_variant_simple ) {


  //constexpr auto engine = spoon::any<varinat_t>( spoon::big_endian::bool8,  spoon::big_endian::uint32, spoon::big_endian::uint16);
  constexpr auto engine = spoon::any<varinat_t>( spoon::big_endian::uint32, spoon::big_endian::uint16, spoon::big_endian::bool8);
  std::vector<uint8_t> binary_data{};

  {
    varinat_t var = uint32_t{1337};
    auto success = spoon::serialize(binary_data, engine, var);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{4} );
    BOOST_TEST(binary_data[0] == 0x00);
    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x05);
    BOOST_TEST(binary_data[3] == 0x39);
  }
  {
    varinat_t var{};
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, engine, var);

    BOOST_TEST(success == true);
    BOOST_TEST((start == end), "start != end");

    struct result_visitor : public boost::static_visitor<> {
      void operator()(bool r) const     { BOOST_TEST(false,               " it is bool"); }
      void operator()(uint16_t e) const { BOOST_TEST(e == uint16_t{1337}, " it is uint16_t but value wrong"); }
      void operator()(uint32_t e) const { BOOST_TEST(e == uint32_t{1337}, " it is uint but value wrong"); }
    };

    result_visitor visitor;
    boost::apply_visitor(visitor, var);
   // mapbox::util::apply_visitor(visitor, var);
  }

}


/*
 *
 *

 // decltype(auto) engine = anytype_(varinat_t);

decltype(auto) engine = anytype_(bool{}, uint32_t{}, double{});

//decltype(auto) engine_fail    = any(binary::bool8, binary::big::uint32, binary::big::float32);
    //decltype(auto) engine_success = any(binary::big::float32, binary::big::uint32, binary::bool8);

 * /auto success = spoon::deserialize_with(engine_fail, start, end, var);
 */










BOOST_AUTO_TEST_SUITE_END()
