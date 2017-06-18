/*
 * get.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_GET_HPP_
#define SRC_SPOON_DESERIALIZER_GET_HPP_

#include <spoon/unused_type.hpp>

namespace spoon {

  namespace detail {
    constexpr auto fail_deserializer = [](auto& start, const auto& end, auto&& type, auto& ctx) -> bool { return false;};
  } /* detail */

  /**
   *  get_deserializer on default we fail!
   */
  template<typename Type, typename Context = spoon::unused_type>
  struct get_deserializer {
    static auto call() -> decltype(detail::fail_deserializer) {
      return detail::fail_deserializer;
    }
  };

} /* spoon */



#endif /* SRC_SPOON_DESERIALIZER_GET_HPP_ */
