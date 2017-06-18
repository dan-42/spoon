/*
 * seq_.hpp
 *
 *  Created on: Jun 11, 2017
 *      Author: dan
 */

#ifndef SRC_SPOON_DESERIALIZER_REP__HPP_
#define SRC_SPOON_DESERIALIZER_REP__HPP_

#include <type_traits>

namespace spoon { namespace deserializer {

  template<typename Engine>
  constexpr auto repeat_(Engine engine) {

    const auto result = [ engine ](auto& start, const auto& end,  auto& container_attr, auto& ctx) -> bool {

                        using container_type = typename std::decay_t<decltype(container_attr)>;
                        using element_type   = typename container_type::value_type;

                        bool pass = true;
                        while(true && start != end) {

                          element_type attr{};

                          pass = engine(start, end, attr, ctx);
                          if(!pass) {
                            break;
                          }
                          else {
                            container_attr.push_back(std::move(attr));
                          }
                        }

                        return true;
                    };
    return result;
  }




} /*namespace serializer*/
} /*namespace spoon*/




#endif /* SRC_SPOON_DESERIALIZER_REP__HPP_ */
