/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SRC_SPOON_ENGINE_ANY_HPP_
#define SRC_SPOON_ENGINE_ANY_HPP_



#include  <spoon/engine/base.hpp>
#include  <spoon/traits/is_supported_engine_type.hpp>
#include  <spoon/detail/invoke_if.hpp>
#include  <spoon/detail/for_each.hpp>

#include <mapbox/variant.hpp>

#include <type_traits>
#include <tuple>



namespace spoon { namespace engine {

namespace detail {

  template<typename Handler>
  struct any_visitor {

    any_visitor(const Handler& handler) : handler_(handler) {}

    template<typename Attr>
    void operator()(Attr&& attr) const {
      handler_(attr);
    }

    const Handler&   handler_;
  };



} //detail





  //todo add tag dispatching for fusion and std::tuples
  template<typename Attr, typename... Engines>
  struct any : base {
    using tag_type          = tag::any;
    using has_context       = std::false_type;
    using attr_type         = Attr;

    using any_engine_type   = std::tuple<Engines...>;
    using number_of_engines = std::integral_constant<std::size_t, sizeof...(Engines)>;

    /**
     * any serializer
     */
    static constexpr inline auto serialize(auto& sink, auto&& variant_attr, auto& ctx) -> bool {

      bool pass                      = false;
      const auto variant_handler = [&pass, &sink, &ctx](auto&& attr) {

        const auto engine_handler = [&pass, &sink, &attr, &ctx](auto&& engine) {
          if(!pass) {

            using engine_type = std::decay_t<decltype(engine)>;
            using scope_attr_type = std::decay_t<decltype(attr)>;
            using supported = typename spoon::traits::is_supported_attr_type<engine_type, scope_attr_type>::type;

            auto handler = [&sink, &pass, &ctx, captured_attr = std::move(attr) ]() mutable {
              // direct call because global type_check would yield error on static_assert!
              pass = engine_type::serialize(sink, std::move(captured_attr), ctx);
            };
            spoon::detail::invoke_if(supported{}, std::move(handler));
          }
        };
        using engine_handler_type = std::decay_t<decltype(engine_handler)>;
        spoon::detail::for_each_type<engine_handler_type, Engines...>(engine_handler);
      };

      using handler_type = std::decay_t<decltype(variant_handler)>;
      detail::any_visitor<handler_type> any_visitor{variant_handler};
      mapbox::util::apply_visitor(any_visitor, variant_attr);

      return pass;
    }

    /**
     * any deserializer
     */
    static constexpr inline auto deserialize(auto& start, const auto& end, auto& variant_attr,  auto& ctx) -> bool {

      bool pass = false;

      const auto engine_handler = [&pass, &start, &end, &variant_attr, &ctx](auto&& engine) {
        if(!pass) {

          using engine_type = std::decay_t<decltype(engine)>;
          using attr_type = typename engine_type::attr_type;

          attr_type attr{};

          pass = ::spoon::deserialize(start, end, attr, engine, ctx);
          if(pass) {
            variant_attr = attr;
          }
        }
      };
      using handler_type = std::decay_t<decltype(engine_handler)>;
      spoon::detail::for_each_type<handler_type, Engines...>(engine_handler);
      return pass;
    }
  };



}}


namespace spoon {

  template<typename Attr, typename... Engines>
  constexpr inline auto any(Engines... /*engines*/) noexcept -> engine::any<Attr, Engines...> {
    return engine::any<Attr, Engines...>{};
  }


}



#endif /* SRC_SPOON_ENGINE_ANY_HPP_ */
