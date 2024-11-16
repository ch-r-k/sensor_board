#include <cstdint>

#ifndef COMMON_HPP_
#define COMMON_HPP_

constexpr std::uint32_t ticksPerSec{1000U};

void busyWaiting(std::uint32_t delay_ms);

#endif