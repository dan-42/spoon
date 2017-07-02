/*
 * poc.cpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */

#include <boost/spirit/include/qi.hpp>

//#include <mapbox/variant.hpp>
#include <spoon/detail/for_each.hpp>
#include <spoon/detail/sfinae_enablers.hpp>


#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>




using binary_data                 = std::vector<uint8_t>;
using binary_data_iterator        = binary_data::iterator;
using binary_data_const_iterator  = binary_data::const_iterator;





namespace spoon {

namespace traits {

//  struct is_unary

template<typename Engine, typename Enable = void>
 struct is_sequence : std::false_type {  };
}

namespace engine {


  struct uint32 {
    using serialize_attribute_type   = uint32_t;
    using deserialize_attribute_type = uint32_t;

    static constexpr inline auto serialize(auto& sink, auto&& attr, auto& ctx) noexcept ->  bool {
  //    std::cout << "engine::uint32::attr " << attr << std::endl;
      constexpr auto byte_size = 4;
      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attr);
      bytes += byte_size - 1;
        for(std::size_t i = 0; i < byte_size; ++i ) {
          //if(!detail::serialize_to(sink , *bytes--)) {
          //  return false;
          //}
          sink.push_back(*bytes--);
        }
      return true;
    }

    static constexpr inline auto deserialize(auto& start, const auto& end, auto& attr, auto& ctx) noexcept ->  bool {

      return true;
    }


  };


}//types

namespace traits {


template<class T>
   using enable_if_is_adapted_struct_t = typename std::enable_if<
     std::is_same<
       typename boost::fusion::traits::tag_of<T>::type,
       boost::fusion::struct_tag
     >::value
,T>::type;



template<typename Engine, typename Attr, class Enable = void>
struct type_supported
{

};

template<typename Engine, typename Attr >
struct type_supported<Engine, Attr,  typename std::enable_if<     ::spoon::traits::is_sequence<Engine>::value
                                                              && std::is_same<typename boost::fusion::traits::tag_of<Attr>::type,
                                                                                    boost::fusion::struct_tag
                                                                          >::value

                                                             >::type  >

{
  using type = std::true_type;
  static constexpr auto value() -> type {
    return {};
  }
};



constexpr auto is_unary(engine::uint32) noexcept -> bool  {
  return true;
}

constexpr auto get_serializer1(auto unsupported) noexcept -> bool  {
  return false;
}




}//traits



constexpr engine::uint32 uint32{};



namespace traits {



  constexpr inline auto has_serializer(spoon::engine::uint32 ) noexcept -> bool  {
    return true;
  }

  constexpr inline auto get_serializer_attr(auto engine) noexcept {
    using engine_type = decltype(engine);
    using attribute_type = typename engine_type::serialize_attribute_type;
    return (attribute_type{});
  }


  template<>
  struct type_supported<spoon::engine::uint32, uint32_t>
  {
    using type = std::true_type;
    static constexpr auto value() -> type {
      return {};
    }
  };



}//traits

namespace detail {


template<class T, typename Engine, class Enable = void>
struct invoke_if {

  static constexpr inline auto call(auto& sink, auto&& attr, auto& ctx) -> bool {

    using attr_type   = std::decay_t<decltype(attr)>;

    static_assert(spoon::traits::type_supported<Engine, attr_type>::type::value, "ERROR: engine attr missmatch, either register the attr_type to that engine, or use an other engine");
   return false;
  }
};

template<class T, typename Engine>
struct invoke_if<T, Engine, typename std::enable_if<std::is_same<T, std::true_type>::value>::type> {

   static constexpr inline auto call(auto& sink, auto&& attr, auto& ctx) -> bool {
   return Engine::serialize(sink, attr, ctx);
  }
};



}


constexpr inline auto serialize(auto& sink, auto&& attr, auto& ctx, auto engine) noexcept ->  bool {
  using engine_type = decltype(engine);
  using attr_type   = std::decay_t<decltype(attr)>;
  std::cout << "spoon::serialize()" << std::endl;
  using is_supported = typename spoon::traits::type_supported<engine_type, attr_type>::type;
  using invoker      = typename detail::invoke_if<is_supported, engine_type>;

  return invoker::call(sink, attr, ctx);
}



namespace engine {

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


} //detail


struct is_sequence_type {};


template<typename Attr, typename... Engines>
struct seq {
  using is_sequence = is_sequence_type;
  using seq_engine_type = std::tuple<Engines...>;
/*
  constexpr seq(Engines... engines) {

  }
*/
  using serialize_attribute_type   = Attr;
  using deserialize_attribute_type = Attr;

  static constexpr inline auto serialize(auto& sink, auto&& seq_attr, auto& ctx) noexcept ->  bool {

    static_assert(boost::fusion::size<Attr>(seq_attr) == engines_size, "ERROR seq_ size does not match serializer size");

    bool pass          = true;
    const auto handler = [&pass, &sink, &ctx](auto engine, auto attr) {
                           if(pass) {
                             pass = ::spoon::serialize(sink, attr, ctx, engine);



                      //       using engine_type = std::decay_t<decltype(engine)>;
                  //           std::cout << " step attr " << attr << std::endl;
                        //     pass = engine_type::serialize(sink, attr, ctx);
                           }
                         };

    detail::for_each_pair<engines_size>(seq_engine_type{}, seq_attr, handler);

    return pass;
  }

  static constexpr inline auto deserialize(auto& start, const auto& end, auto& seq_attr, auto& ctx) noexcept ->  bool {

    return true;
  }

  static const constexpr std::size_t engines_size =  sizeof...(Engines);
  //static const seq_engine_type seq_engine_;

 };


}//engines


template<typename Attr, typename... Engines, std::size_t EnginesSize =  sizeof...(Engines) >
constexpr inline auto seq(Engines... /*engines*/) noexcept -> engine::seq<Attr, Engines...> {
  return engine::seq<Attr, Engines...>{};
}


namespace traits {

template<typename Seq>
struct is_sequence<Seq, typename std::enable_if<std::is_same<typename Seq::is_sequence, spoon::engine::is_sequence_type>::value>::type>
: public std::true_type {  };



}


}






struct ctx {
};


struct foo {
  uint32_t a;
  uint32_t b;
  uint32_t c;
};

BOOST_FUSION_ADAPT_STRUCT(foo, a, b, c);

struct my_seq {
  uint32_t a;
  uint32_t b;
  //foo b;
};

BOOST_FUSION_ADAPT_STRUCT(my_seq, a, b);

int main(int argc, char **argv) {

  binary_data data;

  //int32_t var = 1337;
  my_seq var{1337, 42};

  ctx c;


  constexpr auto sequence = spoon::seq<my_seq>(spoon::uint32, spoon::uint32);
  //spoon::uint32
  auto success = spoon::serialize(data, var, c, sequence );

  std::cout << "serilized "  << success << "   "<< spoon::traits::has_serializer(spoon::uint32)  << std::endl;

  std::cout << " 0x" ;
  for ( const auto& c : data ) {
    std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
  }
  std::cout << std::endl;




  return 0;
}


