/*
 * unused_type.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
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
