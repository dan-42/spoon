/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_LIB_SPOON_HPP_
#define SRC_LIB_SPOON_HPP_

#include <utility>
#include <cstdint>

namespace spoon {

  template<typename Sink, typename Engine, typename ...Attr>
  auto serialize(Sink& sink, const Engine& engine, const Attr&... attr) -> bool {
    bool pass{true};
    engine.serialize(pass, sink, attr...);
    return pass;
  }

  template<typename Sink, typename Engine>
  auto serialize(Sink& sink, const Engine& engine) -> bool {
    bool pass{true};
    engine.serialize(pass, sink);
    return pass;
  }

//-------------------------------------------------------------------------------------------------

  template<typename Iterator, typename Engine, typename Attr>
  auto deserialize(Iterator& start, const Iterator& end, const Engine& engine, Attr& attr) -> bool {
    bool pass{true};
    engine.deserialize(pass, start, end, attr);
    return pass;
  }

  template<typename Iterator, typename Engine>
  auto deserialize(Iterator& start, const Iterator& end, const Engine& engine) -> bool {
    bool pass{true};
    engine.deserialize(pass, start, end);
    return pass;
  }

}/*namespace spoon*/


#endif /* SRC_LIB_SPOON_HPP_ */
