#include <cstdint>

#ifndef COMMON_HPP_
#define COMMON_HPP_

constexpr std::uint32_t TICKS_PER_SEC{1000U};

void busyWaiting(std::uint32_t delay_ms);

#endif