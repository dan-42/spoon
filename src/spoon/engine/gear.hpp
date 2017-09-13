/**
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SRC_SPOON_ENGINE_GEAR_HPP__
#define SRC_SPOON_ENGINE_GEAR_HPP__

#include <spoon/engine/base.hpp>

#include <spoon/detail/without_attr.hpp>
#include <spoon/traits/has_call_operator.hpp>

#include <utility>
#include <type_traits>

namespace spoon { namespace engine {

namespace detail {

  template<class T, class Attr>
  using enable_if_is_attribute_t = typename std::enable_if<
    std::is_same<Attr, T>::value
    && !spoon::traits::has_call_operator<T>::value
  >::type;

  template<class T, class Attr>
  using enable_if_has_call_operator_t = typename std::enable_if<
    !std::is_same<Attr, T>::value
    && spoon::traits::has_call_operator<T>::value
  >::type;


} //detail


///--------------------------------------------------------------------------------------------------------------------
template<typename Derived, typename AttrType>
struct gear : gear_base {

  ///------------------------------------------------------------------------------------------------------------------
  using attribute_type = AttrType;

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Attr,
   detail::enable_if_is_attribute_t<Attr, attribute_type>* = nullptr>
  constexpr inline auto operator()(Attr&& attr) const {
    return ::spoon::detail::with_attr_member<Derived, Attr>{Derived{}, std::forward<Attr>(attr)};
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename AttrProvider,
  detail::enable_if_has_call_operator_t<AttrProvider, attribute_type>* = nullptr>
  constexpr inline auto operator()(AttrProvider&& attr_provider) const {
    return spoon::detail::with_attr_provider<Derived, attribute_type, AttrProvider>{Derived{}, std::forward<AttrProvider>(attr_provider)};
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename AttrProvider,
  detail::enable_if_has_call_operator_t<AttrProvider, attribute_type>* = nullptr>
  constexpr inline auto operator()(const AttrProvider& attr_provider) const {
    return spoon::detail::with_attr_provider<Derived, attribute_type, AttrProvider>{Derived{}, attr_provider};
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink, typename Attr>
  constexpr inline auto serialize(bool& pass, Sink& sink, const Attr& attr) const -> void {
    c_derived().serialize(pass, sink, attr);
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr, typename Attr>
  constexpr inline auto deserialize(bool& pass, Itr& begin, const Itr& end, Attr& attr) const -> void {
    c_derived().deserialize(pass, begin, end, attr);
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename OStream>
  constexpr inline auto what(OStream& os) -> void {
    c_derived().what(os);
  }

private:
  ///------------------------------------------------------------------------------------------------------------------
  auto derived() -> Derived& {
    return *static_cast<Derived*>(this);
  }

  ///------------------------------------------------------------------------------------------------------------------
  auto c_derived() const -> const Derived& {
    return *static_cast<const Derived*>(this);
  }
};

//
//template<typename Attr, typename Serialize, typename Deserialize>
//struct gear_wrapper : Serialize, Deserialize {
//  using attribute_type = Attr;
//
//  template<typename FwdSerialize, typename FwdDeserialize>
//  inline gear_wrapper(FwdSerialize&& s, FwdDeserialize&& d)
//    : FwdSerialize{std::forward<FwdSerialize>(s)}
//    , FwdDeserialize{std::forward<FwdDeserialize>(d)}
//  {
//  }
//
//  template<typename FwdSerialize, typename FwdDeserialize>
//  inline gear_wrapper(const FwdSerialize& s, const FwdDeserialize& d)
//    : FwdSerialize{s}
//    , FwdDeserialize{d}
//  {
//  }
//
//  auto& as_serializer() const noexcept {
//    return static_cast<const Serialize&>(*this);
//  }
//  auto& as_deserializer() const noexcept {
//    return static_cast<const Deserialize&>(*this);
//  }
//
//  template<typename Sink>
//  inline auto serialize(bool& pass, Sink& sink, const Attr& attr) const {
//    as_serializer()(pass, sink, attr);
//  }
//
//  template<typename Iterator>
//  inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, Attr& attr) const {
//    as_deserializer()(pass, start, end, attr);
//  }
//};
//
//template<typename Attr, typename Serializer, typename Deserializer>
//decltype(auto) inline make_geare(Serializer&& s, Deserializer&& d) {
//  gear_wrapper<Attr, Serializer, Deserializer> wrapped{std::forward<Serializer>(s), std::forward<Deserializer>(d)};
//  return gear<decltype(wrapped)>(std::move(wrapped));
//}


}}

#endif /* SRC_SPOON_ENGINE_GEAR_HPP_ */
