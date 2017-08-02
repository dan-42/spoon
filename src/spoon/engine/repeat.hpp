/*
 * repeat.hpp
 *
 *  Created on: Jul 4, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_ENGINE_REPEAT_HPP_
#define SRC_SPOON_ENGINE_REPEAT_HPP_


namespace spoon { namespace traits {

template<typename Engine, typename Ctx>
auto repeate_done_check(Engine, Ctx& ctx, size_t count) -> bool {
  return false;
}



template<typename Engine, typename Ctx>
struct done{
    static auto call(Ctx& ctx, size_t count) -> bool {
      return false;
    }
};


}}

namespace spoon { namespace engine {

namespace detail {


} //detail


  template<typename Attr, std::size_t Min, std::size_t Max, typename Engine, typename Handler>
  struct repeat : base {
    using tag_type          = tag::repeat;
    using has_context       = std::false_type;
    using attr_type         = Attr;
    using engine_type       = Engine;

    using element_type      = typename Attr::value_type;
    using min_type          = std::integral_constant<std::size_t, Min>;
    using max_type          = std::integral_constant<std::size_t, Max>;

    using this_type         = repeat<Attr, Min, Max, Engine, Handler>;
    static_assert(min_type::value <= max_type::value, "ERROR spoon engine repeat, min must be <= max");

    /**
     * any serializer
     */
    static inline auto serialize(auto& sink, auto&& container_attr, auto& ctx) -> bool {

      bool pass = false;
      auto min = min_type::value;
      auto max = max_type::value;

      // both 0 means as much as we have in the container
      if(min == 0 && max == 0) {
        max = container_attr.size();
      }
      // cannot meet requirements fail!
      else if(container_attr.size() < min_type::value) {
        return false;
      }
      else {
        max = (container_attr.size() < max_type::value) ? container_attr.size() :  max_type::value;
      }

      size_t count = 0;
      for( auto attr : container_attr ) {

        if(spoon::traits::repeate_done_check(this_type{}, ctx, count) ) {
           break;
        }


        if(spoon::traits::done<this_type, decltype(ctx)>::call(ctx, count) ) {
           break;
        }

        pass = engine_type::serialize(sink, std::move(attr), ctx);
        if(pass) {
          count++;
          if(count >= max) {
            return true;
          }
        }
        else {
          if(count >= min) {
            return true;
          }
        }
      }
      return false;
    }

    /**
     * any deserializer
     */
    static constexpr inline auto deserialize(auto& start, const auto& end, auto& container_attr, auto& ctx) -> bool {

      bool pass = true;
      size_t count = 0;
      auto min = min_type::value;
      auto max = max_type::value;

      while(true) {
        element_type attr{};
        pass = engine_type::deserialize(start, end, attr, ctx);
        if(!pass) {
          break;
        }
        //todo customization point
        container_attr.push_back(attr);
        count++;


      /*  if(!instance.handler(count, ctx)) {
          return false;
        }
      */

        if(max != 0 && count >= max ) {
          break;
        }
      }

      if(count < min) {
        return false;
      }
      return true;
    }

  };



}}


namespace spoon {



  //without handler
  template<typename Attr, std::size_t Min, std::size_t Max, typename UniqueType, typename Engine>
  constexpr inline auto repeat(Engine ) noexcept {
    return engine::repeat<Attr, Min, Max, Engine, UniqueType>{};
  }

  template<typename Attr, std::size_t Min, std::size_t Max, typename Engine>
  constexpr inline auto repeat(Engine ) noexcept {
    return engine::repeat<Attr, Min, Max, Engine, nullptr_t>{};
  }


  template<typename Attr, std::size_t Count, typename Engine>
  constexpr inline auto repeat(Engine) noexcept  {
    return engine::repeat<Attr, Count, Count, Engine, nullptr_t>{};
  }

  template<typename Attr, typename Engine>
  constexpr inline auto repeat(Engine) noexcept{
    return engine::repeat<Attr, 0, 0, Engine, nullptr_t>{};
  }
}



#endif /* SRC_SPOON_ENGINE_REPEAT_HPP_ */
