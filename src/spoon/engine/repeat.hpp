/*
 * repeat.hpp
 *
 *  Created on: Jul 4, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_ENGINE_REPEAT_HPP_
#define SRC_SPOON_ENGINE_REPEAT_HPP_

#include <spoon/traits/has_call_operator.hpp>

#include <cstdint>
#include <utility>
#include <type_traits>
#include <limits>
#include <cassert>

namespace spoon { namespace engine {

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

      /**
       * handling infinitive, min and max
       */
      template<typename Sink, typename Attr>
      inline auto serialize(bool& pass, Sink& sink, Attr&& attr)  const -> void {
        using element_type = typename std::decay_t<Attr>::value_type;
        const auto expected_count = as_count_provider()();
        size_type count = 0;
        for(decltype(auto) a : attr) {
          if(pass == false || count >= expected_count) {
            return;
          }
          as_gear().serialize(pass, sink, std::forward<element_type>(a));
          ++count;
        }
        if(count != expected_count) {
          pass = false;
        }
      }

      /** deserialize
       */
      template<typename Iterator, typename Attr>
      inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, Attr& attr)  const -> void {
        using element_type = typename std::decay_t<Attr>::value_type;
        const auto expected_count = as_count_provider()();
        element_type element{};
        size_type count = 0;

        while(count < expected_count && pass != false) {
          as_gear().deserialize(pass, start, end, element);
          if(pass == true) {
            ++count;
            attr.push_back(element);
          }
          else {
            pass = true;
            break;
          }
        }
        if(count < expected_count) {
          pass = false;
        }
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
      constexpr repeat(FwdGear&& fwd_gear, size_type fwd_min, size_type fwd_max) : Gear(std::forward<FwdGear>(fwd_gear)), min{fwd_min}, max{fwd_max}  {
        assert(min <= max);
      }


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


      /**
       * handling infinitive, min and max
       */
      template<typename Sink, typename Attr>
      inline auto serialize(bool& pass, Sink& sink, Attr&& attr)  const -> void {
        size_type count = 0;
        for(decltype(auto) a : attr) {
          //done when last call failed or we have reached max
          if(pass == false || count >= max) {
            return;
          }
          as_gear().serialize(pass, sink, std::forward<typename std::decay_t<Attr>::value_type>(a));
          ++count;
        }
        if(count < min) {
          pass = false;
        }
      }

      /** deserialize
       */
      template<typename Iterator, typename Attr>
      inline auto deserialize(bool& pass, Iterator& start, const Iterator& end, Attr& attr)  const -> void {
        using element_type = typename std::decay_t<Attr>::value_type;
        element_type element{};
        size_type count = 0;

        while(count < max && pass != false) {
          as_gear().deserialize(pass, start, end, element);
          if(pass == true) {
            ++count;
            attr.push_back(element);
          }
          else {
            pass = true;
            break;
          }
        }
        if(count < min) {
          pass = false;
        }
      }

    };
  }}}// spoon::engine::detail

namespace spoon { namespace engine {

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

}} //spoon::engine




namespace spoon {

constexpr auto repeat = engine::repeat{};
constexpr auto repeat_until = engine::repeat_until{};

} // spoon


#endif /* SRC_SPOON_ENGINE_REPEAT_HPP_ */
