// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "maze.h"
#include <memory>

#include "PrjGameStateBase.generated.h"

enum class EndGameState { died, survived };

UCLASS()
class IMMORTALSNAIL_API APrjGameStateBase : public AGameStateBase
{
  GENERATED_BODY()
public:
  auto getMaze() const -> const Maze &;
  auto endGame(EndGameState) -> void;

private:
  auto BeginPlay() -> void override;
  auto EndPlay(const EEndPlayReason::Type) -> void override;

  mutable std::unique_ptr<Maze> maze;
};
