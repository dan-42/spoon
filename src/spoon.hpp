/*
 * spoon.hpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */

#ifndef SRC_LIB_SPOON_HPP_
#define SRC_LIB_SPOON_HPP_


#include <utility>
#include <cstdint>

#include <spoon/unused_type.hpp>

#include <spoon/serializer/get.hpp>
#include <spoon/deserializer/get.hpp>

#include <spoon/serializer/binary.hpp>
#include <spoon/deserializer/binary.hpp>


namespace spoon {



  auto serialize_with(const auto& engine, auto& sink, auto&& t, auto& ctx) -> bool {
    return engine(sink, t, ctx);
  }

  auto serialize_with(const auto& engine, auto& sink, auto&& t) -> bool  {
   unused_type unused{};
   return serialize_with(engine, sink, t, unused);
  }


  /**
   * Main interface for users
   */
  auto serialize(auto& sink, auto&& t, auto& ctx) -> bool {
    using t_type   = typename std::decay<decltype(t)>   ::type;
    using ctx_type = typename std::decay<decltype(ctx)> ::type;
    const auto& serializer = get_serializer<t_type, ctx_type>::call();
    return serializer(sink, t, ctx);
  }

  auto serialize(auto& sink, auto&& t) -> bool {
   unused_type unused{};
   return serialize(sink, t, unused);
  }




//-------------------------------------------------------------------------------------------------




  auto deserialize_with(const auto& engine, auto& start, const auto& end, auto& t, auto& ctx) -> bool{
    return engine(start, end, t, ctx);
  }

  auto deserialize_with(const auto& engine, auto& start, const auto& end, auto& t) -> bool{
    unused_type unused{};
    return deserialize_with(engine, start, end, t, unused);
  }



  auto deserialize(auto& start, const auto& end, auto& t, auto& ctx) -> bool {
    using t_type   = typename std::decay<decltype(t)>   ::type;
    using ctx_type = typename std::decay<decltype(ctx)> ::type;
    const auto& deserializer = get_deserializer<t_type, ctx_type>::call();
    return deserializer(start, end, t, ctx);
  }

  auto deserialize(auto& start, const auto& end, auto& t) -> bool {
    const unused_type unused{};
   return deserialize(start, end, t, unused);
  }



}/*namespace spoon*/

#include <spoon/serializer/any.hpp>
#include <spoon/serializer/seq.hpp>
#include <spoon/serializer/repeat.hpp>

#include <spoon/deserializer/any.hpp>
#include <spoon/deserializer/seq_.hpp>
#include <spoon/deserializer/repeat.hpp>

#endif /* SRC_LIB_SPOON_HPP_ */
