/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_DETAIL_GEAR_HPP_
#define SRC_SPOON_DETAIL_GEAR_HPP_

#include <spoon/detail/without_attr.hpp>

#include <spoon/traits/has_call_operator.hpp>

#include <utility>
#include <type_traits>

namespace spoon { namespace detail {


template<typename GearImpl>
struct gear : GearImpl
{
  using attribute_type = typename GearImpl::attribute_type;
  using this_type = gear<GearImpl>;

  constexpr gear() : GearImpl{} {
  }

  template<typename FwdGearImpl>
  constexpr gear(FwdGearImpl&& fwd_gear_impl) : GearImpl{std::forward<FwdGearImpl>(fwd_gear_impl)} {
  }

  auto& as_impl() const noexcept { return static_cast< const GearImpl&>(*this); }

  //serialize
  template<typename Sink, typename... Attr>
  inline auto operator()(bool& pass, Sink& sink, Attr&& ...attr) const -> void {
    as_impl()(pass, sink, std::forward<Attr...>(attr...));
  }
  template<typename Sink>
  inline auto operator()(bool& pass, Sink& sink) const -> void {
    as_impl()(pass, sink);
  }

  //deserialize
  template<typename Iterator>
  inline auto operator()(bool& pass, Iterator& start, const Iterator& end, attribute_type& attr) const -> void {
    as_impl()(pass, start, end, attr);
  }

  //
  template<typename Attr, typename = std::enable_if_t<std::is_integral<Attr>::value> >
  constexpr inline auto operator()(Attr attr) const {
    auto engine = with_attr_member<this_type, Attr>{std::move(attr)};
    return gear<decltype(engine)>{std::move(engine)};
  }

  template<typename AttrProvider, typename = std::enable_if_t<spoon::traits::has_call_operator<AttrProvider>::value> >
  constexpr inline auto operator()(AttrProvider&& attr_provider) const {
    auto engine = with_attr_provider<this_type, AttrProvider>{std::move(attr_provider)};
    return gear<decltype(engine)>{std::move(engine)};
  }

};


}}

#endif /* SRC_SPOON_DETAIL_GEAR_HPP_ */
