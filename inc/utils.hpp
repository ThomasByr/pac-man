
#ifndef __fmt_utils_HPP__
#define __fmt_utils_HPP__

#include <iostream>

#include "utils.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace fmt {

namespace internal {
/**
 * Convert all std::strings to const char* using constexpr if (C++17)
 */
template <typename T> auto convert(T &&t) {
  if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>,
                             std::string>::value) {
    return std::forward<T>(t).c_str();
  } else {
    return std::forward<T>(t);
  }
}

/**
 * printf like formatting for C++ with std::string
 * Original source: https://stackoverflow.com/a/26221725/11722
 */
template <typename... Args>
std::string string_format(const std::string &format, Args &&...args) {
  const auto size =
      snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...) + 1;
  if (size <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}

} // namespace internal

template <typename... Args>
std::string format(const std::string &fmt, Args &&...args) {
  using namespace internal;
  return string_format(fmt, convert(std::forward<Args>(args))...);
}

template <typename... Args> void debug(const std::string &fmt, Args &&...args) {
  using namespace std;
  using namespace internal;
#ifdef DEBUG
  cerr << FG_CYN << "   debug " << RST << format(fmt, args...) << endl;
#else
  (void)fmt;
#endif
}

template <typename... Args> void info(const std::string &fmt, Args &&...args) {
  using namespace std;
  using namespace internal;
  cerr << FG_GRN << "    info " << RST << format(fmt, args...) << endl;
}

template <typename... Args> void alert(const std::string &fmt, Args &&...args) {
  using namespace std;
  using namespace internal;
  cerr << FG_RED << "   alert " << RST << format(fmt, args...) << endl;
}

} // namespace fmt

#endif // __fmt_utils_HPP__
