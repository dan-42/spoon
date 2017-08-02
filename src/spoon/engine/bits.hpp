/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_ENGINE_BITS_HPP_
#define SRC_SPOON_ENGINE_BITS_HPP_





#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>


namespace spoon { namespace engine {


namespace detail {

  template<typename Buffer, typename Attr, class Enable = void>
  struct attr_to_buffer_type {
    static constexpr inline auto call(const Attr& attr) -> Buffer {
      return static_cast<Buffer>(attr);
    }
  };


  template<typename Buffer, typename Attr>
  struct attr_to_buffer_type<Buffer, Attr, typename std::enable_if_t<std::is_same<Attr, bool>::value>> {
    static constexpr inline auto call(const bool& attr) -> Buffer {
      return attr ? 0x01 : 0x00;
    }
  };

  template<typename Buffer, typename Attr>
  struct attr_to_buffer_type<Buffer, Attr, typename std::enable_if_t<std::is_enum<Attr>::value>> {
    static constexpr inline auto call(const Attr& attr) -> Buffer {
      using underlying_type = typename std::underlying_type<Attr>::type;
      constexpr auto underlying_max = std::numeric_limits<underlying_type>::max();
      constexpr auto buffer_max = std::numeric_limits<Buffer>::max();
      static_assert(buffer_max >= underlying_max, "ERROR enum under;ying type to big");
      return static_cast<Buffer>(attr);
    }
  };

}


/**
 * The Engine bits can be parametert to have an engine that extracts bits or multi bit-numbers from the data stream
 * also it can be configured to advance the buffer or work in place
 */
template<typename AttrType, typename BufferType, size_t NumberBits, size_t BitsOffset, typename IsAdvancing = std::false_type >
struct bits : spoon::engine::base {

  using attr_type = AttrType;

  static constexpr auto buffer_size = sizeof(BufferType);

  static_assert( std::is_integral<AttrType>::value ||  std::is_enum<AttrType>::value, "ERROR AttrType must be integral or enum");
  static_assert( std::is_integral<BufferType>::value,                                 "ERROR BufferType must be integral");
  static_assert( std::is_same<typename IsAdvancing::value_type, bool>::value,         "ERROR IsAdvancing must std::false_type(default) or std::true_type");
  static_assert( ((NumberBits + BitsOffset)/8) <= sizeof(BufferType),                 "ERROR BufferType is to small for given NumberBist + BitsOffset");


  static constexpr inline auto serialize(auto& sink, auto&& attr, auto& ctx) -> bool {
    using actual_attr_type = std::decay_t<decltype(attr)>;
    static_assert( std::is_integral<actual_attr_type>::value ||  std::is_enum<actual_attr_type>::value, "ERROR passed attr is not an integral or enum");
    BufferType buffer{};

    if(!extract_buffer(sink, buffer, IsAdvancing{})) {
      return false;
    }
    decltype(auto) attr_value = detail::attr_to_buffer_type<BufferType, actual_attr_type>::call(attr);
    set_bits(buffer, attr_value);
    return insert_buffer(sink, buffer, IsAdvancing{});
  }


  static constexpr inline auto deserialize(auto& start, const auto& end, auto& attr,  auto& ctx) -> bool {
    using actual_attr_type = std::decay_t<decltype(attr)>;
    auto itr = start;
    BufferType value{0};
    BufferType buffer{};
    decltype(auto) buffer_bytes = reinterpret_cast<uint8_t*>(&buffer);

    for(size_t i = 0; i < buffer_size; ++i) {
      buffer_bytes = *itr;
      buffer_bytes++;
      itr++;
    }

    get_bits(buffer, value);

    attr = static_cast<actual_attr_type>(value);

    if(IsAdvancing::value) {
      start = itr;
    }
    return true;
  }


  static constexpr inline auto extract_buffer(auto& sink, BufferType& buffer, std::false_type no_advance) -> bool {

    if(sink.size() < buffer_size) {
      return false;
    }
    decltype(auto) buffer_bytes = reinterpret_cast<uint8_t*>(&buffer);

    auto sink_index = sink.size()- buffer_size;
    for(size_t i = 0; i <  buffer_size; ++i) {
      *buffer_bytes = sink[sink_index + i];
    }
    return true;
  }


  static constexpr inline auto extract_buffer(auto& sink, BufferType& buffer, std::true_type do_advance) -> bool {
    //noop  the buffer will added to the sink
    return true;
  }


  static constexpr inline auto insert_buffer(auto& sink, BufferType& buffer, std::false_type no_advance) -> bool {
    constexpr auto buffer_size  = sizeof(buffer);
    decltype(auto) buffer_bytes = reinterpret_cast<uint8_t*>(&buffer);

    auto sink_index = sink.size() - buffer_size;
    for(size_t i = 0; i <  buffer_size; ++i) {
      sink[sink_index + i] = *buffer_bytes;
    }
    return true;
  }


  static constexpr inline auto insert_buffer(auto& sink, BufferType& buffer, std::true_type do_advance) -> bool {
    //extract data from sink, and we work inplace
    constexpr auto buffer_size  = sizeof(buffer);
    decltype(auto) buffer_bytes = reinterpret_cast<uint8_t*>(&buffer);

    for(size_t i = 0; i <  buffer_size; ++i) {
     sink.push_back(*buffer_bytes++);
    }
    return true;
  }


  static constexpr inline auto create_mask() -> BufferType {
    BufferType mask{0};
    for (size_t i = BitsOffset; i < BitsOffset + NumberBits; ++i) {
      mask = mask | (0x01 << i);
    }
    return mask;
  }


  static constexpr inline auto set_bits(BufferType& data_buffer, const BufferType& value) -> void {
    const auto bit_mask = create_mask();
    data_buffer = data_buffer & ~(bit_mask);
    data_buffer = data_buffer | (value << BitsOffset);
  }

  static constexpr inline auto get_bits(BufferType& data_buffer, BufferType& value) -> void {
    const auto bit_mask = create_mask();
    value =  (data_buffer & bit_mask) >> BitsOffset;
  }


};

}}

namespace spoon {

template<typename AttrType, typename BufferType, size_t NumberBits, size_t BitsOffset, typename IsAdvancing = std::false_type >
constexpr inline auto bits() -> spoon::engine::bits<AttrType, BufferType, NumberBits, BitsOffset, IsAdvancing> {
  return spoon::engine::bits<AttrType, BufferType, NumberBits, BitsOffset, IsAdvancing>{};
}



template<typename BufferType, size_t BitsOffset, typename IsAdvancing = std::false_type >
constexpr inline auto bits_bool() -> spoon::engine::bits<bool, BufferType, 1, BitsOffset, IsAdvancing> {
  return spoon::engine::bits<bool, BufferType, 1, BitsOffset, IsAdvancing>{};
}

}


#endif /*SRC_SPOON_ENGINE_BITS_HPP_*/
