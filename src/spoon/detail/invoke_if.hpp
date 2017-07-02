/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_DETAIL_INVOKE_IF_HPP_
#define SRC_SPOON_DETAIL_INVOKE_IF_HPP_

namespace spoon { namespace detail {

template<typename Handler>
constexpr inline auto invoke_if(std::true_type , Handler&& handler) -> void {
  handler();
}

template<typename Handler>
constexpr inline auto invoke_if(std::false_type , Handler&& handler) -> void {
}

}}

#endif /* SRC_SPOON_DETAIL_INVOKE_IF_HPP_ */
