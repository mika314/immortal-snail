// Copyright (c) 2021 Mika Pi

#pragma once
#include <Math/Vector.h>
#include <sstream>

std::ostream &operator<<(std::ostream &strm, const FString &);
std::ostream &operator<<(std::ostream &strm, const FVector &);

namespace Internal
{
  void dumpLog(const char *);
  void dumpLogErr(const char *);

  inline auto logToStrm(std::ostream &) -> void {}

  template <typename Head, typename... Tail>
  auto logToStrm(std::ostream &strm, Head &&head, Tail &&...tail) -> void
  {
    strm << head << " ";
    logToStrm(strm, std::forward<Tail>(tail)...);
  }

  template <typename... Args>
  void log(const char *fileName, int line, Args &&...args)
  {
    std::ostringstream strm;
    logToStrm(strm, std::forward<Args>(args)...);
    strm << fileName << ":" << line;
    dumpLog(strm.str().c_str());
  }
  template <typename... Args>
  void logErr(const char *fileName, int line, Args &&...args)
  {
    std::ostringstream strm;
    logToStrm(strm, std::forward<Args>(args)...);
    strm << fileName << ":" << line;
    dumpLogErr(strm.str().c_str());
  }
} // namespace Internal

#define LOG(...) ::Internal::log(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...) ::Internal::logErr(__FILE__, __LINE__, __VA_ARGS__)
