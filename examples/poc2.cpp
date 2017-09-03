/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//include <mapbox/variant.hpp>
//#include <mapbox/variant_io.hpp>

#include  <spoon/traits/has_call_operator.hpp>
#include  <spoon/engine/binary.hpp>
#include  <spoon.hpp>
#include  <spoon/any.hpp>

//#include <boost/fusion/adapted.hpp>
//#include <boost/optional.hpp>
//#include <boost/optional/optional_io.hpp>

#include <boost/variant.hpp>

#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <limits>

using my_var_t = boost::variant<uint32_t, bool, uint16_t>;

struct my_sink_type {

  template<typename T>
  auto push_back(const T& t) {
    data_[size_++] = t;

  }

  auto size() const -> size_t {
    return size_;
  }

  std::array<uint8_t, 255> data_{};
  size_t size_{0};
};



auto main(int argc, char* args[]) -> int  {
  my_sink_type my_sink;
  my_var_t my_var;
  my_var = uint32_t{42};
 // constexpr auto my_any = spoon::big_endian::uint32 | spoon::big_endian::bool8 | spoon::big_endian::uint16;
  auto my_any = spoon::any<my_var_t>(spoon::big_endian::bool8, spoon::big_endian::uint16, spoon::big_endian::uint32);//, spoon::big_endian::uint32, spoon::big_endian::uint32);
  bool pass = true;

  my_any.what(std::cout);

  //auto my_any2 = my_any([](){return uint32_t{1337}; });



 // my_any.serialize(pass, my_sink, my_var);

//  constexpr auto my_any = spoon::big_endian::uint32;

//  constexpr spoon::engine::any<decltype(spoon::big_endian::uint32), decltype(spoon::big_endian::bool8)> my_any(spoon::big_endian::uint32, spoon::big_endian::bool8);


//  constexpr spoon::engine::detail::_tuple_impl<
//      std::index_sequence_for<
//        decltype(spoon::big_endian::uint32), decltype(spoon::big_endian::bool8)
//      >,
//      decltype(spoon::big_endian::uint32), decltype(spoon::big_endian::bool8)
//    > gears{spoon::big_endian::uint32, spoon::big_endian::bool8};
//
//  constexpr auto my_any = gears.template get<0>();
//  bool pass = true;
//
//  my_any.serialize(pass, my_sink, uint32_t {22});
   //spoon::serialize(my_sink, my_any, my_var);
//   spoon::serialize(my_sink, my_any2);


 // return sink.size();
return 0;




























}
