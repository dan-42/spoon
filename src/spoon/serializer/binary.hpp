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



#define SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(name, byte_size)                          \
  const constexpr auto name = [](auto& sink, auto&& attribute, auto& ctx) -> bool {               \
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
    static auto call() -> decltype(engine) {                                            \
      return engine;                                                                              \
    }                                                                                             \
  };                                                                                              \
  const auto dummy_##engine_##type = spoon::get_serializer<type, context>::call();            \



namespace spoon { namespace serializer { namespace binary {

  SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(byte_,  1)
  SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(bool_,  1)

  namespace big {

    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word8_ ,   1)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word16_,   2)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word24_,   3)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word32_,   4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word40_,   5)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word48_,   6)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word56_,   7)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word64_,   8)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float_,    4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(double_,   8)

    } /* namespace big*/

}}}


SPOON_SERIALIZER_REGISTER(serializer::binary::bool_,        bool,     spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::big::word8_,  uint8_t,  spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::word8_,  int8_t,   spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::big::word16_, uint16_t, spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::word16_, int16_t,  spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::big::word32_, uint32_t, spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::word32_, int32_t,  spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::big::word64_, uint64_t, spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::word64_, int64_t,  spoon::unused_type)

SPOON_SERIALIZER_REGISTER(serializer::binary::big::float_,  float,    spoon::unused_type)
SPOON_SERIALIZER_REGISTER(serializer::binary::big::double_, double,   spoon::unused_type)



#endif /* SRC_SPOON_SERIALIZER_BINARY_HPP_ */
