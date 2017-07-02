/*
 * binary.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_BINARY_HPP_
#define SRC_SPOON_DESERIALIZER_BINARY_HPP_


#include <spoon/unused_type.hpp>
#include <spoon/deserializer/get.hpp>
#include <spoon/deserializer/detail/deserialize_to.hpp>

#include <utility>
#include <cstdint>

namespace spoon { namespace deserializer {

  template<typename BaseType, typename AttributeType>
  struct assign final {
     static auto to(BaseType base, AttributeType &attr) -> bool {
       attr = static_cast<AttributeType>(base);
       return true;
     }
  };

  template<typename Type>
  struct assign<Type, Type> final {
     static auto to(Type base, Type &attr) -> bool {
       attr = base;
       return true;
     }
  };

}}

#define SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(name, attribute_type, byte_size)             \
  const constexpr auto name = [](auto& start, const auto& end, attribute_type& attribute, auto& ctx)        \
                                                                                       -> bool {  \
        std::cout << "binary deserializer "  << #name  << std::endl;                                   \
        auto itr = start;                                                                        \
        attribute_type value{};                                                                        \
        decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);                                \
        bytes += byte_size-1;                                                                     \
        for(std::size_t i = 0; i < byte_size; ++i ) {                                             \
          std::cout << "binary deserializer "  << #name  << "  i =  " << i << std::endl;          \
          if(itr == end || !detail::deserialize_to(itr, end, *bytes--)) {                         \
            std::cout << "binary deserializer "  << #name  << "  failed  1"<< std::endl;          \
            return false;                                                                         \
          }                                                                                       \
          itr++;                                                                                  \
        }                                                                                         \
        if (!assign<attribute_type, attribute_type>::to(value, attribute)) {                      \
          std::cout << "binary deserializer "  << #name  << "  failed 2"<< std::endl;             \
          return false;                                                                           \
        }                                                                                         \
        start = itr;                                                                              \
        std::cout << "binary deserializer "  << #name  << "  success "<< std::endl;               \
        return true;                                                                              \
      };                                                                                          \



#define SPOON_DESERIALIZER_REGISTER(engine_value, attr_type, context_type)                        \
  namespace spoon {                                                                               \
    template<>                                                                                    \
    struct get_deserializer<attr_type, context_type> {                                            \
      static auto call() -> decltype(engine_value) {                                              \
        return engine_value;                                                                      \
      }                                                                                           \
    };                                                                                            \
                                                                                                  \
  template<>                                                                                      \
  struct deserializer_attribute<std::decay_t<decltype(engine_value)>,  std::decay_t<context_type>> {                          \
      using type = std::decay_t<attr_type>;                                                       \
  };                                                                                              \
}                                                                                                 \


namespace spoon { namespace deserializer { namespace binary {

  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint8,   uint8_t,   1)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int8,    int8_t,    1)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(bool8,   bool,      1)


  namespace big {

    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint16,  uint16_t,  2)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint24,  uint32_t,  3)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint32,  uint32_t,  4)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint40,  uint64_t,  5)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint48,  uint64_t,  6)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint56,  uint64_t,  7)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(uint64,  uint64_t,  8)

    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int16,   int16_t,   2)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int24,   int32_t,   3)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int32,   int32_t,   4)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int40,   int64_t,   5)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int48,   int64_t,   6)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int56,   int64_t,   7)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(int64,   int64_t,   8)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float32, float,     4)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float64, double,    8)

    } /* namespace big*/

}}}

SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::bool8,         bool,      spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::uint8,         uint8_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::uint16,   uint16_t,  spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::uint32,   uint32_t,  spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::uint64,   uint64_t,  spoon::unused_type)

SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::int8,          int8_t,    spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::int16,    int16_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::int32,    int32_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::int64,    int64_t,   spoon::unused_type)

SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::float32,  float,     spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::float64,  double,    spoon::unused_type)







#endif /* SRC_SPOON_DESERIALIZER_BINARY_HPP_ */
