/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#define BOOST_TEST_MODULE test spoon gear
#include <boost/test/included/unit_test.hpp>

#include <iostream>

#include <spoon.hpp>
#include <spoon/engine/gear.hpp>
#include <spoon.hpp>

uint32_t storage;

struct simple_gear :  spoon::engine::gear<simple_gear, uint32_t>{


  template<typename Sink, typename Attr>
  inline auto serialize(bool& pass, Sink& sink, const Attr& attr) const -> void {
    storage = attr;
  }

  template<typename Itr, typename Attr>
  inline auto deserialize(bool& pass, Itr& start, const Itr& end, Attr& attr) const -> void {
    attr = storage;
  }


};


BOOST_AUTO_TEST_SUITE( test_spoon_gear )


BOOST_AUTO_TEST_CASE( test_spoon_gear_simple ) {

  std::vector<uint8_t> sink;
  auto start = std::begin(sink);
  auto end   = std::end(sink);
  uint32_t my_attr{123};

  using my_gear_type = simple_gear;

  constexpr auto my_gear = my_gear_type{};
  spoon::serialize(sink, my_gear, my_attr);
  spoon::deserialize(start, end, my_gear, my_attr);
}




BOOST_AUTO_TEST_CASE( test_spoon_gear_with_const_value ) {

  std::vector<uint8_t> sink;
  auto start = std::begin(sink);
  auto end   = std::end(sink);
  uint32_t my_attr = 0;
  storage = 0;

  using my_gear_type0 = simple_gear;
  constexpr auto my_gear0 = my_gear_type0{};
  constexpr auto my_gear1 = my_gear0(uint32_t{42});

  static_assert(std::is_same<uint32_t, my_gear_type0::attribute_type>::value);
  static_assert(std::is_same<simple_gear::attribute_type, my_gear_type0::attribute_type>::value);
  static_assert(std::is_same<decltype(my_gear1)::attribute_type, decltype(my_gear1)::attribute_type>::value);

  spoon::serialize(sink, my_gear1);
  BOOST_TEST(storage == uint32_t{42});
  BOOST_TEST(my_attr == uint32_t{0});
  BOOST_TEST(spoon::deserialize(start, end, my_gear1, my_attr) == true);
  BOOST_TEST(my_attr == uint32_t{42});
}

BOOST_AUTO_TEST_CASE( test_spoon_gear_with_provider_functor ) {

  std::vector<uint8_t> sink;
  auto start = std::begin(sink);
  auto end   = std::end(sink);
  uint32_t my_attr = 0;
  storage = 0;

  using my_gear_type0 = simple_gear;
  constexpr auto my_gear0 = my_gear_type0{};
  constexpr auto my_gear1 = my_gear0([]() -> uint32_t { return 42;});

  static_assert(std::is_same<uint32_t, my_gear_type0::attribute_type>::value);
  static_assert(std::is_same<simple_gear::attribute_type, my_gear_type0::attribute_type>::value);
  static_assert(std::is_same<decltype(my_gear1)::attribute_type, decltype(my_gear1)::attribute_type>::value);

  BOOST_TEST(spoon::serialize(sink, my_gear1) == true);
  BOOST_TEST(storage == uint32_t{42});
  BOOST_TEST(my_attr == uint32_t{0});
  BOOST_TEST(spoon::deserialize(start, end, my_gear1, my_attr) == true);
  BOOST_TEST(my_attr == uint32_t{42});
}


BOOST_AUTO_TEST_SUITE_END()
