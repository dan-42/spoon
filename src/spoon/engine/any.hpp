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
#include <spoon/engine/gear.hpp>
#include <spoon/detail/pack_helper.hpp>
#include <spoon/detail/for_each.hpp>
//#include <mapbox/variant.hpp>
#include <boost/variant.hpp>
#include <tuple>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <limits>
#include <cassert>
#include <iostream>

namespace spoon { namespace engine {

namespace detail {

///--------------------------------------------------------------------------------------------------------------------
template<typename Sink, typename Attr, typename Gear>
constexpr auto any_serialize_when_supported(std::true_type, bool& pass, Sink& sink, const Attr& attr, const Gear& gear) -> void {
  gear.serialize(pass, sink, attr);
}

///--------------------------------------------------------------------------------------------------------------------
template<typename Sink, typename Attr, typename Gear>
constexpr auto any_serialize_when_supported(std::false_type, bool& , Sink& , const Attr&, const Gear& ) -> void {
}

///--------------------------------------------------------------------------------------------------------------------
template<typename Gears, typename Sink>
class any_visitor : public boost::static_visitor<> {
public:

  ///------------------------------------------------------------------------------------------------------------------
  constexpr any_visitor(bool& pass, Sink& sink, const Gears& gears) : pass_{pass}, sink_{sink}, gears_{gears} {
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename T>
  auto operator()(const T& t) const -> void {

    auto foo = [this, &t](const auto& gear) {
      if(pass_ == false) {
        using gear_attribute_type = typename std::decay_t<decltype(gear)>::attribute_type;
        using is_attr_type_supported = typename std::is_same<gear_attribute_type, T>::type;
        any_serialize_when_supported(is_attr_type_supported{}, pass_, sink_, t, gear);
      }
    };
    ::spoon::detail::for_each_element(foo, gears_);
  }

private:
  bool& pass_;
  Sink& sink_;
  const Gears& gears_;
};

///--------------------------------------------------------------------------------------------------------------------
template<typename Sink, typename VarAttr, typename Gears>
auto visit_and_serialize(bool& pass, Sink& sink, const VarAttr& var_attr, const Gears& gears) {
  any_visitor<Gears, Sink> visitor{pass, sink, gears};
  boost::apply_visitor( visitor, var_attr);
}

} //detail


///--------------------------------------------------------------------------------------------------------------------
template<typename VariantAttr, typename... Gears>
struct any : gear< any<VariantAttr, Gears...>,  VariantAttr>{

  ///------------------------------------------------------------------------------------------------------------------
  using gears_type = spoon::detail::pack_storage<std::index_sequence_for<Gears...>, Gears...>;
  using number_of_gears = std::integral_constant<size_t, sizeof...(Gears)>;

  ///------------------------------------------------------------------------------------------------------------------
  const gears_type gears;

  ///------------------------------------------------------------------------------------------------------------------
  template<typename OStream>
  constexpr inline  auto what(OStream& os) -> void {
  }

  ///------------------------------------------------------------------------------------------------------------------
  constexpr any(const Gears& ...fwd_gears) : gears{fwd_gears...} {
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink, typename Attr>
  auto serialize_attr(bool& pass, Sink& sink, Attr attr) const {
  }

  ///------------------------------------------------------------------------------------------------------------------
  constexpr inline const auto& const_this() const noexcept {
    return (*this);
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink, typename Attr>
  auto serialize(bool& pass, Sink& sink, const Attr& attr) const {
    pass = false;
    detail::visit_and_serialize<Sink, Attr, decltype(gears)>(pass, sink, attr, gears);
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Itr, typename Attr>
  auto deserialize(bool& pass, Itr& start, const Itr& end, Attr& attr) const {
    pass = false;
    auto foo = [&](const auto& gear) {
      if(pass == false) {
        using gear_attribute_type = typename std::decay_t<decltype(gear)>::attribute_type;
        gear_attribute_type tmp{};
        gear.deserialize(pass, start, end, tmp);
        if(pass) {
          attr = tmp;
        }
      }
    };
    ::spoon::detail::for_each_element(foo, gears);
  }
};


}} //spoon::engine




namespace spoon {

///--------------------------------------------------------------------------------------------------------------------
template<typename VariantAttr, typename... Gears>
constexpr auto any(const Gears&... gears) -> spoon::engine::any<VariantAttr, Gears...> {
  constexpr ::spoon::detail::duplication_guard<std::decay_t<Gears>...> guard_type{};
  return spoon::engine::any<VariantAttr, Gears...> {gears...};
}

} // spoon


#endif /* SRC_SPOON_ENGINE_ANY_HPP_ */
