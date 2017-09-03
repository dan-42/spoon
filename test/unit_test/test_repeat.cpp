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


#define BOOST_TEST_MODULE test spoon repeat
#include <boost/test/included/unit_test.hpp>

#include <boost/fusion/adapted/struct.hpp>
#include <iostream>
#include <vector>

#include <spoon.hpp>
#include <spoon/repeat.hpp>
#include <spoon/binary.hpp>


using my_list_t = std::vector<uint32_t>;

BOOST_AUTO_TEST_SUITE( test_spoon_repeat )


BOOST_AUTO_TEST_CASE( test_spoon_repeat_simple ) {

  std::vector<uint8_t> binary_data{};

  {
    my_list_t list;
    list.emplace_back(23);
    list.emplace_back(42);
    list.emplace_back(1337);
    list.emplace_back(3141592653);
    list.emplace_back(1984);


    //decltype(auto) engine = seq_(bool{}, uint32_t{}, double{});
    auto success = spoon::serialize(binary_data, spoon::repeat[spoon::big_endian::uint32], list);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{list.size() * 4 } , "spoon::repeat error min > max");

    size_t offset = 0;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x00);
    BOOST_TEST(binary_data[offset + 3] == 0x17);

    offset += 4;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x00);
    BOOST_TEST(binary_data[offset + 3] == 0x2A);

    offset += 4;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x05);
    BOOST_TEST(binary_data[offset + 3] == 0x39);

    offset += 4;  //BB 40 E6 4D
    BOOST_TEST(binary_data[offset + 0] == 0xBB);
    BOOST_TEST(binary_data[offset + 1] == 0x40);
    BOOST_TEST(binary_data[offset + 2] == 0xE6);
    BOOST_TEST(binary_data[offset + 3] == 0x4D);

    offset += 4; //07 C0
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x07);
    BOOST_TEST(binary_data[offset + 3] == 0xc0);

  }


  {
    my_list_t list;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, spoon::repeat[spoon::big_endian::uint32], list);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");
    BOOST_TEST(list.size()  == std::size_t{5});
    BOOST_TEST(list[0]      == std::size_t{23});
    BOOST_TEST(list[1]      == std::size_t{42});
    BOOST_TEST(list[2]      == std::size_t{1337});
    BOOST_TEST(list[3]      == std::size_t{3141592653});
    BOOST_TEST(list[4]      == std::size_t{1984});
  }


}



BOOST_AUTO_TEST_CASE( test_spoon_repeat_min_max ) {

  std::vector<uint8_t> binary_data{};

  {
    my_list_t list;
    list.emplace_back(23);
    list.emplace_back(42);
    list.emplace_back(1337);
    list.emplace_back(3141592653);
    list.emplace_back(1984);


    //decltype(auto) engine = seq_(bool{}, uint32_t{}, double{});
    auto success = spoon::serialize(binary_data, spoon::repeat[spoon::big_endian::uint32](1, 5), list);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{list.size() * 4 } , "spoon::repeat error min > max");

    size_t offset = 0;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x00);
    BOOST_TEST(binary_data[offset + 3] == 0x17);

    offset += 4;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x00);
    BOOST_TEST(binary_data[offset + 3] == 0x2A);

    offset += 4;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x05);
    BOOST_TEST(binary_data[offset + 3] == 0x39);

    offset += 4;  //BB 40 E6 4D
    BOOST_TEST(binary_data[offset + 0] == 0xBB);
    BOOST_TEST(binary_data[offset + 1] == 0x40);
    BOOST_TEST(binary_data[offset + 2] == 0xE6);
    BOOST_TEST(binary_data[offset + 3] == 0x4D);

    offset += 4; //07 C0
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x07);
    BOOST_TEST(binary_data[offset + 3] == 0xc0);

  }


  {
    my_list_t list;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, spoon::repeat[spoon::big_endian::uint32], list);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");
    BOOST_TEST(list.size()  == std::size_t{5});
    BOOST_TEST(list[0]      == std::size_t{23});
    BOOST_TEST(list[1]      == std::size_t{42});
    BOOST_TEST(list[2]      == std::size_t{1337});
    BOOST_TEST(list[3]      == std::size_t{3141592653});
    BOOST_TEST(list[4]      == std::size_t{1984});
  }


}


BOOST_AUTO_TEST_CASE( test_spoon_repeat_min_max_2 ) {

  std::vector<uint8_t> binary_data{};

  {
    my_list_t list;
    list.emplace_back(23);
    list.emplace_back(42);
    list.emplace_back(1337);
    list.emplace_back(3141592653);
    list.emplace_back(1984);


    //decltype(auto) engine = seq_(bool{}, uint32_t{}, double{});
    auto success = spoon::serialize(binary_data, spoon::repeat[spoon::big_endian::uint32](3, 5), list);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{list.size() * 4 } , "spoon::repeat error min > max");

    size_t offset = 0;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x00);
    BOOST_TEST(binary_data[offset + 3] == 0x17);

    offset += 4;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x00);
    BOOST_TEST(binary_data[offset + 3] == 0x2A);

    offset += 4;
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x05);
    BOOST_TEST(binary_data[offset + 3] == 0x39);

    offset += 4;  //BB 40 E6 4D
    BOOST_TEST(binary_data[offset + 0] == 0xBB);
    BOOST_TEST(binary_data[offset + 1] == 0x40);
    BOOST_TEST(binary_data[offset + 2] == 0xE6);
    BOOST_TEST(binary_data[offset + 3] == 0x4D);

    offset += 4; //07 C0
    BOOST_TEST(binary_data[offset + 0] == 0x00);
    BOOST_TEST(binary_data[offset + 1] == 0x00);
    BOOST_TEST(binary_data[offset + 2] == 0x07);
    BOOST_TEST(binary_data[offset + 3] == 0xc0);

  }


  {
    my_list_t list;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    auto  success = spoon::deserialize(start, end, spoon::repeat[spoon::big_endian::uint32], list);
    BOOST_TEST(success      == true);
    BOOST_TEST((start       == end), "start != end");
    BOOST_TEST(list.size()  == std::size_t{5});
    BOOST_TEST(list[0]      == std::size_t{23});
    BOOST_TEST(list[1]      == std::size_t{42});
    BOOST_TEST(list[2]      == std::size_t{1337});
    BOOST_TEST(list[3]      == std::size_t{3141592653});
    BOOST_TEST(list[4]      == std::size_t{1984});
  }


}



BOOST_AUTO_TEST_SUITE_END()
