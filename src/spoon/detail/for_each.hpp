/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_DETAIL_FOR_EACH_HPP_
#define SRC_SPOON_DETAIL_FOR_EACH_HPP_

#include <functional>
#include <initializer_list>

namespace spoon { namespace detail {

   /**
    * \brief calls function f for each a, without template recursion
    */
  template<typename F, typename... Ts>
  F for_each_args(F f, Ts&&... a) {
    return std::initializer_list<int>{(std::ref(f)(std::forward<Ts>(a)),0)...}, f;
  }

  /**
   * calls F with every type Ts default constructed
   */
  template<typename F, typename... Ts>
  F for_each_type(F f) {
    return std::initializer_list<int>{(std::ref(f)( Ts{}  ),0)...}, f;
  }



  /**
   *  calls F with every element of tuple f(t.get<0>() ), f(t.get<1>() ), ..., f(t.get<I>() )
   *  Tuple must have member function get<Idx>()
   */
  template <class F, class Tuple, std::size_t... I>
  constexpr auto for_each_element(F&& f, const Tuple& t, std::index_sequence<I...>) -> F {
    return std::initializer_list<int>{(std::ref(f)(t. template  get<I>()), 0)...}, f;
  }

  /**
   *  calls F with every element of tuple f(t.get<0>() ), f(t.get<1>() ), ..., f(t.get<I>() )
   *  Tuple must have member function get<Idx>()
   *  Tuple must have static constexpr size_t number_of_elements
   */
  template <class F, class Tuple>
  constexpr auto for_each_element(F&& f, const Tuple& t) -> F {
    return for_each_element(std::forward<F>(f), t, std::make_index_sequence<Tuple::number_of_elements>{});
  }



}}


#endif /* SRC_SPOON_DETAIL_FOR_EACH_HPP_ */
