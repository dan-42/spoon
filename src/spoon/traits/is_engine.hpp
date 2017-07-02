/*
 * is_engine.hpp
 *
 *  Created on: Jul 1, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_TRAITS_IS_ENGINE_HPP_
#define SRC_SPOON_TRAITS_IS_ENGINE_HPP_

#include <spoon/engine/base.hpp>
#include <type_traits>

namespace spoon { namespace traits {

template<typename T>
struct is_engine_type {

  using type = typename std::is_base_of<::spoon::engine::base, T>::type;

};


constexpr inline auto is_engine(const auto& engine) -> bool {
  using engine_type   = typename std::decay<decltype(engine)>::type;
  using is_engine     = typename is_engine_type<engine_type>::type;
  return is_engine::value;
}


}}


#endif /* SRC_SPOON_TRAITS_IS_ENGINE_HPP_ */
