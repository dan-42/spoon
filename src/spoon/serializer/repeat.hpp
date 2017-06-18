/*
 * seq.hpp
 *
 *  Created on: Jun 10, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_SERIALIZER_REP_HPP_
#define SRC_SPOON_SERIALIZER_REP_HPP_



namespace spoon { namespace serializer {



  template<typename Engine>
  constexpr auto repeat_(Engine engine) {

    const auto result = [ engine ](auto& sink, auto&& container_attr, auto& ctx) -> bool {

                        bool pass = true;
                        for(decltype(auto) attr : container_attr) {
                          pass = engine(sink, attr, ctx);
                          if(!pass) {
                            break;
                          }
                        }

                        return pass;
                    };
    return result;
  }


}}


#endif /* SRC_SPOON_SERIALIZER_REP_HPP_ */
