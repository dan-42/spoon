/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef SRC_SPOON_ENGINE_SEQ_HPP_
#define SRC_SPOON_ENGINE_SEQ_HPP_

#include <spoon/engine/base.hpp>
#include <spoon.hpp>
#include <spoon/detail/pack_helper.hpp>
#include <spoon/traits/is_supported_engine_type.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>

#include <type_traits>
#include <tuple>

namespace spoon { namespace engine {

namespace detail {

  ///------------------------------------------------------------------------------------------------------------------
  template <class F, class Tuple, class FusionSequence, std::size_t... I>
  constexpr auto for_each_pair(F&& f, const Tuple& t, FusionSequence& s, std::index_sequence<I...>) -> F {
    return (void)std::initializer_list<int>{(std::ref(f)(t. template  get<I>(), boost::fusion::at_c<I>(s)), 0)...}, f;
  }

  ///------------------------------------------------------------------------------------------------------------------
  template <class F, class TupleEngines, class FusionSequence>
  constexpr auto for_each_pair(F&& f, const TupleEngines& t, FusionSequence& s) -> F {
    return for_each_pair(std::forward<F>(f), t, s, std::make_index_sequence<TupleEngines::number_of_elements>{});
  }

} //detail


///------------------------------------------------------------------------------------------------------------------
template<typename Attr, typename... Engines>
struct seq_fusion : gear<seq_fusion<Attr, Engines...>, Attr>{

  using gears_type = spoon::detail::pack_storage<std::index_sequence_for<Engines...>, Engines...>;
  using number_of_gears = std::integral_constant<size_t, sizeof...(Engines)>;

  ///------------------------------------------------------------------------------------------------------------------
  const gears_type gears;//must be const not const& so we can use constexpr

  ///------------------------------------------------------------------------------------------------------------------
  template<typename... Ts>
  constexpr seq_fusion(Ts&& ...e) : gears{std::forward<Ts>(e)...} {
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Sink, typename SeqAttr>
  constexpr inline auto serialize(bool& pass, Sink& sink, const SeqAttr& seq_attr) const -> void {
    pass = true;
    const auto handler = [&pass, &sink](auto&& engine, auto&& attr) {
                           if(pass) {
                             pass = ::spoon::serialize(sink, engine, attr);
                           }
                         };
    detail::for_each_pair(handler, gears, seq_attr);
  }

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Iterator, typename SeqAttr>
  constexpr inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, SeqAttr& seq_attr) const -> void {
    pass = true;
    const auto handler = [&pass, &start, &end](const auto& engine, auto& attr) {
                           if(pass) {
                             pass = ::spoon::deserialize(start, end, engine, attr);
                           }
                         };
    detail::for_each_pair(handler, gears, seq_attr);
  }
};

}}//spoon::engine


namespace spoon {

  ///------------------------------------------------------------------------------------------------------------------
  template<typename Attr, typename... Engines>
  constexpr inline auto seq(const Engines&... engines) noexcept {
    return engine::seq_fusion<Attr, Engines...>(engines...);
  }

}// spoon

#endif /* SRC_SPOON_ENGINE_SEQ_HPP_ */
