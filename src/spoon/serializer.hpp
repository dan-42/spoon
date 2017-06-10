/*
 * serializer.hpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_HPP_
#define SRC_SPOON_SERIALIZER_HPP_

#include "external/mapbox/variant.hpp"



#include <utility>
#include <cstdint>
#include <boost/spirit/include/qi_binary.hpp>


#define SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(name, type, byte_size)                    \
  const constexpr auto name = [](auto& sink, auto&& attribute, auto& ctx) -> bool {               \
        decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attribute);                            \
        bytes += byte_size - 1;                                                                   \
        for(std::size_t i = 0; i < byte_size; ++i ) {                                             \
          if(!serialize_to(sink , *bytes--)) {                                                    \
            return false;                                                                         \
          }                                                                                       \
        }                                                                                         \
        return true;                                                                              \
      };                                                                                          \



namespace mapbox { namespace traits {

  template <class... Types>
  struct is_mapbox_variant {
    using type = std::false_type;
    enum { value = false };
  };

  template <class... Types>
  struct is_mapbox_variant<mapbox::util::variant<Types...>> {
    using type = std::true_type;
    enum { value = true };
  };

  template <class... Types>
  struct is_mapbox_variant<const mapbox::util::variant<Types...>> {
    using type = std::true_type;
    enum { value = true };
  };


}}

namespace spoon { namespace serializer {


namespace detail {
  namespace util {

 template<typename F, typename... Ts>
  F for_each_args(F f, Ts&&... a) {
    return std::initializer_list<int>{(std::ref(f)(std::forward<Ts>(a)),0)...}, f;
  }

}

}

/**
 * customizable point
 */
  template<typename Container, typename Value>
  struct serialize {
    static bool to(Container& container, Value&& value) {
      container.emplace_back(std::move(value));
      return true;
    }

    static bool to(Container& container, const Value& value) {
          container.push_back(value);
          return true;
        }
  };


  const constexpr auto serialize_to = [](auto& container, auto&& value) -> bool {
    using container_type = typename std::remove_reference<decltype(container)>::type;
    using value_type = typename std::remove_reference<decltype(value)>::type;
    return serialize<container_type, value_type>::to(container, value);
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




  namespace detail {



  //! Tests if T is a specialization of Template
  // thanks to internet
  template <typename T, template <typename...> class Template>
  struct is_specialization_of : std::false_type {};

  template <template <typename...> class Template, typename... Args>
  struct is_specialization_of<Template<Args...>, Template> : std::true_type {};


  template<class T>
  using enable_if_is_variant_t = typename std::enable_if<
      /* traits::is_boost_variant<T>::value
    || */mapbox::traits::is_mapbox_variant<T>::value
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


  } /* detail*/


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



  namespace binary {

    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(byte_, uint8_t,   1)
    decltype(auto) bool_ = byte_;

  } /* namespace binary*/

  namespace binary {  namespace big {

    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word8_ , uint8_t,   1)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word16_, uint16_t,  2)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word24_, uint32_t,  3)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word32_, uint32_t,  4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word40_, uint64_t,  5)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word48_, uint64_t,  6)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word56_, uint64_t,  7)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(word64_, uint64_t,  8)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(float_,  float,     4)
    SPOON_SERIALIZE_MAKE_PRIMITIVE_BINARY_BIGENDIAN(double_, double,    8)

    } /* namespace big*/
  } /* namespace binary*/

} /*namespace serializer*/
} /*namespace spoon*/


#endif /* SRC_SPOON_SERIALIZER_HPP_ */
