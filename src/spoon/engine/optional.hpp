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

#include  <spoon/traits/has_call_operator.hpp>
#include  <spoon/engine/base.hpp>
#include  <spoon.hpp>
#include <boost/optional.hpp>
#include <type_traits>
#include <utility>


namespace spoon { namespace engine {

  namespace detail {

    template<typename Gear, typename ExpectedProvider>
    struct optional_with_expected_provider  : Gear , ExpectedProvider {
      using size_type = std::size_t;
      using attribute_type = boost::optional<typename Gear::attribute_type>;

      template<typename FwdGear, typename FwdExpectedProvider>
      constexpr optional_with_expected_provider(FwdGear&& fwd_gear, FwdExpectedProvider&& fwd_expected_provider)
      : Gear{std::forward<FwdGear>(fwd_gear)}
      , ExpectedProvider{std::forward<FwdExpectedProvider>(fwd_expected_provider)} {}

      auto& as_gear() const noexcept { return static_cast< const Gear&>(*this); }
      auto& as_expected_provider() const noexcept { return static_cast< const ExpectedProvider&>(*this); }

      /**
       * handling infinitive, min and max
       */
      template<typename Sink, typename OptAttr>
      inline auto serialize(bool& pass, Sink& sink, OptAttr&& opt_attr)  const -> void {
        const auto is_expected = as_expected_provider()();
        if(opt_attr && is_expected) {
          pass = spoon::serialize(sink, as_gear(), opt_attr.value());
        }
      }

      /** deserialize
       */
      template<typename Iterator, typename OptAttr>
      inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, OptAttr& opt_attr)  const -> void {
        using value_type = typename OptAttr::value_type;
        const auto is_expected = as_expected_provider()();
        if(is_expected) {
          value_type value{};
          pass = spoon::deserialize(start, end, as_gear(), value);
          if(pass) {
            opt_attr = value;
          }
        }
      }

    };

    template<typename Gear>
    struct optional  : gear<optional<Gear>, typename Gear::attribute_type>, Gear {

      //using attribute_type =  boost::optional<typename Gear::attribute_type>;

      constexpr optional(const Gear& fwd_gears) : Gear{fwd_gears} {
      }
      constexpr optional(Gear&& fwd_gears) : Gear{std::forward<Gear>(fwd_gears)} {
      }

      constexpr auto& as_gear() const noexcept { return static_cast< const Gear&>(*this); }

      template<typename ExpectedProvider, typename = std::enable_if_t<spoon::traits::has_call_operator<ExpectedProvider>::value> >
      constexpr inline auto operator()(ExpectedProvider&& expected_provider)  const {
        return optional_with_expected_provider<Gear, ExpectedProvider>{std::move(as_gear()), std::forward<ExpectedProvider>(expected_provider)};
      }

      /**
       *
       */
      template<typename Sink, typename OptAttr>
      constexpr inline auto serialize(bool& pass, Sink& sink, OptAttr&& opt_attr)  const -> void {
        if(opt_attr) {
          pass = spoon::serialize(sink, as_gear(), opt_attr.value());
        }
      }

      /** deserialize
       */
      template<typename Iterator, typename OptAttr>
      constexpr inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, OptAttr& opt_attr)  const -> void {
        using value_type = typename OptAttr::value_type;
        value_type value{};
        if(spoon::deserialize(start, end, as_gear(), value)) {
          opt_attr = value;
        }
      }

    };
  }}}// spoon::engine::detail

namespace spoon { namespace engine {

  struct optional {
    template<typename FwdGear>
    constexpr auto inline operator[](FwdGear&& fwd_gear) const {
        return detail::optional<FwdGear>(std::forward<FwdGear>(fwd_gear));
    }
    template<typename Gear>
    constexpr auto inline operator[](const Gear& gear) const {
        return detail::optional<Gear>(gear);
    }

    template<typename FwdGear>
    constexpr auto inline operator()(FwdGear&& fwd_gear) const {
        return detail::optional<FwdGear>(std::forward<FwdGear>(fwd_gear));
    }
    template<typename Gear>
    constexpr auto inline operator()(const Gear& gear) const {
        return detail::optional<Gear>(gear);
    }
  };

}} //spoon::engine


namespace spoon {

constexpr auto optional = engine::optional{};

} // spoon



#endif /* SRC_SPOON_ENGINE_OPTIONAL_HPP_ */
