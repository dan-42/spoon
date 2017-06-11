/*
 * for_each.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DETAIL_FOR_EACH_HPP_
#define SRC_SPOON_DETAIL_FOR_EACH_HPP_


namespace spoon { namespace detail {

   /**
    * \brief calls function f for each a, without template recursion
    */
  template<typename F, typename... Ts>
  F for_each_args(F f, Ts&&... a) {
    return std::initializer_list<int>{(std::ref(f)(std::forward<Ts>(a)),0)...}, f;
  }


}}


#endif /* SRC_SPOON_DETAIL_FOR_EACH_HPP_ */
