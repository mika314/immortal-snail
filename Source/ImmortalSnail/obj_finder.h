#pragma once
#include <CoreMinimal.h>
#include <UObject/ConstructorHelpers.h>
#include <unordered_map>

template <typename T>
T *objFinder(const TCHAR *assetPath)
{
  static std::unordered_map<const TCHAR *, T *> objs;
  auto it = objs.find(assetPath);
  if (it == std::end(objs))
    it = objs.emplace(assetPath, ConstructorHelpers::FObjectFinder<T>{assetPath}.Object).first;
  return it->second;
}
