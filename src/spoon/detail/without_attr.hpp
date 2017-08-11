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

#include  <type_traits>
#include  <utility>

namespace spoon { namespace detail {


struct without_attr_base{};

template<typename Type, typename Attr>
struct with_attr_member : without_attr_base, Type {

  const Attr attr;

  constexpr with_attr_member(Attr&& a) : attr(std::move(a)){}

  auto& as_t()  const  noexcept { return static_cast< const Type&>(*this);   }

  template<typename Sink>
  auto serialize(bool b, Sink& sink)  const -> void {
    as_t().serialize(b, sink, attr);
  }
};

template<typename Type, typename AttrProvider>
struct with_attr_provider : without_attr_base, Type, AttrProvider {

  constexpr with_attr_provider(AttrProvider&& a) : AttrProvider(std::move(a)){}

  auto& as_type()  const  noexcept { return static_cast< const Type&>(*this);   }
  auto& as_attribute_provider()  const  noexcept { return static_cast< const AttrProvider&>(*this);   }

  template<typename Sink>
  auto serialize(bool b, Sink& sink)  const -> void {
    as_type().serialize(b, sink, as_attribute_provider()());
  }

};



template<class T>
struct expect_attribute : std::is_base_of<detail::without_attr_base, T> {};


}}

#endif /* SRC_SPOON_DETAIL_WITHOUT_ATTR_HPP_ */
