/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#define BOOST_TEST_MODULE test spoon (de) serialize
#include <boost/test/included/unit_test.hpp>

#include <iostream>

#include <spoon.hpp>
#include <spoon/engine/gear.hpp>



BOOST_AUTO_TEST_SUITE( test_spoon_serialize )


BOOST_AUTO_TEST_CASE( test_spoon_serialize_1 ) {

  auto my_value = 123;
  auto my_gear = spoon::engine::make_geare<int>([&my_value](auto& pass, auto& sink, auto&& attr){
      if(pass) {
        my_value  = 42;
      }
    }, [&my_value](auto& pass, auto& start, const auto& end, auto&& attr){
      if(pass) {
        my_value  = 42;
      }
    });
  std::vector<uint8_t> sink{};
  int my_attr = 1337;

  BOOST_TEST(spoon::serialize(sink, my_gear, my_attr) == true );

  BOOST_TEST(my_value == 42);
}


BOOST_AUTO_TEST_CASE( test_spoon_serialize_2 ) {
  auto my_value = 123;

  auto my_gear = spoon::engine::make_geare<int>([&my_value](auto& pass, auto& sink, auto&& attr){
    if(pass) {
      my_value  = 42;
    }
  }, [&my_value](auto& pass, auto& start, const auto& end, auto&& attr){
    if(pass) {
      my_value  = 42;
    }
  });


  std::vector<uint8_t> sink{};
  int my_attr = 1337;
  BOOST_TEST(spoon::serialize(sink, my_gear, my_attr) == true );

  BOOST_TEST(my_value == 42);
}


BOOST_AUTO_TEST_CASE( test_spoon_deserialize_1 ) {
  auto my_value = 123;
  auto my_gear = spoon::engine::make_geare<int>([&my_value](auto& pass, auto& sink, auto&& attr){
      if(pass) {
        my_value  = 42;
      }
    }, [&my_value](auto& pass, auto& start, const auto& end, auto&& attr){
      if(pass) {
        my_value  = 42;
      }
    });

  std::vector<uint8_t> sink{};

  int my_attr = 1337;

  BOOST_TEST(spoon::serialize(sink, my_gear, my_attr) == true );

  BOOST_TEST(my_value == 42);
}

BOOST_AUTO_TEST_SUITE_END()
