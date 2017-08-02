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


/*******************************************************************/
struct npdu_v1_addres {

  using network_nbr_type  = uint16_t;
  using addr_type         = std::vector<uint8_t>;

  network_nbr_type    network_nbr;
  addr_type           addr;
};

BOOST_FUSION_ADAPT_STRUCT(npdu_v1_addres,
                          network_nbr,
                          addr)

/*******************************************************************/
namespace npdu_message_type {
  using base_type = uint8_t;

  constexpr base_type std_max_value     = 0x7F;
  constexpr base_type prop_min_value    = 0x80;
  constexpr base_type prop_max_value    = 0xFF;

  constexpr base_type who_is_router_to_network     = 0x00;
  constexpr base_type i_amm_router_to_network      = 0x01;
  constexpr base_type i_could_be_router_to_network = 0x02;
  /*
   *.....
   *
   */
}

/*******************************************************************/
struct npdu_v1_frame {

  enum class network_priority_type : uint8_t {
    normale    = 0,
    urgent     = 1,
    critical   = 2,
    life_safty = 3,
  };
  using expecting_reply_type = bool;
  using addres_type          = boost::optional<npdu_v1_addres>;
  using hop_count_type       = boost::optional<uint8_t>;
  using message_type_type    = boost::optional<npdu_message_type::base_type>;
  using vendor_id_type       = boost::optional<uint16_t>;
  using apdu_type            = std::vector<uint8_t>;

//---------------------------------------------------
  expecting_reply_type  expecting_reply;

  network_priority_type network_priority;

  addres_type           destination;
  addres_type           source;

  hop_count_type        hop_count;

  message_type_type     message_type;

  vendor_id_type        vendor_id;

  apdu_type             apdu;
};


BOOST_FUSION_ADAPT_STRUCT(npdu_v1_frame,
                          expecting_reply,
                          network_priority,
                          destination,
                          source,
                          hop_count,
                          message_type,
                          vendor_id,
                          apdu);


/*******************************************************************/
namespace engine {


/**
 * npdu_v1_frame format
 *
 * 1 byte  version
 * 1 byte control info
 *  bit 7 flag
 *  bit 6 == 0
 *  bit 5 flag
 *  bit 4 == 0
 *  bit 3 flag
 *  bit 2 flag
 *  bit 0,1 == uint2_t
 */

using npdu_network_priority       = spoon::engine::bits<::npdu_v1_frame::network_priority_type, uint8_t, 2, 0>;



constexpr auto npdu_address = spoon::repeat<std::vector<uint8_t>>(spoon::uint8);

struct npdu_v1_frame_ctx {
  bool has_dest{false};
  bool has_src{false};
  bool has_hop_count{false};
  bool has_message_type{false};
  bool has_vendor_id{false};
};


} //engine



namespace spoon { namespace traits {

using npdu_address_engine = std::decay_t<decltype(::engine::npdu_address)>;

template<>
auto repeate_done_check<npdu_address_engine, ::engine::npdu_v1_frame_ctx>(npdu_address_engine, ::engine::npdu_v1_frame_ctx& ctx, size_t count ) -> bool {
  return true;
}


template<>
struct done<npdu_address_engine, ::engine::npdu_v1_frame_ctx>{
    static auto call(::engine::npdu_v1_frame_ctx& ctx, size_t count) -> bool {
      return true;
    }
};

}}




namespace engine {


static constexpr auto npdu_v1_addres_engine = spoon::seq<npdu_v1_addres>(::spoon::uint16, ::engine::npdu_address);


static constexpr auto npdu_expecting_reply        = spoon::bits_bool<uint8_t, 2>();
static constexpr auto npdu_has_src_addr           = spoon::bits_bool<uint8_t, 3>();
static constexpr auto npdu_has_dst_addr           = spoon::bits_bool<uint8_t, 5>();
static constexpr auto npdu_has_npdu_message_type  = spoon::bits_bool<uint8_t, 7>();

static constexpr ::engine::npdu_network_priority network_priority{};
static constexpr auto address_engine  = spoon::optional<::npdu_v1_frame::addres_type>(npdu_v1_addres_engine);
static constexpr auto hop_count       = spoon::optional<::npdu_v1_frame::hop_count_type>(spoon::uint8);
static constexpr auto message_type    = spoon::optional<::npdu_v1_frame::message_type_type>(spoon::uint8);
static constexpr auto vendor_id       = spoon::optional<::npdu_v1_frame::vendor_id_type>(spoon::uint16);
static constexpr auto apdu            = spoon::repeat<::npdu_v1_frame::apdu_type>(spoon::uint8);


///
///



} //engine

namespace spoon { namespace traits {

using hop_count_engine_type = decltype(::engine::hop_count);

template<>
constexpr auto expect_optional<>( hop_count_engine_type engine, ::engine::npdu_v1_frame_ctx& ctx) -> bool {
  return ctx.has_hop_count;
}

}}


namespace engine {

struct npdu_v1_frame_engine : spoon::engine::base {

  using attr_type = npdu_v1_frame;





  static constexpr auto seq_engine = spoon::seq<attr_type>(engine::npdu_expecting_reply,
                                                          network_priority,
                                                          address_engine,
                                                          address_engine,
                                                          hop_count,
                                                          message_type,
                                                          vendor_id,
                                                          apdu);



  static constexpr auto npdu_has_src_addr           = spoon::engine::bits<bool, uint8_t, 1, 3>{};
  static constexpr auto npdu_has_dst_addr           = spoon::engine::bits<bool, uint8_t, 1, 5>{};
  static constexpr auto npdu_has_npdu_message_type  = spoon::engine::bits<bool, uint8_t, 1, 7>{};

  static constexpr inline auto serialize(auto& sink, auto&& attr, auto& ctx) -> bool {

    npdu_v1_frame_ctx c;
    c.has_dest          = (attr.destination)  ? true : false;
    c.has_src           = (attr.source)       ? true : false;
    c.has_hop_count     = (attr.hop_count)    ? true : false;
    c.has_message_type  = (attr.message_type) ? true : false;
    c.has_vendor_id     = (attr.vendor_id)    ? true : false;


    uint8_t npdu_version{0x01};
    uint8_t control_info{0x00};
    spoon::serialize(sink, npdu_version, spoon::uint8);
    spoon::serialize(sink, control_info, spoon::uint8);


    spoon::serialize(sink, c.has_dest , npdu_has_dst_addr);
    spoon::serialize(sink, c.has_src, npdu_has_src_addr);

    spoon::serialize(sink, c.has_message_type, npdu_has_npdu_message_type);

    spoon::serialize(sink, attr, seq_engine, ctx);








    /**
     *  * prepare ctx
     *  * call serialze
     */

    return true;
  }

  static constexpr inline auto deserialize(auto& start, const auto& end, auto& attr,  auto& ctx) -> bool {
    auto itr = start;
    attr_type value{};


    if (!spoon::engine::detail::deserialize_to_attr(value, attr)) { return false; }
    start = itr;
    return true;
  }



};

}




int main(int argc, char **argv) {

  constexpr engine::npdu_v1_frame_engine engine{};

  npdu_v1_frame frame{};
  npdu_v1_addres dst;
  dst.network_nbr = 0x1111;
  dst.addr = npdu_v1_addres::addr_type{0x23, 0x23, 0x23};

  frame.message_type = 42;
  frame.destination = dst;

  binary_data data{};

  spoon::serialize(data, frame, engine);

  std::cout << " 0x" ;
  for ( const auto& c : data ) {
    std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
  }
  std::cout << std::endl;

  return 0;
}


