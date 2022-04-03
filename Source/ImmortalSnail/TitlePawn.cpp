// Copyright (c) 2022 Mika Pi

#include "TitlePawn.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "log.h"

// Sets default values
ATitlePawn::ATitlePawn()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATitlePawn::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void ATitlePawn::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATitlePawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAction("Start", IE_Pressed, this, &ATitlePawn::onStart);
}

auto ATitlePawn::onStart() -> void
{
  LOG("Start");
  const std::string level = TCHAR_TO_UTF8(*UGameplayStatics::ParseOption(UGameplayStatics::GetGameMode(this)->OptionsString, "level"));
  if (level.empty())
  {
    LOG("Loading first level");
    UGameplayStatics::OpenLevel(this, FName("Intro"), true, TEXT("level=1"));
  }
  else
  {
    LOG("Loading", level);
    UGameplayStatics::OpenLevel(this, FName("GamePlay"), true, UTF8_TO_TCHAR(("level=" + level).c_str()));
  }
}
