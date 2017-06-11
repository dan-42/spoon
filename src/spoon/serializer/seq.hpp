/*
 * seq.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_SEQ_HPP_
#define SRC_SPOON_SERIALIZER_SEQ_HPP_


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


}}} /* namespaces */






#endif /* SRC_SPOON_SERIALIZER_SEQ_HPP_ */
