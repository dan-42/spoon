/*
 * serialize_to.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_DETAIL_SERIALIZE_TO_HPP_
#define SRC_SPOON_SERIALIZER_DETAIL_SERIALIZE_TO_HPP_


#include <spoon/serialize_to.hpp>

namespace spoon { namespace serializer {


  namespace detail {

    const constexpr auto serialize_to = [](auto& container, auto&& value) -> bool {

      using container_type  = typename std::remove_reference<decltype(container)>::type;
      using value_type      = typename std::remove_reference<decltype(value)>::type;

      return serialize<container_type, value_type>::to(container, value);
    };

  } /* detail */

}}


#endif /* SRC_SPOON_SERIALIZER_DETAIL_SERIALIZE_TO_HPP_ */
