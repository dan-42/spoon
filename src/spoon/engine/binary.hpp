/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_ENGINE_BINARY_HPP_
#define SRC_SPOON_ENGINE_BINARY_HPP_

#include <spoon/engine/gear.hpp>
#include <spoon/engine/base.hpp>
#include <spoon/traits/traits.hpp>
#include <spoon/traits/is_supported_engine_type.hpp>

#include <cstdint>
#include <iostream>

namespace spoon { namespace engine { namespace big_endian {

template<typename Attr, typename Endian, std::size_t Bits>
struct binary : private spoon::engine::base {

  using attribute_type = Attr;
  using this_type = binary<Attr, Endian, Bits>;
  static constexpr std::size_t byte_size = Bits/8;

  template<typename Sink>
  inline auto serialize(bool& pass, Sink& sink, Attr attr)  const -> void {

    decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attr);
    bytes += byte_size - 1;
    for(std::size_t i = 0; i < byte_size; ++i ) {
      if(!spoon::traits::detail::serialize_to_sink(sink , *bytes--)) { pass = false; return; }
    }
  }

  template<typename Iterator>
  inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, Attr& attr)  const -> void {
    auto itr = start;
    attribute_type value{};
    decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);
    bytes += byte_size -1;
    for(std::size_t i = 0; i < byte_size; ++i ) {
      if(!spoon::traits::detail::deserialize_from_source(itr, end, *bytes--)) { pass = false; return; }
    }

    if (!spoon::traits::detail::assign_to(value, attr)) {
      pass =  false;
    }
    start = itr;
    pass = true;
  }

};

using bool8  = spoon::engine::gear<binary<bool,      spoon::tag::big_endian,    8>>;
using uint8  = spoon::engine::gear<binary<uint8_t,   spoon::tag::big_endian,    8>>;
using  int8  = spoon::engine::gear<binary< int8_t,   spoon::tag::big_endian,    8>>;
using uint16 = spoon::engine::gear<binary<uint16_t,  spoon::tag::big_endian,   16>>;
using  int16 = spoon::engine::gear<binary< int16_t,  spoon::tag::big_endian,   16>>;
using uint32 = spoon::engine::gear<binary<uint32_t,  spoon::tag::big_endian,   32>>;
using  int32 = spoon::engine::gear<binary< int32_t,  spoon::tag::big_endian,   32>>;
using uint64 = spoon::engine::gear<binary<uint64_t,  spoon::tag::big_endian,   64>>;
using  int64 = spoon::engine::gear<binary< int64_t,  spoon::tag::big_endian,   64>>;

}}} // spoon::engine::big_endian


namespace spoon { namespace big_endian {

  constexpr engine::big_endian::bool8     bool8{};

  constexpr engine::big_endian::uint8     uint8{};
  constexpr engine::big_endian:: int8      int8{};
  constexpr engine::big_endian::uint16    uint16{};
  constexpr engine::big_endian::uint32    uint32{};
  constexpr engine::big_endian::uint64    uint64{};
//constexpr engine::big_endian::uint64    uint64{};

//  constexpr engine::big_endian::float32   float32{};
//  constexpr engine::big_endian::float64   float64{};

}}


#endif /* SRC_SPOON_ENGINE_BINARY_HPP_ */



