
#include <spoon.hpp>
#include <spoon/binary.hpp>

#include <random>
#include <array>
#include <limits>
#include <vector>
#include <chrono>

int main()
{
  std::mt19937 gen(42); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<uint32_t> dis(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
  constexpr size_t n = 1000000;
  std::array<uint32_t, n> data;
  std::vector<uint8_t> binary_data(n*sizeof(uint32_t));

  for (size_t idx = 0; idx < n; ++idx)
  {
    data[idx] = dis(gen);
  }


  const auto start = std::chrono::high_resolution_clock::now();
  for (size_t idx = 0; idx < n; ++idx)
  {
    spoon::serialize(binary_data, spoon::big_endian::uint32, data[idx]);
  }
  const auto end = std::chrono::high_resolution_clock::now();


  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
