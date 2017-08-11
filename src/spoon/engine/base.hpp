/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SRC_SPOON_ENGINE_BASE_HPP_
#define SRC_SPOON_ENGINE_BASE_HPP_

namespace spoon {

 /**
  * forward declaration for recursive calls inside of complex engines
  */
// constexpr auto serialize  (auto& sink,                   auto&& attr, const auto& engine, auto& ctx) -> bool;
// constexpr auto deserialize(auto& start, const auto& end, auto&  attr, const auto& engine, auto& ctx) -> bool;

} //spoon


namespace spoon { namespace tag {

  struct big_endian{};
  struct little_endian{};

}}

namespace spoon { namespace engine {

  /**
   *  base class to identify an engine via traits
   */
  struct base{};

  struct gear_base{};

}}



#endif /* SRC_SPOON_ENGINE_BASE_HPP_ */
