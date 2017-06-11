/*
 * any.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_ANY_HPP_
#define SRC_SPOON_SERIALIZER_ANY_HPP_

#include <spoon/detail/for_each.hpp>
#include <spoon/serializer/get.hpp>
#include <spoon/serializer/binary.hpp>



namespace spoon { namespace serializer {


  template<typename... Ss>
  const auto any_(Ss... ss) {

    constexpr auto result = [ ss... ](auto& sink, auto&& attr, auto& ctx) -> bool {

                            bool pass           = false;
                            const auto any_pass = [&pass, &sink, &attr, &ctx] (auto engine) -> void {
                                                  if(!pass) {
                                                    pass = engine(sink, attr, ctx);
                                                  }
                                                };

                            ::spoon::detail::for_each_args(any_pass, ss...);
                            return pass;
                          };
    return result;
  }



  template<typename Pass, typename Serializer, typename Sink, typename Ctx>
  struct any_visitor {

    any_visitor(Pass& pass,Sink& sink, Ctx& ctx) : pass_(pass), sink_(sink), ctx_(ctx) {}


    template<typename Attr, bool Same = std::is_same<Attr, Serializer>::value >
    void operator()(Attr attr) const {
      std::cerr << "[INFO] any_visitor specialisation "
                << "     Attr: " << boost::typeindex::type_id< Attr >().pretty_name()
                << " and Serializer: " << boost::typeindex::type_id< Serializer >().pretty_name()
                << " and Same: " << (Same ?  "true" : "false")
                <<'\n' ;
      if(Same && !pass_ ) {


        decltype(auto) engine = ::spoon::get_serializer<Serializer, Ctx> :: call();

        pass_ = engine(sink_, attr, ctx_);

      }
    }

    Pass&   pass_;
    Sink&   sink_;
    Ctx&    ctx_;

  };

  template<typename... Ss>
    auto any_type_(Ss... ss) {

      auto result = [ ss... ](auto& sink, auto&& attr, auto& ctx) -> bool {

          bool pass        = false;

          const auto each_serializer_type = [&pass, &sink, &attr, &ctx] (auto serializer_v) {

            using serializer_type  = typename std::decay<decltype(serializer_v)> ::type;
            using ctx_type         = typename std::decay<decltype(ctx)>  ::type;
            using sink_type        = typename std::decay<decltype(sink)>  ::type;

            const any_visitor<bool, serializer_type, sink_type, ctx_type> any_visitor_(pass, sink, ctx);

            mapbox::util::apply_visitor(any_visitor_, attr);
          };

          ::spoon::detail::for_each_args(each_serializer_type, ss...);


          return pass;
      };
      return result;
    }



}} /* namespaces */





#endif /* SRC_SPOON_SERIALIZER_ANY_HPP_ */
