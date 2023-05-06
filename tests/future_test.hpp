
#ifndef __tests_future_test_H__
#define __tests_future_test_H__

#include <chrono>
#include <future>
#include <thread>
#include <vector>

#include "m.h"
#include "utils.h"

namespace future_test {

namespace detail {

int task(unsigned id) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);
  return id;
}

void future_test_0(void) {
  using namespace std::chrono;

  sys_pause::Timer timer{};
  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  timer.start_timer(1);
  while (!timer.is_expired()) { std::this_thread::sleep_for(100ms); }

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(t2 - t1).count();
  assert_leq(std::abs(duration - 1000), 100);
}

} // namespace detail

void future_test(void) {
  using namespace detail;
  test_case(future_test_0);
}

} // namespace future_test

#endif // __tests_future_test_H__
