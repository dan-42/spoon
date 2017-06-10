/*
 * serializer.hpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_HPP_
#define SRC_SPOON_DESERIALIZER_HPP_

#include "external/mapbox/variant.hpp"

#include <utility>
#include <cstdint>


#define SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(name, byte_size)                        \
  const constexpr auto name = [](auto& start, const auto& end, auto& attribute, auto& ctx)        \
                                                                                       -> bool {  \
        decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attribute);                            \
        bytes += byte_size-1;                                                                     \
        for(std::size_t i = 0; i < byte_size; ++i ) {                                             \
          if(!deserialize_to(start, end, *bytes--)) {                                             \
            return false;                                                                         \
          }                                                                                       \
          start++;                                                                                \
        }                                                                                         \
        return true;                                                                              \
      };                                                                                          \




namespace spoon { namespace deserializer {



/**
 * customizable point
 */
  template<typename Start, typename End, typename Value>
  struct deserialize {
    static bool to(Start& start, const End& end, Value& value) {
      assert(start != end);
      value = *start;
      return true;
    }

  };


  const constexpr auto deserialize_to = [](auto& start, const auto& end, auto& value) -> bool {
    using start_type = typename std::remove_reference<decltype(start)>::type;
    using end_type   = typename std::remove_reference<decltype(end)>::type;
    using value_type = typename std::remove_reference<decltype(value)>::type;
    return deserialize<start_type, end_type, value_type>::to(start, end, value);
  };





  const constexpr auto true_ = [](auto& sink, auto t, auto& ctx)-> bool {
         return true;
       };

  const constexpr auto false_ = [](auto& sink, auto t, auto& ctx)-> bool {
        return false;
      };


  const constexpr auto unused_ = true_;


  /**
   * const auto my_seq = seq_(word16_, word_32,....)
   *
   * my_seq(sink, t, ctx)
   */




//  namespace detail {


/*
  //! Tests if T is a specialization of Template
  // thanks to internet
  template <typename T, template <typename...> class Template>
  struct is_specialization_of : std::false_type {};

  template <template <typename...> class Template, typename... Args>
  struct is_specialization_of<Template<Args...>, Template> : std::true_type {};


  template<class T>
  using enable_if_is_variant_t = typename std::enable_if<
        mapbox::traits::is_mapbox_variant<T>::value
  ,T>::type;

  template<class T>
  using enable_if_is_tuple_t = typename std::enable_if<
      is_specialization_of<T, std::tuple>::value , T>::type;

  template<class T>
  using enable_if_is_primitve_t = typename std::enable_if<
       std::is_integral<T>::value || std::is_floating_point<T>::value ,T>::type;


  template <std::size_t N, typename Attr, enable_if_is_primitve_t<Attr>* = nullptr>
    constexpr auto invoke_seq(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
      if(pass) {
        std::cout << "invoke_seq primitive" << std::endl;
        pass = std::get<N>(fs) (sink, attr, ctx);
      }
    }

  template <std::size_t N, typename Attr, enable_if_is_tuple_t<Attr>* = nullptr>
  constexpr auto invoke_seq(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
    if(pass) {
      std::cout << "invoke_seq tuple" << std::endl;
      pass = std::get<N>(fs) (sink, std::get<N>(attr), ctx);
    }
  }




  template<std::size_t N, typename Attr, enable_if_is_primitve_t<Attr>* = nullptr>
  constexpr auto invoke_any(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
    if(!pass) {
      std::cout << "invoke_any primitive" << std::endl;
      pass = std::get<N>(fs) (sink, attr, ctx);
    }
  }



  template<std::size_t N, typename Attr, enable_if_is_tuple_t<Attr>* = nullptr>
  constexpr auto invoke_any(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
    if(!pass) {
      std::cout << "invoke_any tuple" << std::endl;
      pass = std::get<N>(fs) (sink, std::get<N>(attr), ctx);
    }
  }

  template<std::size_t N, typename Attr, enable_if_is_variant_t<Attr>* = nullptr>
    constexpr auto invoke_any(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
      if(!pass) {
        std::cout << "invoke_any variant" << std::endl;


        attr.match([&](auto a){
          pass = std::get<N>(fs) (sink, a, ctx);
        } );



       // pass = std::get<N>(fs) (sink, std::get<N>(attr), ctx);
      }
    }




  template <class F, size_t... Is>
  constexpr auto index_apply_impl(F f, std::index_sequence<Is...>) {
      return f(std::integral_constant<size_t, Is> {}...);
  }

  template <size_t N, class F>
  constexpr auto index_apply(F f) {
      return index_apply_impl(f, std::make_index_sequence<N>{});
  }

  template <class Ss>
  constexpr auto apply_seq(Ss ss, auto& sink, auto as, auto& ctx) {
    std::cout << "apply_seq " << std::endl;
      bool pass = true;
      return index_apply<std::tuple_size<Ss>{}>(
                          [&](auto... Is) {
                            std::cout << "apply_seq result" << std::endl;
                              int dummy[sizeof...(Is)] = { (detail::invoke_seq<Is>(pass, ss, sink, as, ctx), 0)... };
                              return pass;
                          });
  }

  template <class Ss>
  constexpr auto apply_any(Ss ss, auto& sink, auto as, auto& ctx) {
    std::cout << "apply_any " << std::endl;
      bool pass = false;
      return index_apply<std::tuple_size<Ss>{}>(
                          [&](auto... Is) {
                              std::cout << "apply_any result" << std::endl;
                              int dummy[sizeof...(Is)] = { (detail::invoke_any<Is>(pass, ss, sink, as, ctx), 0)... };
                              return pass;
                          });
  }


  } / * detail* /


  template<typename... Ss>
  constexpr auto seq_(Ss... ss) {

    std::cout << "seq_ " << std::endl;
    const auto args = std::make_tuple(std::move(ss)...);

    const auto result = [ args ](auto& sink, auto attr, auto& ctx)-> bool {
      std::cout << "seq_ result" << std::endl;
            return detail::apply_seq(args, sink, attr, ctx);
        };
    return result;
  }


  template<typename... Ss>
  constexpr auto any_(Ss... ss) {

    const auto args = std::make_tuple(std::move(ss)...);

    const auto result = [ args ](auto& sink, auto attr, auto& ctx)-> bool {
           return detail::apply_any(args, sink, attr, ctx);
       };
    return result;
  }

  */

  namespace binary {

    SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(byte_, 1)
    decltype(auto) bool_ = byte_;

  } /* namespace binary*/

  namespace binary {  namespace big {

  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word8_,   1)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word16_,  2)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word24_,  3)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word32_,  4)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word40_,  5)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word48_,  6)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word56_,  7)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word64_,  8)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float_,   4)
  SPOON_DESERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(double_,  8)

    } /* namespace big*/
  } /* namespace binary*/

} /*namespace serializer*/
} /*namespace spoon*/


#endif /* SRC_SPOON_SERIALIZER_HPP_ */
