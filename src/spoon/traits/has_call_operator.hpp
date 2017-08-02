/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SRC_SPOON_TRAITS_HAS_CALL_OPERATOR_HPP_
#define SRC_SPOON_TRAITS_HAS_CALL_OPERATOR_HPP_

#include <spoon/engine/base.hpp>
#include <type_traits>

namespace spoon { namespace traits {

/**
 * This implementation is from "decltype" from the stackoverflow question:
 * https://stackoverflow.com/questions/5100015/c-metafunction-to-determine-whether-a-type-is-callable#5117641
 * many thanks
 *
 */
namespace detail {
  template<bool, typename T>
  struct has_regular_call_operator
  {
    template<typename C> // detect regular operator()
    static char test(decltype(&C::operator()));

    template<typename C> // worst match
    static char (&test(...))[2];

    static const bool value = (sizeof( test<T>(0)  ) == 1);
  };

  template<typename T>
  struct has_regular_call_operator<true,T>
  {
    static const bool value = true;
  };
}//detail

template<typename T>
struct has_call_operator
{
  template<typename F, typename A> // detect 1-arg operator()
  static char test(int, decltype( (*(F*)0)( (*(A*)0) ) ) = 0);

  template<typename F, typename A, typename B> // detect 2-arg operator()
  static char test(int, decltype( (*(F*)0)( (*(A*)0), (*(B*)0) ) ) = 0);

  template<typename F, typename A, typename B, typename C> // detect 3-arg operator()
  static char test(int, decltype( (*(F*)0)( (*(A*)0), (*(B*)0), (*(C*)0) ) ) = 0);

  template<typename F, typename ...Args> // worst match
  static char (&test(...))[2];

  static const bool OneArg = (sizeof( test<T, int>(0)  ) == 1);
  static const bool TwoArg = (sizeof( test<T, int, int>(0)  ) == 1);
  static const bool ThreeArg = (sizeof( test<T, int, int, int>(0)  ) == 1);

  static const bool HasTemplatedOperator = OneArg || TwoArg || ThreeArg;
  static const bool value = detail::has_regular_call_operator<HasTemplatedOperator, T>::value;
};

}} //spoon::traits


#endif /* SRC_SPOON_TRAITS_HAS_CALL_OPERATOR_HPP_ */
