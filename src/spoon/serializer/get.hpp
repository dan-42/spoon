/*
 * get.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_GET_HPP_
#define SRC_SPOON_SERIALIZER_GET_HPP_

#include <boost/type_index.hpp>
#include <iostream>


namespace spoon {

  namespace detail {
    constexpr auto fail_serializer = [](auto& sink, auto&& type, auto& ctx) -> bool { return false;};
  } /* detail */

  /**
   *  get_serializer on default we fail!
   */
  template<typename Type, typename Context = std::nullptr_t>
  struct get_serializer
  {
     static auto call() -> decltype(detail::fail_serializer) {
      std::cerr << "[ERROR] missing get_serializer specialisation for type: " << boost::typeindex::type_id< Type >().pretty_name() << " and Context: " << boost::typeindex::type_id< Context >().pretty_name() << '\n' ;
      //static_assert(false, "[ERROR] -------- Missing serializer for Type in"  );
      return detail::fail_serializer;
    }
  };

} /* spoon */



#endif /* SRC_SPOON_SERIALIZER_GET_HPP_ */
