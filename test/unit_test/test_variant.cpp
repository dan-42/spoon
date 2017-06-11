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
/*
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_binary.hpp>
#include <boost/variant.hpp>
*/
BOOST_AUTO_TEST_SUITE( test_spoon_variant )



BOOST_AUTO_TEST_CASE( test_spoon_variant_simple_1 ) {
/*
//using variant_t = boost::variant<bool, uint32_t, int16_t, double>;
  using variant_t = boost::variant<int32_t, int16_t>;

variant_t var = int32_t{1337};

std::vector<uint8_t> binary_data{};
auto sink = std::back_insert_iterator<decltype(binary_data)>(binary_data);

//auto success = boost::spirit::karma::generate(sink, (boost::spirit::karma::bool_ | boost::spirit::karma::big_dword | boost::spirit::karma::big_word  | boost::spirit::karma::big_bin_double), var);
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

  std::vector<uint8_t> binary_data{};

  using varinat_t = mapbox::util::variant<bool, uint32_t, double>;

  {
    using namespace spoon::serializer;

    varinat_t var = uint32_t{1337};
   // decltype(auto) engine = any_(binary::bool_, binary::big::word32_, binary::big::double_);
    decltype(auto) engine = any_type_(bool{}, uint32_t{}, double{});
   // decltype(auto) engine = any_type_(varinat_t);

    auto success = spoon::serialize_with(engine, binary_data, var);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{4} );
    BOOST_TEST(binary_data[0] == 0x00);
    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x05);
    BOOST_TEST(binary_data[3] == 0x39);
  }






}


BOOST_AUTO_TEST_SUITE_END()
