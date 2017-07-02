/*
 * binary.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_BINARY_HPP_
#define SRC_SPOON_SERIALIZER_BINARY_HPP_


#include <spoon/unused_type.hpp>
#include <spoon/serializer/detail/serialize_to.hpp>
#include <spoon/serializer/get.hpp>


#include <utility>
#include <cstdint>



#define SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(name, attribute_type, byte_size)          \
  const constexpr auto name = [](auto& sink, attribute_type&& attribute, auto& ctx) -> bool {     \
        decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attribute);                            \
        bytes += byte_size - 1;                                                                   \
        for(std::size_t i = 0; i < byte_size; ++i ) {                                             \
          if(!detail::serialize_to(sink , *bytes--)) {                                            \
            return false;                                                                         \
          }                                                                                       \
        }                                                                                         \
        return true;                                                                              \
      };                                                                                          \



#define SPOON_SERIALIZER_REGISTER(engine, type, context)                                          \
  namespace spoon {                                                                               \
    template<> struct get_serializer<type, context>;                                              \
  }                                                                                               \
  template<>                                                                                      \
  struct spoon::get_serializer<type, context> {                                                   \
    static auto call() -> decltype(engine) {                                                      \
      return engine;                                                                              \
    }                                                                                             \
  };                                                                                              \
  const auto dummy_##engine_##type = spoon::get_serializer<type, context>::call();                \



namespace spoon { namespace serializer { namespace binary {

  SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint8,  uint8_t, 1)
  SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int8,   int8_t,  1)
  SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(bool8,  bool,    1)

  namespace big {


    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint16,  uint16_t,  2)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint24,  uint32_t,  3)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint32,  uint32_t,  4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint40,  uint64_t,  5)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint48,  uint64_t,  6)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint56,  uint64_t,  7)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint64,  uint64_t,  8)

    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int16,   int16_t,   2)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int24,   int32_t,   3)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int32,   int32_t,   4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int40,   int64_t,   5)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int48,   int64_t,   6)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int56,   int64_t,   7)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int64,   int64_t,   8)

    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float32, float,     4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float64, double,    8)

    } /* namespace big*/

}}}


SPOON_SERIALIZER_REGISTER(serializer::binary::bool8,        bool,     spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::uint8,        uint8_t,  spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::uint16,  uint16_t, spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::uint32,  uint32_t, spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::uint64,  uint64_t, spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::int8,         int8_t,   spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::int16,   int16_t,  spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::int32,   int32_t,  spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::uint64,  int64_t,  spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::big::float32,  float,   spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::float64, double,   spoon::unused_type)



#endif /* SRC_SPOON_SERIALIZER_BINARY_HPP_ */
