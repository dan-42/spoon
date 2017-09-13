/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_DETAIL_PACK_HELPER_HPP_
#define SRC_SPOON_DETAIL_PACK_HELPER_HPP_

#include <cstdint>
#include <utility>
#include <type_traits>

namespace spoon { namespace detail {

///--------------------------------------------------------------------------------------------------------------------
template<typename... Ts>
struct duplication_guard : Ts... {
  constexpr duplication_guard(){};
};

///--------------------------------------------------------------------------------------------------------------------
template <std::size_t I, typename T>
struct pack_leaf {
  constexpr pack_leaf(const T& t) : value{t} {}
  using type = T;
  const T& value;
};

///--------------------------------------------------------------------------------------------------------------------
template <typename Is, typename ...Ts>
struct pack_storage;

///--------------------------------------------------------------------------------------------------------------------
template <std::size_t ...Is, typename ...Ts>
struct pack_storage<std::index_sequence<Is...>, Ts...>  : pack_leaf<Is, Ts>...
{
  ///------------------------------------------------------------------------------------------------------------------
  using this_type = pack_storage<std::index_sequence<Is...>, Ts...> ;
  static constexpr std::size_t number_of_elements = sizeof...(Is);

  ///------------------------------------------------------------------------------------------------------------------
  constexpr pack_storage(const Ts& ...ts) : pack_leaf<Is, Ts>{ts}... {
  }

  ///------------------------------------------------------------------------------------------------------------------
  template <std::size_t I, typename T>
  static constexpr const pack_leaf<I, T>& select( const pack_leaf<I, T>& leaf) noexcept {
    return leaf;
  }

  ///------------------------------------------------------------------------------------------------------------------
  template <std::size_t I>
  constexpr const auto& get() const noexcept {
    // let overload resolution take its course
    return select<I>(*this).value;
  }
};

}} // spoon::detail

#endif /* SRC_SPOON_DETAIL_PACK_HELPER_HPP_ */
