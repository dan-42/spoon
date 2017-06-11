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

namespace spoon {  namespace detail {


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

}}

#endif /* SRC_SPOON_DETAIL_SFINAE_ENABLERS_HPP_ */
