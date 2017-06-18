/*
 * binary.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_BINARY_HPP_
#define SRC_SPOON_DESERIALIZER_BINARY_HPP_


#include <spoon/unused_type.hpp>
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



}}

#define SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(name, base_type, byte_size)             \
  const constexpr auto name = [](auto& start, const auto& end, auto& attribute, auto& ctx)        \
                                                                                       -> bool {  \
        std::cout << "binary deserializer "  << #name  << std::endl;                                   \
        using attribute_type =  std::decay_t<decltype(attribute)>;                                \
        auto itr = start;                                                                        \
        base_type value{};                                                                        \
        decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);                                \
        bytes += byte_size-1;                                                                     \
        for(std::size_t i = 0; i < byte_size; ++i ) {                                             \
          std::cout << "binary deserializer "  << #name  << "  i =  " << i << std::endl;                                   \
          if(itr == end || !detail::deserialize_to(itr, end, *bytes--)) {                         \
            std::cout << "binary deserializer "  << #name  << "  failed  1"<< std::endl;                                   \
            return false;                                                                         \
          }                                                                                       \
          itr++;                                                                                  \
        }                                                                                         \
        if (!assign<base_type, attribute_type>::to(value, attribute)) {                           \
          std::cout << "binary deserializer "  << #name  << "  failed 2"<< std::endl;                                   \
          return false;                                                                           \
        }                                                                                         \
        start = itr;                                                                              \
        std::cout << "binary deserializer "  << #name  << "  success "<< std::endl;                                   \
        return true;                                                                              \
      };                                                                                          \



#define SPOON_DESERIALIZER_REGISTER(engine, type, context)                                        \
  namespace spoon {                                                                               \
    template<>                                                                                    \
    struct get_deserializer<type, context> {                                                      \
      static auto call() -> decltype(engine) {                                                    \
        return engine;                                                                            \
      }                                                                                           \
    };                                                                                            \
  }                                                                                               \



namespace spoon { namespace deserializer { namespace binary {

  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(byte_, uint8_t, 1)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(bool_, bool,    sizeof(bool))


  namespace big {

    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word8_,  uint8_t,  1)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word16_, uint16_t,  2)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word24_, uint32_t,  3)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word32_, uint32_t,  4)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word40_, uint64_t,  5)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word48_, uint64_t,  6)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word56_, uint64_t,  7)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word64_, uint64_t,  8)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float_,  float,  4)
    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(double_, double,  8)

    } /* namespace big*/

}}}

SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::bool_,          bool,      spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::byte_,          uint8_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::byte_,          int8_t,    spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::word16_,   uint16_t,  spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::word16_,   int16_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::word32_,   uint32_t,  spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::word32_,   int32_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::word64_,   uint64_t,  spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::word64_,   int64_t,   spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::float_,    float,     spoon::unused_type)
SPOON_DESERIALIZER_REGISTER(spoon::deserializer::binary::big::double_,   double,    spoon::unused_type)







#endif /* SRC_SPOON_DESERIALIZER_BINARY_HPP_ */
