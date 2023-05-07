
#ifndef __inc_utils_H__
#define __inc_utils_H__

#include <chrono>
#include <string>

#define __VERSION__ "1.6.9"
#define __AUTHORS__ "ThomasByr & Arnaud"

#define RST "\x1b[m\x1b[0m"

#define FG_RED "\x1b[0;31m"
#define FG_GRN "\x1b[0;32m"
#define FG_YEL "\x1b[0;33m"
#define FG_BLU "\x1b[0;34m"
#define FG_MAG "\x1b[0;35m"
#define FG_CYN "\x1b[0;36m"
#define FG_WHT "\x1b[0;37m"

#define BG_BLU "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GRN "\x1b[42m"
#define BG_ORA "\x1b[43m"
#define BG_CYN1 "\x1b[44m"
#define BG_YEL "\x1b[45m"
#define BG_CYN2 "\x1b[46m"
#define BG_WHT "\x1b[47m"

/**
 * @brief Format String related functions
 *
 */
namespace fmt {

/**
 * @brief format string
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 * @return std::string
 */
template <typename... Args>
std::string format(const std::string &fmt, Args &&...args);

/**
 * @brief print debug message only if DEBUG is defined on std::cerr
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 */
template <typename... Args> void debug(const std::string &fmt, Args &&...args);

/**
 * @brief print info message on std::cerr
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 */
template <typename... Args> void info(const std::string &fmt, Args &&...args);

/**
 * @brief print warning message on std::cerr
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 */
template <typename... Args>
void warning(const std::string &fmt, Args &&...args);

/**
 * @brief print error message on std::cerr
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 */
template <typename... Args> void error(const std::string &fmt, Args &&...args);

/**
 * @brief print error message on std::cerr and abort
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 */
template <typename... Args>
[[noreturn]] void panic(const std::string &fmt, Args &&...args);

/**
 * @brief print error message on std::cerr and abort
 *
 * @tparam Args template arguments
 * @param fmt   format string
 * @param args  format arguments
 */
template <typename... Args>
[[noreturn]] void unreachable(const std::string &fmt, Args &&...args);

} // namespace fmt

/**
 * @brief System pause related functions
 *
 */
namespace sys_pause {

/**
 * @brief pause the program until the user press any key
 *
 */
void wait_for_key(void);

/**
 * @brief wait for the user to press any key and return true
 * @note this function is non-blocking and meant to be used in a loop
 *
 */
bool wait_for_next_keypress(void);

class Timer {

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
  bool running;

public:
  ///@brief new timer with
  Timer(void);
  ~Timer(void) = default;

  ///@brief if the timer is running
  bool is_running(void);

  ///@brief start the timer with a given duration in seconds
  void start_timer(unsigned sec);
  ///@brief check for intermidiate step
  ///(i.e. if the timer has passed a given duration)
  bool step_passed(unsigned sec);
  ///@brief check for timer expiration
  bool is_expired(void);
  ///@brief reset the timer
  void reset_timer(void);
  ///@brief add a given duration to the timer
  void add_time(unsigned sec);
};

} // namespace sys_pause

#include "utils.hpp"

#endif // __inc_utils_H__
