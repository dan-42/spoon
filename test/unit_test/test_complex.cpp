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


#define BOOST_TEST_MODULE test spoon complex
#include <boost/test/included/unit_test.hpp>

#include <boost/fusion/adapted/struct.hpp>
#include <iostream>

#include <spoon.hpp>




struct pod_type {

  bool      my_bool;
  uint32_t  my_uint32;
  double    my_double;


};

BOOST_FUSION_ADAPT_STRUCT(pod_type, my_bool, my_uint32, my_double);


namespace spoon { namespace serializer {
  decltype(auto) pod_engine     = seq_      ( binary::bool_,  binary::big::word32_, binary::big::double_ );
}}

SPOON_SERIALIZER_REGISTER(serializer::pod_engine,        pod_type,     spoon::unused_type)

using varinat_t = mapbox::util::variant<bool, uint32_t, double, pod_type>;

namespace spoon { namespace deserializer {

  template <typename BaseType>
  struct assign<BaseType, varinat_t> {
      static auto to(BaseType base, varinat_t &attr) -> bool {
        attr = base;
        return true;
      }
   };

}}


using my_type = std::vector<varinat_t>;


inline void print(const std::vector<uint8_t> &data) {
    for(auto &c : data)
      std::cout << " 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)c;
    std::cout << std::endl;
  }


BOOST_AUTO_TEST_SUITE( test_spoon_complex )




BOOST_AUTO_TEST_CASE( test_spoon_complex_1 ) {

  std::vector<uint8_t> binary_data{};

  {
    using namespace spoon::serializer;

    my_type var{};

    var.emplace_back(uint32_t{23});
    var.emplace_back(bool{true});
    var.emplace_back(pod_type{true, 1337, 2.17});
    var.emplace_back(double{3.1474283});


    //decltype(auto) pod_engine     = seq_      ( binary::bool_,  binary::big::word32_, binary::big::double_ );
    decltype(auto) variant_engine = any_type_ (bool{}, uint32_t{}, double{}, varinat_t{});
    //decltype(auto) variant_engine = any_      (pod_engine, binary::big::double_, binary::big::word32_, binary::bool_);
    decltype(auto) engine         = repeat_   (variant_engine);


    auto success = spoon::serialize_with(engine, binary_data, var);
    BOOST_TEST(success == true);

    print(binary_data);
    BOOST_TEST( binary_data.size() == size_t{4} );
    BOOST_TEST(binary_data[0] == 0x00);
    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x05);
    BOOST_TEST(binary_data[3] == 0x39);
  }

/*
  {
    using namespace spoon::deserializer;


    varinat_t var{};
    auto     start = binary_data.begin();
    const auto end = binary_data.end();

    decltype(auto) engine = any_(binary::big::double_, binary::big::word32_, binary::bool_);

    auto  success = spoon::deserialize_with(engine, start, end, var);

    BOOST_TEST(success == true);
    BOOST_TEST((start == end), "start != end");


    struct result_visitor {
      void operator()(bool r) const     { BOOST_TEST(false,               " it is bool"); }
      void operator()(uint32_t e) const { BOOST_TEST(e == uint32_t{1337}, " it is uint but value wrong"); }
      void operator()(double e) const   { BOOST_TEST(false,               " it is double  "); }
    };

    result_visitor visitor;
    mapbox::util::apply_visitor(visitor, var);

  }
*/
}






BOOST_AUTO_TEST_SUITE_END()
