/*
 * seq_.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_SEQ__HPP_
#define SRC_SPOON_DESERIALIZER_SEQ__HPP_





#include <boost/fusion/support/config.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>


#include <tuple>
#include <type_traits>

  /**
   * const auto my_seq = seq_(word16_, word_32,....)
   *
   * my_seq(sink, t, ctx)
   */




//  namespace detail {


/*
  //! Tests if T is a specialization of Template
  // thanks to internet
  template <typename T, template <typename...> class Template>
  struct is_specialization_of : std::false_type {};

  template <template <typename...> class Template, typename... Args>
  struct is_specialization_of<Template<Args...>, Template> : std::true_type {};


  template<class T>
  using enable_if_is_variant_t = typename std::enable_if<
        mapbox::traits::is_mapbox_variant<T>::value
  ,T>::type;

  template<class T>
  using enable_if_is_tuple_t = typename std::enable_if<
      is_specialization_of<T, std::tuple>::value , T>::type;

  template<class T>
  using enable_if_is_primitve_t = typename std::enable_if<
       std::is_integral<T>::value || std::is_floating_point<T>::value ,T>::type;


  template <std::size_t N, typename Attr, enable_if_is_primitve_t<Attr>* = nullptr>
    constexpr auto invoke_seq(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
      if(pass) {
        std::cout << "invoke_seq primitive" << std::endl;
        pass = std::get<N>(fs) (sink, attr, ctx);
      }
    }

  template <std::size_t N, typename Attr, enable_if_is_tuple_t<Attr>* = nullptr>
  constexpr auto invoke_seq(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
    if(pass) {
      std::cout << "invoke_seq tuple" << std::endl;
      pass = std::get<N>(fs) (sink, std::get<N>(attr), ctx);
    }
  }




  template<std::size_t N, typename Attr, enable_if_is_primitve_t<Attr>* = nullptr>
  constexpr auto invoke_any(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
    if(!pass) {
      std::cout << "invoke_any primitive" << std::endl;
      pass = std::get<N>(fs) (sink, attr, ctx);
    }
  }



  template<std::size_t N, typename Attr, enable_if_is_tuple_t<Attr>* = nullptr>
  constexpr auto invoke_any(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
    if(!pass) {
      std::cout << "invoke_any tuple" << std::endl;
      pass = std::get<N>(fs) (sink, std::get<N>(attr), ctx);
    }
  }

  template<std::size_t N, typename Attr, enable_if_is_variant_t<Attr>* = nullptr>
    constexpr auto invoke_any(bool& pass, auto fs, auto& sink, Attr attr, auto& ctx) -> void {
      if(!pass) {
        std::cout << "invoke_any variant" << std::endl;


        attr.match([&](auto a){
          pass = std::get<N>(fs) (sink, a, ctx);
        } );



       // pass = std::get<N>(fs) (sink, std::get<N>(attr), ctx);
      }
    }




  template <class F, size_t... Is>
  constexpr auto index_apply_impl(F f, std::index_sequence<Is...>) {
      return f(std::integral_constant<size_t, Is> {}...);
  }

  template <size_t N, class F>
  constexpr auto index_apply(F f) {
      return index_apply_impl(f, std::make_index_sequence<N>{});
  }

  template <class Ss>
  constexpr auto apply_seq(Ss ss, auto& sink, auto as, auto& ctx) {
    std::cout << "apply_seq " << std::endl;
      bool pass = true;
      return index_apply<std::tuple_size<Ss>{}>(
                          [&](auto... Is) {
                            std::cout << "apply_seq result" << std::endl;
                              int dummy[sizeof...(Is)] = { (detail::invoke_seq<Is>(pass, ss, sink, as, ctx), 0)... };
                              return pass;
                          });
  }

  template <class Ss>
  constexpr auto apply_any(Ss ss, auto& sink, auto as, auto& ctx) {
    std::cout << "apply_any " << std::endl;
      bool pass = false;
      return index_apply<std::tuple_size<Ss>{}>(
                          [&](auto... Is) {
                              std::cout << "apply_any result" << std::endl;
                              int dummy[sizeof...(Is)] = { (detail::invoke_any<Is>(pass, ss, sink, as, ctx), 0)... };
                              return pass;
                          });
  }


  } / * detail */

namespace spoon { namespace deserializer {

namespace detail {

  template<int Idx, int Left>
  struct seq_for_each_dispatch final {

    constexpr static inline void step(const auto& engines, auto& attributes, const auto& function) {

      decltype(auto) engine = std::get<Idx>(engines);
      decltype(auto) attr   = boost::fusion::at_c<Idx>(attributes);
      function(engine, attr);
      seq_for_each_dispatch<Idx+1, Left-1>::step(engines, attributes, function);
    }
  };


  template<int Idx>
  struct seq_for_each_dispatch<Idx, 0> final {
    constexpr static inline void step(const auto& engines, auto& attributes, const auto& function) {  }
  };

  template<std::size_t Length>
  constexpr inline void for_each_pair(const auto& engines, auto& attributes, const auto& function ) {
    seq_for_each_dispatch<0, Length>::step(engines, attributes, function);
  }


} /*detail */



  template<typename... Ss, std::size_t Size =  sizeof...(Ss)>
  constexpr auto seq_(Ss... ss) {

    using seq_engine_type = std::tuple<Ss...>;

    const auto result = [ ss... ](auto& start, const auto& end, auto& seq_attr, auto& ctx) -> bool {

                            using seq_attr_type = std::decay_t<decltype(seq_attr)>;
                            bool pass           = true;
                            seq_engine_type       seq_engine(ss...);

                            static_assert(boost::fusion::size<seq_attr_type>(seq_attr) == Size, "ERROR seq_ size does not match serializer size");

                            const auto handler = [&pass, &start, &end,  &ctx](auto engine, auto& attr) {
                                                     if(pass) {
                                                       pass = engine(start, end, attr, ctx);
                                                     }
                                                   };

                            detail::for_each_pair<Size>(seq_engine, seq_attr, handler);

                            return pass;
                        };
    return result;
  }




} /*namespace serializer*/
} /*namespace spoon*/




#endif /* SRC_SPOON_DESERIALIZER_SEQ__HPP_ */
