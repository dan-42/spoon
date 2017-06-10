/*
 * spoon.hpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */

#ifndef SRC_LIB_SPOON_HPP_
#define SRC_LIB_SPOON_HPP_


#include <utility>
#include <cstdint>

#include <spoon/serializer.hpp>
#include <spoon/deserializer.hpp>

namespace spoon {

  namespace detail {
    constexpr auto fail_serializer = [](auto& sink, auto&& type, auto& ctx) -> bool { return false;};
  }
  /**
   *  get_serializer on default we fail!
   */
  template<typename Type, typename Context = void>
  struct get_serializer {
    static auto call() -> decltype(detail::fail_serializer) {
    //  static_assert(false, "[ERROR] -------- Missing serializer for Type in"  );
      return detail::fail_serializer;
    }
  };

#define SPOON_SERIALIZER_REGISTER(engine, type, context)                      \
    template<>                                                                \
    struct get_serializer<type, context> {                                    \
      static auto call() -> decltype(engine) {                                \
        return engine;                                                        \
      }                                                                       \
    };                                                                        \


  SPOON_SERIALIZER_REGISTER(serializer::binary::big::word16_, uint16_t, void)
  SPOON_SERIALIZER_REGISTER(serializer::binary::big::word16_,  int16_t, void)

  template<typename Type>
  bool serialize(auto& sink, const auto& serializer, Type t) {
   const auto unused = nullptr;
   return serializer(sink, t, unused);
  }

  template<typename Type, typename Context>
  bool serialize(auto& sink, const auto& serializer, Type t, Context& ctx) {
   return serializer(sink, t, ctx);
  }



  template<typename Type, typename Context>
  bool serialize(auto& sink, Type t, Context& ctx) {
    const auto& serializer = get_serializer<Type, Context>::call();
    return serializer(sink, t, ctx);
  }


  template<typename Type>
  bool serialize(auto& sink, Type t) {
   const auto unused = nullptr;
   return serialize(sink, t, unused);
  }











  template<typename Type>
  bool deserialize(auto& start, const auto& end, const auto& deserializer, Type& t) {
   const auto unused = nullptr;
   return deserializer(start, end, t, unused);
  }

  template<typename Type, typename Context>
  bool deserialize(auto& start, const auto& end, const auto& deserializer, Type& t, Context& ctx) {
   return deserializer(start, end, t, ctx);
  }





}/*namespace spoon*/




#endif /* SRC_LIB_SPOON_HPP_ */
