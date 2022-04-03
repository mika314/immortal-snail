// Fill out your copyright notice in the Description page of Project Settings.

#include "PrjGameStateBase.h"

auto APrjGameStateBase::EndPlay(const EEndPlayReason::Type reason) -> void
{
  Super::EndPlay(reason);
  maze = nullptr;
}

auto APrjGameStateBase ::getMaze() const -> const Maze &
{
  if (!maze)
  {
    maze = std::make_unique<Maze>(32);
  }

  return *maze;
}
