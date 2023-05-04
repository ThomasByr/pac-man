
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

bool wait_for_ms(useconds_t usec) {
  using namespace std::chrono;
  static auto start = std::optional<steady_clock::time_point>();
  if (!start) { start = steady_clock::now(); }
  auto now = steady_clock::now();
  auto diff = duration_cast<microseconds>(now - *start);
  if (diff.count() >= usec) {
    start.reset();
    return true;
  }
  return false;
}

} // namespace sys_pause