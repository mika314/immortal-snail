// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ImmortalSnailGameMode.h"
#include "ImmortalSnailCharacter.h"
#include "ImmortalSnailHUD.h"
#include "PrjGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

AImmortalSnailGameMode::AImmortalSnailGameMode() : Super()
{
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
  DefaultPawnClass = PlayerPawnClassFinder.Class;

  // use our custom HUD class
  HUDClass = AImmortalSnailHUD::StaticClass();

  GameStateClass = APrjGameStateBase::StaticClass();
}
