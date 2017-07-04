/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SRC_SPOON_TRAITS_IS_SUPPORTED_ENGINE_TYPE_HPP_1
#define SRC_SPOON_TRAITS_IS_SUPPORTED_ENGINE_TYPE_HPP_1

#include <type_traits>
#include <typeinfo>

namespace spoon { namespace traits {


template<class Engine, class Attr, class Enable = void>
struct is_supported_attr_type {
  using type = std::false_type;
};


/**
 * Enable if  Engine::attr_type == Attr
 */
template<class Engine, class Attr>
struct is_supported_attr_type <Engine, Attr, typename std::enable_if<std::is_same<typename Engine::attr_type, Attr>::value >::type > {
  using type = std::true_type;
};



constexpr inline auto is_supported(const auto& engine, const auto& attr)  -> bool {
  using engine_type = std::decay_t<decltype(engine)>;
  using attr_type   = std::decay_t<decltype(attr)>;
  using supported   = typename is_supported_attr_type<engine_type, attr_type>::type;
  return supported::value;
}

}}

#define SPOON_REGISTER_ATTR_TO_ENGINE(engine_type, attr_type)                                      \
template<>                                                                                         \
struct spoon::traits::is_supported_attr_type <engine_type, attr_type> {                            \
  using type = std::true_type;                                                                     \
};                                                                                                 \

#endif /* SRC_SPOON_TRAITS_IS_SUPPORTED_ENGINE_TYPE_HPP_ */
