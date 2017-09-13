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


#define BOOST_TEST_MODULE test spoon optional
#include <boost/test/included/unit_test.hpp>

#include <boost/optional.hpp>
#include <iostream>
#include <vector>

#include <spoon.hpp>
#include <spoon/optional.hpp>
#include <spoon/binary.hpp>

#include <boost/optional/optional_io.hpp>


using my_type = boost::optional<uint32_t>;




BOOST_AUTO_TEST_SUITE( test_spoon_optional )





BOOST_AUTO_TEST_CASE( test_spoon_optional_simple_empty_optional ) {

  std::vector<uint8_t> binary_data{};

  {
    my_type my;
    constexpr auto engine = spoon::optional[spoon::big_endian::uint32];
    auto success = spoon::serialize(binary_data, engine, my);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{ 0 } , "spoon::optional size error ");

  }
  {
    my_type my;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, spoon::optional[spoon::big_endian::uint32], my);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");
    if(!my) {
      BOOST_TEST(true);
    }
    else {
      BOOST_TEST(false);
    }


  }
}

BOOST_AUTO_TEST_CASE( test_spoon_optional_simple_filled_optional ) {

  std::vector<uint8_t> binary_data{};

  {
    my_type my;
    my = 23;
    //decltype(auto) engine = seq_(bool{}, uint32_t{}, double{});
    auto success = spoon::serialize(binary_data, spoon::optional[spoon::big_endian::uint32], my);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{ 4 } , "spoon::optional size error ");

    BOOST_TEST(binary_data[0] == 0x00);
    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x00);
    BOOST_TEST(binary_data[3] == 0x17);
  }
  {
    my_type my;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, spoon::optional[spoon::big_endian::uint32], my);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");
    BOOST_TEST((my));
    BOOST_TEST(my.value() == uint32_t{23});
  }
}


BOOST_AUTO_TEST_CASE( test_spoon_optional_simple_with_expecting_with_data_true_true) {

  std::vector<uint8_t> binary_data{};

  {
    my_type my;
    my = 23;

    auto success = spoon::serialize(binary_data, spoon::optional[spoon::big_endian::uint32]([](){return true;}), my);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{ 4 } , "spoon::optional size error ");

  }
  {
    my_type my;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, spoon::optional[spoon::big_endian::uint32]([](){return true;}), my);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");

    BOOST_TEST(!!my);
  }
}

BOOST_AUTO_TEST_CASE( test_spoon_optional_simple_with_expecting_no_data_false_false) {

  std::vector<uint8_t> binary_data{};

  {
    my_type my;

    auto success = spoon::serialize(binary_data, spoon::optional[spoon::big_endian::uint32]([](){return false;}), my);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{ 0 } , "spoon::optional size error ");

  }
  {
    my_type my;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();
    auto  success = spoon::deserialize(start, end, spoon::optional[spoon::big_endian::uint32]([](){return false;}), my);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");
    BOOST_TEST(!my);
  }
}

BOOST_AUTO_TEST_CASE( test_spoon_optional_simple_with_expecting_with_data_false_false) {

  std::vector<uint8_t> binary_data{};

  {
    my_type my;
    my = 23;
    auto success = spoon::serialize(binary_data, spoon::optional[spoon::big_endian::uint32]([](){return false;}), my);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{ 0 } , "spoon::optional size error ");

  }
  {
    my_type my;
    binary_data.push_back(0x00);
    binary_data.push_back(0x00);
    binary_data.push_back(0x00);
    binary_data.push_back(0x17);
    auto     start = binary_data.begin();
    const auto end = binary_data.end();
    auto  success = spoon::deserialize(start, end, spoon::optional[spoon::big_endian::uint32]([](){return false;}), my);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       != end), "start != end");
    BOOST_TEST(!my);
  }
}

BOOST_AUTO_TEST_SUITE_END()
