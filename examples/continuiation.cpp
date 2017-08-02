
#include  <nlohmann/json.hpp>

#include <utility>
#include <type_traits>
#include <cstdint>
#include <cassert>
#include <array>
#include <tuple>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)



struct my_sink_type {

  template<typename T>
  auto push_back(const T&) {
    ++size_;
  }

  auto size() const -> size_t {
    return size_;
  }

  size_t size_{0};
};

//using sink_type = std::vector<uint8_t>;
using sink_type = my_sink_type;

sink_type sink{};



struct serializer_tag_type{};
struct deserializer_tag_type{};

constexpr serializer_tag_type   serializer_tag{};
constexpr deserializer_tag_type deserializer_tag{};

struct node_base {};
struct node_root : node_base {
  static constexpr auto index = 0;
};


template<typename T>
struct advance_index {
  static constexpr auto value = 1;
};


template<>
struct advance_index<node_root> {
  static constexpr auto value = 0;
};


template<typename Function, typename Pass, typename Sink, typename Attr, size_t Idx, typename = std::enable_if_t<!std::is_integral<Attr>::value>>
auto invoke_if_tuple_attr( Function& f, Pass& p, Sink& s, const Attr& a) -> void {
  f(serializer_tag_type{}, p, s, std::get<Idx>(a));
}


template <typename Parent, typename Engine>
struct node : Parent, Engine
{
  static constexpr auto index = Parent::index + advance_index<Engine>::value;
  using this_type = node<Parent, Engine>;


  // forwarding constructor
  template <typename ParentFwd, typename EngineFwd>
  node(ParentFwd&& p, EngineFwd&& f) : Parent{FWD(p)}, Engine{FWD(f)}   {   }

  // helper
  auto& as_engine() noexcept { return static_cast<Engine&>(*this); }
  auto& as_parent() noexcept { return static_cast<Parent&>(*this); }

  //entry points
  auto operator()(serializer_tag_type, bool& pass, auto& sink, auto&& attr) -> void {
    call(serializer_tag, pass, sink, FWD(attr));
  }

  auto operator()(deserializer_tag_type, bool& pass, auto& begin, const auto& end, auto& attr) -> void {
    call(deserializer_tag, pass, begin, end, attr);
  }

  // create new type from a const& type
  template <typename NextEngine>
  auto next_in_sequence(const NextEngine& ) {

    auto next = [](bool& pass, auto& sink, auto attr) mutable {
      NextEngine next_engine{};
      next_engine(pass, sink, attr);
    };

    using next_type = decltype(next);
    return ::node<this_type, next_type>{std::move(*this), std::move(next)};
  }

  //create new type and move engine on into it
  template <typename NextEngine>
  auto next_in_sequence(NextEngine&& next_engine)
  {
    auto next = [next_engine = FWD(next_engine)](bool& pass, auto& sink, auto attr) mutable {
      next_engine(pass, sink, attr);
    };

    using next_type = decltype(next);
    return ::node<this_type, next_type>{std::move(*this), std::move(next)};
  }



   //----------------------------------------------------------------
    auto call_without_parent(std::true_type, serializer_tag_type, bool& pass, auto& sink, auto&& attr) -> void {

      invoke_if_tuple_attr<Engine, decltype(pass), decltype(sink), decltype(attr), index>(as_engine(), pass, sink, attr);
    }

    auto call_without_parent(std::false_type, serializer_tag_type, bool& pass, auto& sink, auto&& attr) -> void {
      as_parent().call(serializer_tag, pass, sink, attr);
      invoke_if_tuple_attr<Engine, decltype(pass), decltype(sink), decltype(attr), index>(as_engine(), pass, sink, attr);
    }

    auto call(serializer_tag_type, bool& pass, auto& sink, auto&& attr) -> void {
      using is_parent_root = typename std::is_same<Parent, node_root>::type;
      call_without_parent(is_parent_root{}, serializer_tag, pass, sink, FWD(attr));
    }


    //----------------------------------------------------------------
    auto call(deserializer_tag_type, bool& pass, auto& start, const auto& end, auto& attr) -> void
    {
      using is_parent_root = typename std::is_same<Parent, node_root>::type;
      //return call_without_parent(is_parent_root{}, pass, sink, FWD(attr));
    }
};



template <typename Engine>
auto initiate(const Engine& )
{
    return node<node_root, Engine>{node_root{}, std::forward<Engine>(Engine{})};
}

template <typename Engine>
auto initiate(Engine&& f)
{
    return node<node_root, Engine>{node_root{}, std::forward<Engine>(f)};
}

template<typename EngineParent, typename EngineChild, typename = std::enable_if_t<std::is_base_of<node_base, std::decay_t<EngineParent>>::value>>
auto operator<<(EngineParent&& p, EngineChild&& child) -> decltype(auto)
{
   return std::move(p.next_in_sequence(std::move(child)));
}

