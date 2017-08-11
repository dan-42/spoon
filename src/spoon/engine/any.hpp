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

#include <spoon/traits/has_call_operator.hpp>
#include <spoon/engine/base.hpp>
//#include <mapbox/variant.hpp>
#include <boost/variant.hpp>
#include <tuple>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <limits>
#include <cassert>

namespace spoon { namespace engine {

namespace detail {

  template<typename Gear, typename Sink>
  class any_visitor : public boost::static_visitor<> {
  public:

    constexpr any_visitor(bool& pass, Sink& sink, const Gear& gear) : pass_{pass}, sink_{sink}, gear_{gear} {
    }

    template<typename T>
    auto operator()(T t) const -> void {
        gear_.serialize_attr(pass_, sink_, std::move(t));
    }

  private:
    bool& pass_;
    Sink& sink_;
    const Gear& gear_;
  };


  template<typename Sink, typename VarAttr, typename Gear>
  auto visit_and_serialize(bool& pass, Sink& sink, VarAttr var_attr, const Gear& gear) {
    any_visitor<Gear, Sink> visitor{pass, sink, gear};
    boost::apply_visitor( visitor, var_attr);
  }
}

struct alternative_base{};


template<typename T>
struct is_alternative : std::false_type{};

template<typename T1, typename T2>
struct alternative : private alternative_base, T1, T2 {

  using attribute_type = typename T1::attribute_type;
  using this_type = alternative<T1, T2>;

  template<typename FwdT1, typename FwdT2>
  constexpr alternative(FwdT1&& fwd_t1, FwdT2&& fwd_t2) : T1{std::forward<FwdT1>(fwd_t1)}, T2{std::forward<FwdT2>(fwd_t2)}
  {}

  auto& as_t1() const noexcept { return static_cast< const T1&>(*this); }
  auto& as_t2() const noexcept { return static_cast< const T2&>(*this); }

  // t2 is also an alternative
  //so we call serialize_attr as we already visited the variant
  template<typename Sink, typename Attr>
  auto serialize_attr(std::true_type, bool& pass, Sink& sink, Attr& attr) const {
    as_t1().serialize_attr(pass, sink, attr);
  }

  //t2 is the last type
  template<typename Sink, typename Attr>
  auto serialize_attr(std::false_type, bool& pass, Sink& sink, Attr& attr) const {
    as_t1().serialize(pass, sink, attr);
  }

  template<typename Sink, typename Attr>
  auto serialize_attr( bool& pass, Sink& sink, Attr attr) const {
    using is_t1_an_alternative = typename std::is_base_of<alternative_base, T1>::type;
    serialize_attr(is_t1_an_alternative{}, pass, sink, attr);

    if(!pass) {
      as_t2().serialize(pass, sink, attr);
    }
    else {
      pass = true;
    }
  }

  // entry point
  template<typename Sink, typename VariantAttr>//, typename std::enable_if_t<true>>
  auto serialize(bool& pass, Sink& sink, VariantAttr attr) const {

    detail::visit_and_serialize<Sink, VariantAttr, this_type>(pass, sink, std::move(attr), *this);
  }
};

template <typename T1, typename T2>
struct is_alternative <alternative<T1, T2>>: std::true_type{};


template<typename T1, typename T2, typename = std::enable_if_t<std::is_base_of<base, T1>::value && std::is_base_of<base, T2>::value> >
constexpr auto operator|(T1&& a, T2&& b) {
  return alternative<T1, T2>(std::forward<T1>(a), std::forward<T2>(b));
}

template<typename T1, typename T2, typename = std::enable_if_t<std::is_base_of<base, T1>::value && std::is_base_of<base, T2>::value> >
constexpr auto operator|(const T1 &a, const T2 &b) {
  return alternative<T1, T2>(a, b);
}

//alternative<alternative<alternative<T3, T4>>>

}} //spoon::engine




namespace spoon {

using ::spoon::engine::operator|;

//auto v = any[a, b, c, f];

} // spoon












////////////////////////////////////////////////////////////////////////////////
//namespace spoon { namespace engine {
//
//namespace detail {
//
//  template<typename Handler>
//  struct any_visitor {
//
//    any_visitor(const Handler& handler) : handler_(handler) {}
//
//    template<typename Attr>
//    void operator()(Attr&& attr) const {
//      handler_(attr);
//    }
//
//    const Handler&   handler_;
//  };
//
//
//
//} //detail
//
//
//
//
//
//  //todo add tag dispatching for fusion and std::tuples
//  template<typename Attr, typename... Engines>
//  struct any : base {
//    using tag_type          = tag::any;
//    using has_context       = std::false_type;
//    using attr_type         = Attr;
//
//    using any_engine_type   = std::tuple<Engines...>;
//    using number_of_engines = std::integral_constant<std::size_t, sizeof...(Engines)>;
//
//    /**
//     * any serializer
//     */
//    static constexpr inline auto serialize(auto& sink, auto&& variant_attr, auto& ctx) -> bool {
//
//      bool pass                      = false;
//      const auto variant_handler = [&pass, &sink, &ctx](auto&& attr) {
//
//        const auto engine_handler = [&pass, &sink, &attr, &ctx](auto&& engine) {
//          if(!pass) {
//
//            using engine_type = std::decay_t<decltype(engine)>;
//            using scope_attr_type = std::decay_t<decltype(attr)>;
//            using supported = typename spoon::traits::is_supported_attr_type<engine_type, scope_attr_type>::type;
//
//            auto handler = [&sink, &pass, &ctx, captured_attr = std::move(attr) ]() mutable {
//              // direct call because global type_check would yield error on static_assert!
//              pass = engine_type::serialize(sink, std::move(captured_attr), ctx);
//            };
//            spoon::detail::invoke_if(supported{}, std::move(handler));
//          }
//        };
//        using engine_handler_type = std::decay_t<decltype(engine_handler)>;
//        spoon::detail::for_each_type<engine_handler_type, Engines...>(engine_handler);
//      };
//
//      using handler_type = std::decay_t<decltype(variant_handler)>;
//      detail::any_visitor<handler_type> any_visitor{variant_handler};
//      mapbox::util::apply_visitor(any_visitor, variant_attr);
//
//      return pass;
//    }
//
//    /**
//     * any deserializer
//     */
//    static constexpr inline auto deserialize(auto& start, const auto& end, auto& variant_attr,  auto& ctx) -> bool {
//
//      bool pass = false;
//
//      const auto engine_handler = [&pass, &start, &end, &variant_attr, &ctx](auto&& engine) {
//        if(!pass) {
//
//          using engine_type = std::decay_t<decltype(engine)>;
//          using attr_type = typename engine_type::attr_type;
//
//          attr_type attr{};
//
//          pass = ::spoon::deserialize(start, end, attr, engine, ctx);
//          if(pass) {
//            variant_attr = attr;
//          }
//        }
//      };
//      using handler_type = std::decay_t<decltype(engine_handler)>;
//      spoon::detail::for_each_type<handler_type, Engines...>(engine_handler);
//      return pass;
//    }
//  };
//
//
//
//}}
//
//
//namespace spoon {
//
//  template<typename Attr, typename... Engines>
//  constexpr inline auto any(Engines... /*engines*/) noexcept -> engine::any<Attr, Engines...> {
//    return engine::any<Attr, Engines...>{};
//  }
//
//
//}
//
//

#endif /* SRC_SPOON_ENGINE_ANY_HPP_ */
