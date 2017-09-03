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
#include <spoon/traits/traits.hpp>
#include <spoon/traits/is_supported_engine_type.hpp>

#include <cstdint>
#include <iostream>

namespace spoon { namespace engine { namespace big_endian {

///--------------------------------------------------------------------------------------------------------------------
template<typename AttrType, std::size_t Bits>
struct binary : ::spoon::engine::gear<binary<AttrType, Bits>, AttrType> {

  ///------------------------------------------------------------------------------------------------------------------
  static constexpr std::size_t byte_size = Bits/8;
  static constexpr std::size_t bit_size  = Bits;

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink, typename Attr>
  constexpr inline auto serialize(bool& pass, Sink& sink, const Attr& attr) const -> void {
    decltype(auto) bytes = reinterpret_cast<const uint8_t*>(&attr);
    bytes += byte_size - 1;
    for(std::size_t i = 0; i < byte_size; ++i ) {
      if(!spoon::traits::detail::serialize_to_sink(sink , *bytes--)) {
        pass = false;
        return;
      }
    }
    pass = true;
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr, typename Attr>
  constexpr inline auto deserialize(bool& pass, Itr& begin, const Itr& end, Attr& attr) const -> void {
    auto itr = begin;
    AttrType value{};
    decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);
    bytes += byte_size -1;
    for(std::size_t i = 0; i < byte_size; ++i ) {
      if(!spoon::traits::detail::deserialize_from_source(itr, end, *bytes--)) {
        pass = false;
        return;
      }
    }

    if (!spoon::traits::detail::assign_to(value, attr)) {
      pass =  false;
    }
    begin = itr;
    pass = true;
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename OStream>
  constexpr inline auto what(OStream& os) -> void {
    os << "big_endian::binary<" << Bits << ">";
  }

};

using bool8    = binary<bool,      8>;
using uint8    = binary<uint8_t,   8>;
using int8     = binary<int8_t,    8>;
using uint16   = binary<uint16_t, 16>;
using int16    = binary<int16_t,  16>;
using uint32   = binary<uint32_t, 32>;
using int32    = binary<int32_t,  32>;
using uint64   = binary<uint64_t, 64>;
using int64    = binary<int64_t,  64>;
using float32  = binary<float,    32>;
using float64  = binary<double,   64>;

}}} // spoon::engine::big_endian


namespace spoon { namespace big_endian {
  constexpr engine::big_endian::bool8     bool8{};
  constexpr engine::big_endian::uint8     uint8{};
  constexpr engine::big_endian::int8      int8{};
  constexpr engine::big_endian::uint16    uint16{};
  constexpr engine::big_endian::int16     int16{};
  constexpr engine::big_endian::uint32    uint32{};
  constexpr engine::big_endian::int32     int32{};
  constexpr engine::big_endian::uint64    uint64{};
  constexpr engine::big_endian::int64     int64{};
  constexpr engine::big_endian::float32   float32{};
  constexpr engine::big_endian::float64   float64{};
}}


#endif /* SRC_SPOON_ENGINE_BINARY_HPP_ */



