/*
 * serializer.hpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZE_TO_HPP_
#define SRC_SPOON_SERIALIZE_TO_HPP_


namespace spoon { namespace serializer {

/**
 * customizable point
 */
  template<typename Container, typename Value>
  struct serialize {

    static auto to(Container& container, Value&& value) -> bool {
      container.emplace_back(std::move(value));
      return true;
    }

    static auto to(Container& container, const Value& value) -> bool {
      container.push_back(value);
      return true;
    }

  };

}}


#endif /* SRC_SPOON_SERIALIZE_TO_HPP_ */
