/*
 * any.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_ANY_HPP_
#define SRC_SPOON_DESERIALIZER_ANY_HPP_


#include <spoon/detail/for_each.hpp>

#include "external/mapbox/variant.hpp"

#include <utility>
#include <cstdint>


namespace spoon { namespace deserializer {


  template<typename Pass, typename Engine, typename Start, typename End, typename Ctx>
  struct variant_visitor {
    variant_visitor(Pass& pass, const Engine& engine, Start& start, const End& end, Ctx& ctx) :
                                    pass_(pass), engine_(engine), start_(start), end_(end), ctx_(ctx){}


    template<typename Attr>
    void operator()(Attr& attr) const     {

      std::cerr << "[INFO] deserializer variant_visitor "
                << "  Attr: " << boost::typeindex::type_id< Attr >().pretty_name()
                << "  Ctx: " << boost::typeindex::type_id< Ctx >().pretty_name()
                << "  Engine" << boost::typeindex::type_id< Engine >().pretty_name()
                << '\n' ;


      auto& itr = start_;
      pass_ = engine_(itr, end_, attr, ctx_);
      if(pass_) {
        start_ = itr;
      }
    }

    Pass& pass_;
    const Engine& engine_;
    Start& start_;
    const End& end_;
    Ctx& ctx_;

  };


  template<typename... Ss>
  constexpr auto any_(Ss... ss) {

    const auto result = [ ss... ](auto& start, const auto& end, auto& variant_attr, auto& ctx)-> bool {

                        bool pass = false;
                        size_t cnt = 0;
                        ::spoon::detail::for_each_args([&cnt, &pass, &start, &end, &variant_attr, &ctx ](const auto& deserializer) {
                                                      std::cout << " any cnt " << cnt << std::endl;
                                                      cnt++;
                                                        if(!pass && start != end) {

                                                          pass = deserializer(start, end, variant_attr, ctx);


                                                        }
                                                    }, ss...);

                        return pass;
                     };
    return result;
  }




}}

#endif /* SRC_SPOON_DESERIALIZER_ANY_HPP_ */
