/*
 * invoke_if.hpp
 *
 *  Created on: Jul 2, 2017
 *      Author: dan
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
