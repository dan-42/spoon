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
using varinat_t = mapbox::util::variant<bool, uint32_t, double>;

namespace spoon { namespace deserializer {



  template <typename BaseType>
  struct assign<BaseType, varinat_t> {
      static auto to(BaseType base, varinat_t &attr) -> bool {
        attr = base;
        return true;
      }
   };

}}

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

  {
    using namespace spoon::serializer;

    varinat_t var = uint32_t{1337};
    //decltype(auto) engine = any_( binary::big::double_,  binary::big::word32_, binary::bool_);
    decltype(auto) engine = any_type_(bool{}, uint32_t{}, double{});
    auto success = spoon::serialize_with(engine, binary_data, var);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{4} );
    BOOST_TEST(binary_data[0] == 0x00);
    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x05);
    BOOST_TEST(binary_data[3] == 0x39);
  }


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

}


/*
 *
 *

 // decltype(auto) engine = any_type_(varinat_t);

decltype(auto) engine = any_type_(bool{}, uint32_t{}, double{});

//decltype(auto) engine_fail    = any_(binary::bool_, binary::big::word32_, binary::big::double_);
    //decltype(auto) engine_success = any_(binary::big::double_, binary::big::word32_, binary::bool_);

 * /auto success = spoon::deserialize_with(engine_fail, start, end, var);
 */










BOOST_AUTO_TEST_SUITE_END()
