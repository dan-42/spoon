/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SRC_SPOON_TRAITS_TRAITS_HPP_
#define SRC_SPOON_TRAITS_TRAITS_HPP_

#include <type_traits>

namespace spoon { namespace traits { namespace detail {
    template<typename Sink, typename Attr>
    inline auto serialize_to_sink(Sink &sink, Attr&& attr) -> bool {
      sink.push_back(attr);
      return true;
    }

    template<typename Iterator, typename Attr>
    inline auto deserialize_from_source(Iterator& start, const Iterator& end, Attr&attr) -> bool {
      if(start != end) {
        attr = *start++;
        return true;
      }
      return false;
    }

    template<typename Value, typename Attr>
    inline auto assign_to(Value&& value, Attr& attr) -> bool {
      attr = value;
      return true;
    }
}}}


#endif /* SRC_SPOON_TRAITS_TRAITS_HPP_ */
