/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_DETAIL_WITHOUT_ATTR_HPP_
#define SRC_SPOON_DETAIL_WITHOUT_ATTR_HPP_

#include <spoon/engine/base.hpp>

#include  <type_traits>
#include  <utility>

namespace spoon { namespace detail {

///--------------------------------------------------------------------------------------------------------------------
struct with_attr_member_base{};

///--------------------------------------------------------------------------------------------------------------------
template<typename Gear, typename Return = typename std::is_base_of<with_attr_member_base, Gear>::type>
constexpr auto is_with_attr(const Gear&) -> Return {
  return {};
}

///--------------------------------------------------------------------------------------------------------------------
template<typename Gear, typename AttrType>
struct with_attr_member  : with_attr_member_base, ::spoon::engine::gear<Gear, AttrType> {

  ///------------------------------------------------------------------------------------------------------------------
  constexpr with_attr_member(Gear&& gear, AttrType&& attr)
    : ::spoon::engine::gear<Gear, AttrType>{std::forward<Gear>(gear)}, attr_{std::forward<AttrType>(attr)} {
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink>
  constexpr inline auto serialize(bool& pass, Sink& sink) const -> void {
    as_gear().serialize(pass, sink, attr_);
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr>
  constexpr inline auto deserialize(bool& pass, Itr& begin, const Itr& end) const -> void {
    AttrType deserialized_value{};
    as_gear().deserialize(pass, begin, end, deserialized_value);
    if(pass) {
      pass = (attr_ == deserialized_value);
    }
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr, typename Attr>
  constexpr inline auto deserialize(bool& pass, Itr& begin, const Itr& end, Attr& attr) const -> void {
    deserialize(pass, begin, end);
    if(pass) {
      attr = attr_;
    }
  }

private:
  ///------------------------------------------------------------------------------------------------------------------
  constexpr inline auto as_gear() const -> const ::spoon::engine::gear<Gear, AttrType>& {
    return *static_cast<const ::spoon::engine::gear<Gear, AttrType>*>(this);
  }

  const AttrType attr_;
};


///--------------------------------------------------------------------------------------------------------------------
struct with_attr_provider_base{};

///--------------------------------------------------------------------------------------------------------------------
template<typename Gear, typename Return = typename std::is_base_of<with_attr_provider_base, Gear>::type>
constexpr auto is_with_attr_provider(const Gear&) -> Return {
  return {};
}

///--------------------------------------------------------------------------------------------------------------------
template<typename Gear, typename AttributeType, typename AttrProvider>
struct with_attr_provider  : with_attr_provider_base
                           , ::spoon::engine::gear<Gear, AttributeType>
                           , AttrProvider {

  ///------------------------------------------------------------------------------------------------------------------
  constexpr with_attr_provider(Gear&& gear, AttrProvider&& attr)
    : ::spoon::engine::gear<Gear, AttributeType>{std::forward<Gear>(gear)}, AttrProvider{std::forward<AttrProvider>(attr)} {
  }

  ///------------------------------------------------------------------------------------------------------------------
   constexpr with_attr_provider(Gear&& gear, const AttrProvider& attr)
     : ::spoon::engine::gear<Gear, AttributeType>{std::forward<Gear>(gear)}, AttrProvider{attr} {
   }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink>
  constexpr inline auto serialize(bool& pass, Sink& sink) const -> void {
    as_gear().serialize(pass, sink, as_provider()());
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr>
  constexpr inline auto deserialize(bool& pass, Itr& begin, const Itr& end) const -> void {
    AttributeType deserialized_value{};
    as_gear().deserialize(pass, begin, end, deserialized_value);
    if(pass) {
      pass = (deserialized_value == as_provider()());
    }
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr, typename Attr>
  constexpr inline auto deserialize(bool& pass, Itr& begin, const Itr& end, Attr& attr) const -> void {
    AttributeType deserialized_value{};
    as_gear().deserialize(pass, begin, end, deserialized_value);
    if(pass) {
      const auto expected_value = as_provider()();
      pass = (deserialized_value == expected_value);
      if(pass) {
        attr = expected_value;
      }
    }
  }

private:
  ///------------------------------------------------------------------------------------------------------------------
  constexpr inline auto as_gear() const -> const ::spoon::engine::gear<Gear, AttributeType>& {
    return *static_cast<const ::spoon::engine::gear<Gear, AttributeType>*>(this);
  }

  ///------------------------------------------------------------------------------------------------------------------
  constexpr inline auto as_provider() const -> const AttrProvider& {
    return *static_cast<const AttrProvider*>(this);
  }

};

///------------------------------------------------------------------------------------------------------------------
template<class T, typename TrueFalseType = std::integral_constant<bool,
                                           (  std::is_base_of<with_attr_member_base,   T>::value
                                           || std::is_base_of<with_attr_provider_base, T>::value)> >
struct expect_attribute : TrueFalseType {};


}}//sppon::detail

#endif /* SRC_SPOON_DETAIL_WITHOUT_ATTR_HPP_ */
