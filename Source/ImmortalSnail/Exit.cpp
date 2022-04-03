// Copyright (c) 2022 Mika Pi

#include "Exit.h"
#include "Engine/World.h"
#include "PrjGameStateBase.h"
#include "log.h"

auto AExit::BeginPlay() -> void
{
  Super::BeginPlay();
  OnActorBeginOverlap.AddDynamic(this, &AExit::onBeginOverlap);
}

auto AExit::onBeginOverlap(AActor *me, AActor *other) -> void
{
  LOG("BeginOverlap");
  auto gameState = GetWorld()->GetGameState<APrjGameStateBase>();
  if (!gameState)
  {
    LOG_ERR("GameSate is not APrjGameStateBase");
    return;
  }
  gameState->endGame(EndGameState::survived);
}
