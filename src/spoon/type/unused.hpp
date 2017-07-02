/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_UNUSED_TYPE_HPP_
#define SRC_SPOON_UNUSED_TYPE_HPP_


namespace spoon { namespace type {

  /**
   * unused_type used e.g. as an unused ctx
   */
  struct unused_type{};

  constexpr unused_type unused{};

}
}  // namespace spoon


#endif /* SRC_SPOON_UNUSED_TYPE_HPP_ */
