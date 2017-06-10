/*
 * sfinae_enablers.hpp
 *
 *  Created on: Jun 8, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DETAIL_SFINAE_ENABLERS_HPP_
#define SRC_SPOON_DETAIL_SFINAE_ENABLERS_HPP_


#include "external/mapbox/variant.hpp"

#include <utility>
#include <cstdint>
#include <boost/spirit/include/qi_binary.hpp>




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


}}} /* namespaces */



#endif /* SRC_SPOON_DETAIL_SFINAE_ENABLERS_HPP_ */
