// Fill out your copyright notice in the Description page of Project Settings.

#include "PrjGameStateBase.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Engine/World.h"
#include "Exit.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "loc.h"
#include "log.h"

auto APrjGameStateBase::EndPlay(const EEndPlayReason::Type reason) -> void
{
  Super::EndPlay(reason);
  maze = nullptr;
}

auto APrjGameStateBase ::getMaze() const -> const Maze &
{
  if (!maze)
  {
    const std::string level = TCHAR_TO_UTF8(*UGameplayStatics::ParseOption(UGameplayStatics::GetGameMode(this)->OptionsString, "level"));
    if (level == "1")
      maze = std::make_unique<Maze>(8);
    else if (level == "2")
      maze = std::make_unique<Maze>(9);
    else if (level == "3")
      maze = std::make_unique<Maze>(10);
    else if (level == "4")
      maze = std::make_unique<Maze>(12);
    else if (level == "5")
      maze = std::make_unique<Maze>(14);
    else if (level == "6")
      maze = std::make_unique<Maze>(16);
    else if (level == "7")
      maze = std::make_unique<Maze>(18);
    else if (level == "8")
      maze = std::make_unique<Maze>(21);
    else if (level == "9")
      maze = std::make_unique<Maze>(24);
    else if (level == "10")
      maze = std::make_unique<Maze>(27);
    else // if (level == "11")
      maze = std::make_unique<Maze>(32);
  }

  return *maze;
}

auto APrjGameStateBase::BeginPlay() -> void
{
  Super::BeginPlay();
  const auto size = getMaze().getSize();

  GetWorld()->SpawnActor<AExit>(AExit::StaticClass(), FVector{size * 200.f - 100.f, size * 200.f - 100.f, 170.f + 50.f}, FRotator{0.f, 0.f, 0.f});
  auto exit = GetWorld()->SpawnActor<ATextRenderActor>(
    ATextRenderActor::StaticClass(), FVector{size * 200.f, size * 200.f - 100.f, 346.f}, FRotator{0.f, 180.f, 0.f});
  exit->GetTextRender()->SetText(LOC("EXIT"));
  exit->GetTextRender()->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
  exit->GetTextRender()->SetWorldSize(50);
}

auto APrjGameStateBase::endGame(EndGameState val) -> void
{
  const std::string level = TCHAR_TO_UTF8(*UGameplayStatics::ParseOption(UGameplayStatics::GetGameMode(this)->OptionsString, "level"));
  switch (val)
  {
  case EndGameState::died: {
    LOG("Died");
    if (level.empty())
      UGameplayStatics::OpenLevel(this, FName("Died"), true, TEXT("level=1"));
    else
      UGameplayStatics::OpenLevel(this, FName("Died"), true, UTF8_TO_TCHAR(("level=" + level).c_str()));
    break;
  }
  case EndGameState::survived: {
    LOG("Survived");
    if (level.empty())
      UGameplayStatics::OpenLevel(this, FName("Survived"), true, TEXT("level=1"));
    else
    {
      const auto iLevel = std::stoi(level) + 1;
      if (iLevel <= 11)
        UGameplayStatics::OpenLevel(this, FName("Survived"), true, UTF8_TO_TCHAR(("level=" + std::to_string(iLevel)).c_str()));
      else
        UGameplayStatics::OpenLevel(this, FName("Outro"), true, UTF8_TO_TCHAR(("level=999")));
    }
    break;
  }
  }
}
