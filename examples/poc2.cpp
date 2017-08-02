/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//include <mapbox/variant.hpp>
//#include <mapbox/variant_io.hpp>

#include  <spoon/traits/has_call_operator.hpp>
#include  <spoon/engine/binary.hpp>

//#include <boost/fusion/adapted.hpp>
//#include <boost/optional.hpp>
//#include <boost/optional/optional_io.hpp>



#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <limits>


namespace spoon {



  template<typename Sink, typename Engine, typename... Attr>
  auto serialize(Sink& sink, const Engine& engine, Attr&& ...attr) -> bool {
    bool pass = true;
    engine(pass, sink, std::forward<Attr...>(attr...));
    return pass;
  }

  template<typename Sink, typename Engine>
  auto serialize(Sink& sink, const Engine& engine)  -> bool {
    bool pass = true;
    engine(pass, sink);
    return pass;
  }
  ///////////////////////

namespace engine {

  namespace detail {


    template<typename Gear, typename CountProvider>
    struct repeat_with_count_provider  : Gear , CountProvider {
      using size_type = std::size_t;


      template<typename FwdGear, typename FwdCountProvider>
      constexpr repeat_with_count_provider(FwdGear&& fwd_gear, FwdCountProvider&& fwd_count_provider)
      : Gear{std::forward<FwdGear>(fwd_gear)}
      , CountProvider{std::forward<FwdCountProvider>(fwd_count_provider)} {}

      auto& as_gear() const noexcept { return static_cast< const Gear&>(*this); }
      auto& as_count_provider() const noexcept { return static_cast< const CountProvider&>(*this); }

      ////// TODO
      template<typename Sink, typename Attr>
      inline auto operator()(bool& pass, Sink& sink, Attr&& attr)  const -> void {
       as_count_provider()();
       for(decltype(auto) a : attr) {
         as_gear()(pass, sink, std::forward<typename std::decay_t<Attr>::value_type>(a));
       }
      }

      /** deserialize
      */
      template<typename Iterator, typename Attr>
      inline auto operator()(bool& pass, Iterator& start, const Iterator& end, Attr& attr)  const -> void {

      }

    };




    template<typename Gear>
    struct repeat  : Gear {
      using size_type = std::size_t;
      size_type min = std::numeric_limits<size_t>::min();
      size_type max = std::numeric_limits<size_t>::max();

      template<typename FwdGear>
      constexpr repeat(FwdGear&& fwd_gear) : Gear(std::forward<FwdGear>(fwd_gear)) {}

      template<typename FwdGear>
      constexpr repeat(FwdGear&& fwd_gear, size_type count) : Gear(std::forward<FwdGear>(fwd_gear)), min{count}, max{count} {}

      template<typename FwdGear>
      constexpr repeat(FwdGear&& fwd_gear, size_type fwd_min, size_type fwd_max) : Gear(std::forward<FwdGear>(fwd_gear)), min{fwd_min}, max{fwd_max}  {}


      constexpr auto& as_gear() const noexcept { return static_cast< const Gear&>(*this); }

      constexpr inline auto operator()(size_type count)  const {
        return repeat<Gear>{std::move(as_gear()), count};
      }

      constexpr inline auto operator()(size_type min, size_type max)  const {
        return repeat<Gear>{std::move(as_gear()), min, max};
      }

      template<typename CountProvider, typename = std::enable_if_t<spoon::traits::has_call_operator<CountProvider>::value> >
      constexpr inline auto operator()(CountProvider&& count_provider)  const {
        return repeat_with_count_provider<Gear, CountProvider>{std::move(as_gear()), std::forward<CountProvider>(count_provider)};
      }



////// TODO
      template<typename Sink, typename Attr>
      inline auto operator()(bool& pass, Sink& sink, Attr&& attr)  const -> void {
        for(decltype(auto) a : attr) {
          as_gear()(pass, sink, std::forward<typename std::decay_t<Attr>::value_type>(a));
        }
      }

      /** deserialize
       */
      template<typename Iterator, typename Attr>
      inline auto operator()(bool& pass, Iterator& start, const Iterator& end, Attr& attr)  const -> void {

      }

    };
  }//detail



  struct repeat {
    template<typename FwdGear>
    constexpr auto inline operator[](FwdGear&& fwd_gear) const {
        return detail::repeat<FwdGear>(std::forward<FwdGear>(fwd_gear));
    }
    template<typename FwdGear>
    constexpr auto inline operator[](const FwdGear& fwd_gear) const {
        return detail::repeat<FwdGear>(fwd_gear);
    }
  };

  struct repeat_until {

    constexpr inline repeat_until(){}
    template<typename FwdGear>
    constexpr auto inline operator[](FwdGear&& fwd_gear) const {
        return detail::repeat<FwdGear>(std::forward<FwdGear>(fwd_gear));
    }
    template<typename FwdGear>
    constexpr auto inline operator[](const FwdGear& fwd_gear) const {
        return detail::repeat<FwdGear>(fwd_gear);
    }
  };

}//enggine


constexpr auto repeat = engine::repeat{};
constexpr auto repeat_until = engine::repeat_until{};





} // spoon


struct my_sink_type {

  template<typename T>
  auto push_back(const T& t) {
    data_[size_++] = t;

  }

  auto size() const -> size_t {
    return size_;
  }

  std::array<uint8_t, 255> data_{};
  size_t size_{0};
};



auto main(int argc, char* args[]) -> int  {


  int i =  42;
  auto binary_uint32_always23 = spoon::big_endian::uint32(23);
  auto binary_uint32_always23_ = spoon::big_endian::uint32([&i]() { std::cout << "hello " << i++ << std::endl; return 32; });
  auto t2 = spoon::big_endian::uint32([](){ std::cout << "hello " << std::endl; return 32; });
  constexpr auto t3 = spoon::big_endian::uint32([](){ return 32; });

  my_sink_type sink;
  spoon::serialize(sink, spoon::big_endian::uint32, 23);
  spoon::serialize(sink, binary_uint32_always23);
  spoon::serialize(sink, binary_uint32_always23_);
  spoon::serialize(sink, t2);


 std::vector<uint32_t>  my_ints{1,2,3,4,5,5,6,7,8};







 constexpr auto my_vector_engine0 = spoon::repeat[spoon::big_endian::uint32] ;
 constexpr auto my_vector_engine1 = spoon::repeat[spoon::big_endian::uint32] (1);
 constexpr auto my_vector_engine2 = spoon::repeat[spoon::big_endian::uint32] (1, 5);
 constexpr auto my_vector_engine3 = spoon::repeat[spoon::big_endian::uint32] ([](){ std::cout << "FOOBAR" << std::endl; return 5;});


  //auto my_vector_engine4 = spoon::repeat_until[spoon::big_endian::uint32]([](const auto& count){return true;});



  spoon::serialize(sink, my_vector_engine0, my_ints);
  spoon::serialize(sink, my_vector_engine1, my_ints);
  spoon::serialize(sink, my_vector_engine2, my_ints);
  spoon::serialize(sink, my_vector_engine3, my_ints);
  //spoon::serialize(sink, my_vector_engine4, my_ints);




  return sink.size();





























}
