/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_ENGINE_BINARY_HPP_
#define SRC_SPOON_ENGINE_BINARY_HPP_


#include <spoon/engine/base.hpp>
#include <spoon/traits/is_supported_engine_type.hpp>

// left here for simpler reworking till we have a solid status
/*

 /// parses 4 byte uint32_t
  struct uint32 : simple_engine_no_context {
    using attr_type = uint32_t;
    using byte_size = std::integral_constant<size_t, 4>;
    static constexpr inline auto serialize(auto& sink, auto&& attr, auto& ctx) -> bool {
      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attr);
      bytes += byte_size::value - 1;
      for(std::size_t i = 0; i < byte_size::value; ++i ) {
        if(!detail::serialize_to_sink(sink , *bytes--)) { return false; }
      }
      return true;
    }
    static constexpr inline auto deserialize(auto& start, const auto& end, auto& attr,  auto& ctx) -> bool {
      auto itr = start;
      attr_type value{};
      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);
      bytes += byte_size::value-1;
      for(std::size_t i = 0; i < byte_size::value; ++i ) {
        if(!detail::deserialize_from_source(itr, end, *bytes--)) { return false; }
        itr++;
      }
      if (!detail::deserialize_to_attr(value, attr)) { return false; }
      start = itr;
      return true;
    }
  };
// */

#define SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(engine_name, engine_attr_type, engine_byte_size)  \
  struct engine_name : simple_engine_no_context {                                                            \
    using attr_type = engine_attr_type;                                                                      \
    using byte_size = std::integral_constant<size_t, engine_byte_size>;                                      \
    static constexpr inline auto serialize(auto& sink, auto&& attr, auto& ctx) -> bool {                     \
      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attr);                                              \
      bytes += byte_size::value - 1;                                                                         \
      for(std::size_t i = 0; i < byte_size::value; ++i ) {                                                   \
        if(!detail::serialize_to_sink(sink , *bytes--)) { return false; }                                    \
      }                                                                                                      \
      return true;                                                                                           \
    }                                                                                                        \
    static constexpr inline auto deserialize(auto& start, const auto& end, auto& attr,  auto& ctx) -> bool { \
      auto itr = start;                                                                                      \
      attr_type value{};                                                                                     \
      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);                                             \
      bytes += byte_size::value-1;                                                                           \
      for(std::size_t i = 0; i < byte_size::value; ++i ) {                                                   \
        if(!detail::deserialize_from_source(itr, end, *bytes--)) { return false; }                           \
        itr++;                                                                                               \
      }                                                                                                      \
      if (!detail::deserialize_to_attr(value, attr)) { return false; }                                       \
      start = itr;                                                                                           \
      return true;                                                                                           \
    }                                                                                                        \
  };                                                                                                         \



namespace spoon { namespace engine {

  namespace detail {

    constexpr inline auto serialize_to_sink(auto& sink, auto value) -> bool {
      //todo add customisation point here!
      sink.push_back(value);
      return true;
    }


    constexpr inline auto deserialize_from_source(const auto& start, const auto& end, auto& value) -> bool {
      //todo add customization point here!
      if(start == end) {
        return false;
      }
      value = (*start);
      return true;
    }

    constexpr inline auto deserialize_to_attr(auto &&value, auto& attr) -> bool {
      //todo add customization point here!
      attr = value;
      return true;
    }

  } //detail

  struct simple_engine_no_context : base {
      using tag_type    = tag::simple;
      using has_context = std::false_type;
  };


  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(bool8,     bool,       1)

  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(uint8,     uint8_t,    1)
  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(uint16,    uint16_t,   2)
  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(uint32,    uint32_t,   4)
  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(uint64,    uint64_t,   8)

  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(float32,   float,      4)
  SPOON_ENGINE_DETAIL_DEFINE_BINARY_BIGENDIAN_ENGINE(float64,   double,     8)


}}


SPOON_REGISTER_ATTR_TO_ENGINE(spoon::engine::uint8,  int8_t);
SPOON_REGISTER_ATTR_TO_ENGINE(spoon::engine::uint16, int16_t);
SPOON_REGISTER_ATTR_TO_ENGINE(spoon::engine::uint32, int32_t);
SPOON_REGISTER_ATTR_TO_ENGINE(spoon::engine::uint64, int64_t);


namespace spoon {

  constexpr engine::bool8     bool8{};

  constexpr engine::uint8     uint8{};
  constexpr engine::uint16    uint16{};
  constexpr engine::uint32    uint32{};
  constexpr engine::uint64    uint64{};

  constexpr engine::float32   float32{};
  constexpr engine::float64   float64{};

}


#endif /* SRC_SPOON_ENGINE_BINARY_HPP_ */



