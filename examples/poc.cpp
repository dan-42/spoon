/*
 * poc.cpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */


#include <spoon.hpp>
#include <mapbox/variant.hpp>

#include <thread>
#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <tuple>
#include <boost/spirit/home/karma.hpp>

using binary_data                 = std::vector<uint8_t>;
using binary_data_iterator        = binary_data::iterator;
using binary_data_const_iterator  = binary_data::const_iterator;





using my_variant_t = mapbox::util::variant<bool, uint32_t>;
using my_tuple_t = std::tuple<my_variant_t, uint16_t, uint32_t, float, double, uint64_t>;

const constexpr auto my_serializer = [](auto&& sink, my_tuple_t data, auto& ctx) -> bool{
      using namespace spoon::serializer;
      using namespace spoon::serializer::binary::big;

      const auto int_any = any_(word8_, word32_);
      const auto int_seq = seq_(int_any, word16_, word32_, float_, double_);
      //
      //const auto seq = seq_(int_seq, int_any);
      //constexpr auto int_rep = repeate();
      //constexpr auto my = seq(int_seq, int_any)

      int_seq(sink, data, ctx);
      return true;
  };

namespace spoon {

    template<>
    struct get_serializer<my_tuple_t> {
      static auto call() -> decltype(my_serializer) {
        return my_serializer;
      }
    };
}

struct my_ctx {

};



int main(int argc, char **argv) {



  my_variant_t my_variant;
  my_variant = uint32_t{232323};
  my_tuple_t my_tuple_value{my_variant, 0, 165000, 0.42f, 3.14, 1};


  binary_data data;
 // auto success = spoon::serializer::apdu_value(sink, value);
  bool success{};

  /**
   * serialized by matching the type to a serializer
   */
  success = spoon::serialize(data, my_tuple_value, my_serializer);
  //success = spoon::serialize(data, std::make_tuple(true, false), my_serializer);
  assert(success);


  /**
   *  serialized by providing a custom serializer
   */
  //success = spoon::serialize(data, my_tuple_value, my_serializer);
  assert(success);

  //success = spoon::serialize(sink, ctx, engine, value1, value2, value3 ...);


  /*
  //sink data that is generated to
  // ctx optional value stingly typed, needs wrapper
  // engine, callable function (auto& sink, auto& ctx, std::tuple<decltype(value1), decltype(value1), decltype(value1), decltype(value1),>)
  // values111  variadic templates


  open:  hot to handle nested types?
      how to track that one value is already is serialized, and die when cardinality is wrong
  */

//  success = spoon::serialize(data, value, my_serializer);
//  success = spoon::serialize(data, value, ctx);

  /*
  success = spoon::serialize(data, value, [](auto&& sink, auto&& data, auto& ctx) -> bool{
    using namespace spoon::serializer;
 //   seq(unsigned_integer, unsigned_integer, unsigned_integer);

    return true;
  }, ctx);
*/
  if(!success) {
    std::cout << "fuck" << std::endl;
    return -1;
  }

  std::cout << "serilized " << std::endl;
  std::cout << " 0x" ;
  for ( const auto& c : data ) {
    std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
  }
  std::cout << std::endl;




  return 0;
}


