/**
 * Copyright (C) 2017 by dan (Daniel Friedrich)
 *
 * This file is part of project spoon
 * a c++14 (de)serialization library for (binary) protocols
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <mapbox/variant.hpp>
#include <mapbox/variant_io.hpp>
#include <spoon.hpp>

#include <boost/fusion/adapted.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>


using binary_data                 = std::vector<uint8_t>;
using binary_data_iterator        = binary_data::iterator;
using binary_data_const_iterator  = binary_data::const_iterator;


struct foo {
  uint32_t a;
  uint32_t b;
  uint32_t c;
};

BOOST_FUSION_ADAPT_STRUCT(foo, a, b, c);

struct my_seq {
  uint32_t d;
  uint32_t e;
  foo f;
};

BOOST_FUSION_ADAPT_STRUCT(my_seq, d, e, f);

auto do_any() -> void {
  std::cout << "\n------------------ANY-------------------------" << std::endl;
  using my_any_t = mapbox::util::variant<uint32_t, double>;
  constexpr auto any_engine = spoon::any<my_any_t>(spoon::float64, spoon::uint32);
  binary_data data;

  {
     my_any_t var;
     //var = double{-3.145};
     var = uint32_t{3145};
     auto success = spoon::serialize(data, var, any_engine );

     std::cout << " 0x" ;
     for ( const auto& c : data ) {
       std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
     }
     std::cout << std::endl;
  }

  {
    auto start = data.begin();
   auto end   = data.end();
   my_any_t var;
   auto success = spoon::deserialize(start, end, var, any_engine);
   std::cout << std::dec << "deserialize " << var << std::endl;
  }
}


auto do_seq() -> void {
  std::cout << "\n------------------SEQ-------------------------" << std::endl;
  binary_data data;
  constexpr auto foo_sequence = spoon::seq<foo>(spoon::uint32, spoon::uint32, spoon::uint32);
  constexpr auto my_sequence = spoon::seq<my_seq>(spoon::uint32, spoon::uint32, foo_sequence);
  {
    //uint32_t var = 1337;
    foo f{1337, 42, 23};
    my_seq var{1984, 314, f};


    auto success = spoon::serialize(data, var, my_sequence );
   // auto success = spoon::serialize(data, var, spoon::uint32 );

    std::cout << " 0x" ;
    for ( const auto& c : data ) {
      std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
    }
    std::cout << std::endl;
  }

  {
    auto start = data.begin();
    auto end   = data.end();
    my_seq var;
    auto success = spoon::deserialize(start, end, var, my_sequence);
    std::cout << std::dec << "deserialize " << var.d << " " << var.e << " " << var.f.a << " " << var.f.b << " " << var.f.c << std::endl;
  }

}


auto do_optional()  -> void {
  std::cout << "\n------------------OPTIONAL-------------------------" << std::endl;

    using optional_double = boost::optional<double>;
    constexpr auto engine = spoon::optional<optional_double>(spoon::float64);
    {
      binary_data data;
      {
        optional_double var{};
        auto success = spoon::serialize(data, var, engine );
        std::cout << " 0x" ;
        for ( const auto& c : data ) {
          std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
        }
        std::cout << std::endl;
      }

      {
        auto start = data.begin();
        auto end   = data.end();
        optional_double var;
        auto success = spoon::deserialize(start, end, var, engine);
        std::cout << std::dec << "deserialize " << var << std::endl;
      }
    }

    {
      binary_data data;
      {
        optional_double var;
        var = 3.1472;
        auto success = spoon::serialize(data, var, engine );
        std::cout << " 0x" ;
        for ( const auto& c : data ) {
          std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
        }
        std::cout << std::endl;
      }

      {
        auto start = data.begin();
        auto end   = data.end();
        optional_double var;
        auto success = spoon::deserialize(start, end, var, engine);
        std::cout << std::dec << "deserialize " << var.get() << std::endl;
      }
    }

}


using vector_double = std::vector<double>;
struct one;
struct two;

constexpr auto vector_double_engine_1 = spoon::repeat<vector_double, 1,10, one>(spoon::float64);
constexpr auto vector_double_engine_2 = spoon::repeat<vector_double, 1,10, two>(spoon::float64);

using vector_double_engine_1_type = std::decay_t<decltype(vector_double_engine_1)>;
using vector_double_engine_2_type = std::decay_t<decltype(vector_double_engine_2)>;

//static_assert(std::is_same<vector_double_engine_1_type, vector_double_engine_2_type>::value, "FUCK" );

struct my_done_check {

  static int count;

   auto foo() -> bool {
     std::cout << "Hello World: my_done_check count:" << count << std::endl;
     return false;
   }

   auto inc(auto c) -> void {
     count++;
   }

};

int my_done_check::count = 0;

namespace spoon { namespace traits {
  template<>
  auto repeate_done_check<>(const vector_double_engine_1_type& engine, my_done_check& ctx, size_t count) -> bool {
    ctx.inc(count);
    return false;
  }
}}//spoon::traits

auto do_repeat()  -> void {
  std::cout << "\n------------------REPEATE-------------------------" << std::endl;

    {
      binary_data data;
      my_done_check ctx;
      {
        vector_double var{};
        var.push_back(3.23);
        var.push_back(1.23);
        var.push_back(34241.23);
        var.push_back(-323);
        auto success = spoon::serialize(data, var, vector_double_engine_1 , ctx);
        if(success) {
          std::cout << "success" << std::endl;
          ctx.foo();
        }
        else {
          std::cout << "FAILED" << std::endl;
        }
        std::cout << " 0x" ;
        for ( const auto& c : data ) {
          std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
        }

        std::cout << std::endl;
      }
      {
        auto start = data.begin();
        auto end   = data.end();
        vector_double var;
        auto success = spoon::deserialize(start, end, var, vector_double_engine_1, ctx);
        for(auto v : var)
        {
          std::cout << std::dec << "deserialize " << v << std::endl;
        }

      }
    }
}

/*



struct complex_t {
  std::vector<uint32_t> a;
  std::vector<uint32_t> b;
};

BOOST_FUSION_ADAPT_STRUCT(complex_t, a, b );



auto do_complex()  -> void {
  std::cout << "\n------------------COMPLEX-------------------------" << std::endl;


    constexpr auto engine_a = spoon::repeat<decltype(complex_t::a), 0, 0>(spoon::uint32, [](const auto& count, auto& ctx) -> bool {
                                                                                          std::cout << "engine_a count " << count << std::endl;
                                                                                          if(count >= 4) return false;
                                                                                          else return true;
                                                                                  });

    constexpr auto engine_b = spoon::repeat<decltype(complex_t::a), 0,0 >(spoon::uint32, [](const auto& count, auto& ctx) -> bool {
                                                                                          std::cout << "engine_b count " << count << std::endl;
                                                                                          return true;
                                                                                  });

    constexpr auto engine = spoon::seq<complex_t>(engine_a, engine_b);
    {
      binary_data data;
      {
        complex_t var{};
        var.a.push_back(3.23);
        var.a.push_back(1.23);
        var.a.push_back(34241.23);
        var.a.push_back(-323);
        var.b.push_back(3.23);
        var.b.push_back(1.23);
        var.b.push_back(34241.23);
        var.b.push_back(-323);

        auto success = spoon::serialize(data, var, engine );
        if(success) {
          std::cout << "success" << std::endl;
        }
        else {
          std::cout << "FAILED" << std::endl;
        }
        std::cout << " 0x" ;
        for ( const auto& c : data ) {
          std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
        }
        std::cout << std::endl;
      }
      {
        auto start = data.begin();
        auto end   = data.end();
        complex_t var;
        auto success = spoon::deserialize(start, end, var, engine);
        for(auto v : var.a)
        {
          std::cout << std::dec << "deserialize a " << v << std::endl;
        }
        for(auto v : var.b)
        {
          std::cout << std::dec << "deserialize b " << v << std::endl;
        }

      }
    }
}
*/

int main(int argc, char **argv) {

 do_seq();
 do_any();
 do_optional();
 do_repeat();
 //do_complex();


  return 0;
}


