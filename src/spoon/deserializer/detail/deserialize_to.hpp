/*
 * deserialize_to.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_DETAIL_DESERIALIZE_TO_HPP_
#define SRC_SPOON_DESERIALIZER_DETAIL_DESERIALIZE_TO_HPP_


#include <spoon/deserialize_to.hpp>

#include <type_traits>

namespace spoon { namespace deserializer {


namespace detail {
  const constexpr auto deserialize_to = [](auto& start, const auto& end, auto& value) -> bool {
    using start_type = typename std::remove_reference<decltype(start)>::type;
    using end_type   = typename std::remove_reference<decltype(end)>::type;
    using value_type = typename std::remove_reference<decltype(value)>::type;
    return deserialize<start_type, end_type, value_type>::to(start, end, value);
  };

}}}


#endif /* SRC_SPOON_DESERIALIZER_DETAIL_DESERIALIZE_TO_HPP_ */
