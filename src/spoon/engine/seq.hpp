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

#include  <spoon/engine/base.hpp>
#include  <spoon/traits/is_supported_engine_type.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>

#include <type_traits>
#include <tuple>

namespace spoon { namespace engine {

namespace detail {

  template<int Idx, int Left>
  struct seq_for_each_dispatch final {

    constexpr static inline void step(auto&& engines, auto&& attributes, const auto& function) {

      decltype(auto) engine = std::get<Idx>(engines);
      decltype(auto) attr   = boost::fusion::at_c<Idx>(attributes);
      function(engine, attr);
      seq_for_each_dispatch<Idx+1, Left-1>::step(engines, attributes, function);
    }
  };


  /**
   * empty step
   */
  template<int Idx>
  struct seq_for_each_dispatch<Idx, 0> final {
    constexpr static inline void step(auto&& engines, auto&& attributes, const auto& function) {  }
  };

  template<std::size_t Length>
  constexpr inline void for_each_pair(auto&& engines, auto&& attributes, const auto& function ) {
    seq_for_each_dispatch<0, Length>::step(engines, attributes, function);
  }


} //detail





  //todo add tag dispatching for fusion and std::tuples
  template<typename Attr, typename... Engines>
  struct seq : base {
    using tag_type          = tag::seq;
    using has_context       = std::false_type;
    using attr_type         = Attr;

    using seq_engine_type   = std::tuple<Engines...>;
    using number_of_engines = std::integral_constant<std::size_t, sizeof...(Engines)>;

    /**
     * seq serializer
     */
    static constexpr inline auto serialize(auto& sink, auto&& seq_attr, auto& ctx) -> bool {
      static_assert(boost::fusion::size<attr_type>(seq_attr) == number_of_engines::value, "ERROR seq sizes does not match attr and engine");
      bool pass          = true;
      const auto handler = [&pass, &sink, &ctx](auto&& engine, auto&& attr) {
                             if(pass) {
                               pass = ::spoon::serialize(sink, attr, engine, ctx);
                             }
                           };
      detail::for_each_pair<number_of_engines::value>(seq_engine_type{}, seq_attr, handler);

      return pass;
    }

    /**
     * seq deserializer
     */
    static constexpr inline auto deserialize(auto& start, const auto& end, auto& seq_attr,  auto& ctx) -> bool {
      static_assert(boost::fusion::size<attr_type>(seq_attr) == number_of_engines::value, "ERROR seq sizes does not match attr and engine");
      bool pass          = true;
      const auto handler = [&pass, &start, &end, &ctx](auto&& engine, auto&& attr) {
                                       if(pass) {
                                         pass = ::spoon::deserialize(start, end, attr, engine, ctx);
                                       }
                                     };
      detail::for_each_pair<number_of_engines::value>(seq_engine_type{}, seq_attr, handler);

      return pass;
    }
  };



}}


namespace spoon {

  template<typename Attr, typename... Engines>
  constexpr inline auto seq(Engines... /*engines*/) noexcept -> engine::seq<Attr, Engines...> {
    return engine::seq<Attr, Engines...>{};
  }


}

#endif /* SRC_SPOON_ENGINE_SEQ_HPP_ */
