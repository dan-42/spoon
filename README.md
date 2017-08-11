# spoon
a c++14 binary serializer, deserializer header only library, for custom binary formats




## TODO
 - [x] spoon::serialize()
 - [x] spoon::deserialize()
 
 - [x] spoon::big::bool8
 - [x] spoon::big::uint8
 - [x] spoon::big::uint16
 - [x] spoon::big::uint32
 - [x] spoon::big::uint64
 
 - [x] spoon::repeat[ENGINE](count)
 - [x] spoon::repeat[ENGINE](min, max)
 - [x] spoon::repeat[ENGINE]([]() -> size_t { return 3;})
 - [] spoon::repeat_until[ENGINE]([](const auto& count) -> bool { /*true when done repeating*/})
 
 - [x] spoon::optional[ENGINE]
 - [x] spoon::optional[ENGINE]([]() -> bool { /*true if expect to be parsed generated */}) 
 
 - [] spoon::any[ENGINE1, ENGINE2, ENGINE3] //left to right first wins
 - [] spoon::any[ENGINE1  ENGINE2, ENGINE3]([](const auto& attr ) -> bool) //"asks" if ENGINE1 for ENGINE1::attribute_type shall be used
 - [] spoon::any_by_index[ENGINE1, ENGINE2, ENGINE3]([]() -> index_type { return index to use })
 - [] spoon::any_of[ENGINE1, ENGINE2, ENGINE3]([]() -> engine { return engine to be used })  //do look up by user and return own type 
 
 - [] spoon::seq(ENGINE1, ENGINE2,..)
 - [] ENGINE1 << ENGINE2 << ENGINE3...
 
 - [] spoon::eps[] //???
 
 - [] spoon::engine::bits<buffer_type, attr_type, bits, offset> // can extract 4bit value into enum
 - [] spoon::flag<buffer_type, offset>// extract single  bit to bool
 - [] spoon::adv //go to next byte
 - [] spoon::adv(count) go to count next byte
 
 - [] examples
  - [] bacnet_npdu
  - [] simple examples
  - [] benchmarks
  
 - []  documentation
 - [] add static_asserts to spoon::serialize deserialzie
 
 
 
 spoon::serialize(sink,   spoon::serializers::repeat[spoon::serializers::big_endian::uint32], list)
 spoon::deserialize(sink, spoon::deserializers::repeat[spoon::deserializers::big_endian::uint32], list)