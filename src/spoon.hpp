/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_LIB_SPOON_HPP_
#define SRC_LIB_SPOON_HPP_

#include <utility>
#include <cstdint>


#include <spoon/type/unused.hpp>
#include <spoon/traits/is_engine.hpp>
#include <spoon/traits/is_supported_engine_type.hpp>

#include <spoon/binary.hpp>
#include <spoon/seq.hpp>
#include <spoon/any.hpp>


namespace spoon {

  constexpr auto serialize(auto& sink, auto&& attr, const auto& engine, auto& ctx) -> bool {
    static_assert(traits::is_engine(engine)          == true, "ERROR provided variable engine is not a spoon::engine type");
    static_assert(traits::is_supported(engine, attr) == true, "ERROR provided attr is not supported by the provided engine");
    using engine_type = typename std::decay<decltype(engine)>::type;
    return engine_type::serialize(sink, std::move(attr), ctx);;
  }


  constexpr auto serialize(auto& sink, auto&& attr, const auto& engine) -> bool  {
   return serialize(sink, std::move(attr), engine, spoon::type::unused);
  }

//-------------------------------------------------------------------------------------------------

  constexpr auto deserialize(auto& start, const auto& end, auto& attr, const auto& engine, auto& ctx) -> bool {
    static_assert(traits::is_engine(engine)          == true, "ERROR provided variable engine is not a spoon::engine type");
    static_assert(traits::is_supported(engine, attr) == true, "ERROR provided attr is not supported by the provided engine");
    using engine_type = typename std::decay<decltype(engine)>::type;
    return engine_type::deserialize(start, end, attr, ctx);
  }

  constexpr auto deserialize(auto& start, const auto& end, auto& attr, const auto& engine) -> bool {
   return deserialize(start, end, attr, engine, spoon::type::unused);
  }

}/*namespace spoon*/


#endif /* SRC_LIB_SPOON_HPP_ */
