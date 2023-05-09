
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <optional>

#include "utils.h"

/// **Note**
/// this file uses a lot of C functions
/// this is because C++ equivalent always do weird things
/// plus we do a lot of low level stuff here

namespace sys_pause {

namespace internal {

int _kbhit() {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  switch (fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK)) {
  case -1: fmt::panic("could not set stdin to non-blocking");
  default: break;
  }

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  switch (fcntl(STDIN_FILENO, F_SETFL, oldf)) {
  case -1: fmt::panic("could not set stdin back to blocking");
  default: break;
  }

  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}

void _after() {
  getchar();       // consume the key
  printf("\b \b"); // go back, print space, go back :)
}

} // namespace internal

void wait_for_key() {
  using namespace internal;
  fmt::info("Press any key to continue...");
  while (!_kbhit()) { usleep(100); }
  _after();
  fmt::debug("Key pressed");
}

bool wait_for_next_keypress() {
  using namespace internal;
  switch (_kbhit()) {
  case 1: _after(); return true;
  case 0: return false;
  }
  fmt::unreachable("invalid return value from _kbhit()");
}

Timer::Timer() : running(false) {}

bool Timer::is_running() { return running; }

void Timer::start_timer(unsigned sec) {
  if (running) { fmt::panic("timer already running"); }
  start = std::chrono::high_resolution_clock::now();
  end = start + std::chrono::seconds(sec);
  running = true;
}

bool Timer::step_passed(unsigned sec) {
  if (!running) { return false; }
  auto now = std::chrono::high_resolution_clock::now();
  return now >= start + std::chrono::seconds(sec);
}

bool Timer::is_expired() {
  if (!running) { return false; }
  return std::chrono::high_resolution_clock::now() >= end;
}

void Timer::reset_timer() { running = false; }

void Timer::add_time(unsigned sec) {
  if (!running) { return; }
  end += std::chrono::seconds(sec);
}

} // namespace sys_pause
