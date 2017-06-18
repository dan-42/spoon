/*
 * deserialize_to.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZE_TO_HPP_
#define SRC_SPOON_DESERIALIZE_TO_HPP_

#include <cassert>

namespace spoon { namespace deserializer {

/**
 * customizable point
 */
  template<typename Start, typename End, typename Value>
  struct deserialize {
    static bool to(Start& start, const End& end, Value& value) {
      assert(start != end);
      value = *start;
      return true;
    }

  };


}}



#endif /* SRC_SPOON_DESERIALIZE_TO_HPP_ */
