/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_ENGINE_OPTIONAL_HPP_
#define SRC_SPOON_ENGINE_OPTIONAL_HPP_


#include  <spoon/engine/base.hpp>
#include  <spoon/traits/is_supported_engine_type.hpp>
#include  <spoon/detail/invoke_if.hpp>
#include  <spoon/detail/for_each.hpp>
//todo move serialize into from binary
#include  <spoon/binary.hpp>

#include <mapbox/variant.hpp>

#include <type_traits>
#include <tuple>


namespace spoon { namespace traits {

template<typename Engine, typename Ctx>
constexpr auto expect_optional(Engine, Ctx& ctx) -> bool {
  return true;
}

}}


namespace spoon { namespace engine {

namespace detail {



} //detail





  //todo add tag dispatching for std::optional, boost::optional ... if API differs
  template<typename Attr, typename Engine>
  struct optional : base {
    using tag_type          = tag::optional;
    using has_context       = std::false_type;
    using attr_type         = Attr;

    using optional_engine_type   = Engine;
    using optional_value_type    = typename Attr::value_type;
    using this_type              = optional<Attr, Engine>;

    /**
     * optional serializer
     */
    static constexpr inline auto serialize(auto& sink, auto&& optional_attr, auto& ctx) -> bool {

      if(optional_attr && spoon::traits::expect_optional(this_type{}, ctx)) {
        using sink_type = std::decay_t<decltype(sink)>;
        sink_type optional_sink{};
        bool pass = optional_engine_type::serialize(optional_sink, optional_attr.value(), ctx);

        if(pass) {
          //todo  make it more generic
          for(auto c : optional_sink ) {
            if(!detail::serialize_to_sink(sink , std::move(c))) { return false; }
          }
        }
      }
      return true;
    }

    /**
     * optional deserializer
     */
    static constexpr inline auto deserialize(auto& start, const auto& end, auto& optional_attr,  auto& ctx) -> bool {

      if(!spoon::traits::expect_optional(this_type{}, ctx)) {
        return true;
      }
      optional_value_type value{};

      auto pass = optional_engine_type::deserialize(start, end, value, ctx);
      if(pass) {
        optional_attr = value;
      }
      return true;
    }
  };



}}


namespace spoon {

  template<typename Attr, typename Engine>
  constexpr inline auto optional(Engine /*engines*/) noexcept -> engine::optional<Attr, Engine> {
    return engine::optional<Attr, Engine>{};
  }

}


#endif /* SRC_SPOON_ENGINE_OPTIONAL_HPP_ */
