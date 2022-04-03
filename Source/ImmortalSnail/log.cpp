// Copyright (c) 2022 Mika Pi

#include "log.h"

#include <CoreMinimal.h>
#include <fstream>

namespace Internal
{
  auto dumpLog(const char *value) -> void
  {
    //    std::ofstream f("D:\\marooned.log", std::ios_base::app);
    //    f << value << std::endl;
    UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(value));
  }

  auto dumpLogErr(const char *value) -> void
  {
    UE_LOG(LogTemp, Error, TEXT("%s"), ANSI_TO_TCHAR(value));
  }
} // namespace Internal

std::ostream &operator<<(std::ostream &strm, const FString &val)
{
  strm << TCHAR_TO_UTF8(*val);
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const FVector &val)
{
  strm << "(" << val.X << ", " << val.Y << ", " << val.Z << ")";
  return strm;
}
