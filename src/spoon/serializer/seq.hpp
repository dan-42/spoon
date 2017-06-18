/*
 * seq.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_SEQ_HPP_
#define SRC_SPOON_SERIALIZER_SEQ_HPP_


#include <boost/fusion/support/config.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>


#include <tuple>
#include <type_traits>



namespace spoon { namespace serializer {


namespace detail {

  template<int Idx, int Left>
  struct seq_for_each_dispatch final {

    constexpr static inline void step(const auto& engines, const auto& attributes, const auto& function) {

      decltype(auto) engine = std::get<Idx>(engines);
      decltype(auto) attr   = boost::fusion::at_c<Idx>(attributes);
      function(engine, attr);
      seq_for_each_dispatch<Idx+1, Left-1>::step(engines, attributes, function);
    }
  };


  template<int Idx>
  struct seq_for_each_dispatch<Idx, 0> final {
    constexpr static inline void step(const auto& engines, const auto& attributes, const auto& function) {  }
  };

  template<std::size_t Length>
  constexpr inline void for_each_pair(const auto& engines, const auto& attributes, const auto& function ) {
    seq_for_each_dispatch<0, Length>::step(engines, attributes, function);
  }


} /*detail */



  template<typename... Ss, std::size_t Size =  sizeof...(Ss)>
  constexpr auto seq_(Ss... ss) {

    using seq_engine_type = std::tuple<Ss...>;

    const auto result = [ ss... ](auto& sink, auto&& seq_attr, auto& ctx) -> bool {

                        using seq_attr_type = std::decay_t<decltype(seq_attr)>;
                        bool pass           = true;
                        seq_engine_type       seq_engine(ss...);

                        static_assert(boost::fusion::size<seq_attr_type>(seq_attr) == Size, "ERROR seq_ size does not match serializer size");

                        const auto handler = [&pass, &sink, &ctx](auto engine, auto attr) {
                                               if(pass) {
                                                 pass = engine(sink, attr, ctx);
                                               }
                                             };

                        detail::for_each_pair<Size>(seq_engine, seq_attr, handler);

                        return pass;
                    };
    return result;
  }


}}


#endif /* SRC_SPOON_SERIALIZER_SEQ_HPP_ */