template<typename EngineParent, typename EngineChild, typename = std::enable_if_t<std::is_base_of<node_base, EngineParent>::value>>
auto operator<<(EngineParent&& p, const EngineChild& ) -> decltype(auto)
{
   return std::move(p.next_in_sequence(EngineChild{}));
}





template<typename Type, typename Lambda>
struct with_lambda : Type, Lambda {

  template<typename TypeFwd, typename LambdaFwd>
  with_lambda(TypeFwd&& t, LambdaFwd&& l) : Type{FWD(t)}, Lambda{FWD(l)}
  {}

  auto& as_t() noexcept { return static_cast<Type&>(*this);   }
  auto& as_l() noexcept { return static_cast<Lambda&>(*this); }


  auto operator()(auto domain, bool b, sink_type& sink, auto attr) -> void {
  //  assert(false);
    as_l()(sink);
    as_t()(domain, b, sink, attr);
  }

};



template<typename RepeatEngine, typename InnerEngine>
struct repeat_with : RepeatEngine, InnerEngine {

  template<typename RepeatEngineFwd, typename InnerEngineFwd>
  repeat_with(RepeatEngineFwd&& t, InnerEngineFwd&& l) : RepeatEngine{FWD(t)}, InnerEngine{FWD(l)}
  {}

  auto& as_repeat_engine() noexcept { return static_cast<RepeatEngine&>(*this);   }
  auto& as_inner_engine() noexcept { return static_cast<InnerEngine&>(*this); }


  auto operator()(serializer_tag_type domain, bool b, sink_type& sink, auto attr) -> void {
  //  assert(false);
    as_repeat_engine()(domain, b, sink, attr, as_inner_engine());
  }

};


struct empty_t{};
empty_t empty{};

template<typename Ctx = empty_t>
struct engine_int {

  constexpr engine_int() : ctx{empty} {
  };


  constexpr engine_int(Ctx& c) : ctx{c} {
  };

//  template<typename L, typename = std::enable_if_t<!std::is_base_of<node_base, L>::value>>
//  auto operator()(L&& l) const {
//     return with_lambda<engine_int, L>{engine_int{}, std::move(l)};
//  }

  auto operator()(serializer_tag_type,   bool b, sink_type& sink, auto attr) -> void {
    sink.push_back(0x00);
  }

  auto operator()(deserializer_tag_type, bool b, sink_type& sink, auto attr) -> void {
  //  sink++;
  }


  Ctx& ctx;
};

template<typename Ctx = empty_t>
struct engine_repeat {

  constexpr engine_repeat() : ctx{empty} {
  };


  constexpr engine_repeat(Ctx& c) : ctx{c} {
  };

  template<typename InnerEngine>
  auto operator[](const InnerEngine& inner_engine) const {
     return repeat_with<engine_repeat, InnerEngine>{engine_repeat{}, InnerEngine{}};
  }

  template<typename InnerEngine>
  auto operator[](InnerEngine&& inner_engine) const {
     return repeat_with<engine_repeat, InnerEngine>{engine_repeat{}, std::move(inner_engine)};
  }


  auto operator()(serializer_tag_type,   bool b, sink_type& sink, auto attr, auto inner_engine) -> void {
     for(decltype(auto) a : attr) {
       inner_engine(serializer_tag_type{}, b, sink, a);
     }
   }

   auto operator()(deserializer_tag_type, bool b, sink_type& sink, auto attr, auto inner_engine) -> void {

   }

  Ctx& ctx;
};




constexpr auto int_ = engine_int<empty_t>{};
constexpr auto repeat_ = engine_repeat<empty_t>{};




auto main() -> int {

  std::tuple<int, std::array<int, 10>
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
//  ,int
  > tuple_attr{};

  std::get<0>(tuple_attr) = 1337;
  std::get<1>(tuple_attr) = std::array<int, 10>{1,2,3,4,5,6,7,8,9,0};


  bool b = true;
  int attr{42};



//  auto tt = initiate(int_( [](auto& sink){sink += 10000; }  ))
//       //   .then(make_int([](auto& sink){ sink += 100;}   ))
//          .then(make_int())
//          .then(make_int())
//          .then(make_int())
//          .then(make_int())
//          ;


   auto t = initiate(int_) << repeat_[int_];
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_//([](auto& sink){ std::cout << "foo" << sink << std::endl; sink += 25;})
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_
//       << int_;

  //static_assert(std::is_base_of<node_base, std::decay_t<decltype(t)> >::value);

  // auto tt = t.then([](do_serialize,   bool b, sink_type& sink, auto attr){});
   //auto ttt = t << [](do_serialize, bool b, sink_type& sink, auto attr){};

  t(serializer_tag, b, sink, tuple_attr);
 // tt(b, sink, attr);


  //std::cout << "result " << sink.size() << std::endl;
  return sink.size();

}
