// Copyright 2022 Mika Pi

#include "Snail.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "ImmortalSnailProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PrjGameStateBase.h"
#include "log.h"
#include "obj_finder.h"
#include <array>

ASnail::ASnail() : teleportCue(objFinder<USoundBase>(TEXT("/Game/teleport_Cue"))), slimeSound(CreateDefaultSubobject<UAudioComponent>("slimeSound"))
{
  PrimaryActorTick.bCanEverTick = true;
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(objFinder<UStaticMesh>(TEXT("/Game/SM_Snail")));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->SetCollisionProfileName(TEXT("BlockAll"));

  onHitSounds.push_back(objFinder<USoundBase>(TEXT("/Game/Snail-0001_Cue")));
  onHitSounds.push_back(objFinder<USoundBase>(TEXT("/Game/Snail-0002_Cue")));
  onHitSounds.push_back(objFinder<USoundBase>(TEXT("/Game/Snail-0003_Cue")));
  onHitSounds.push_back(objFinder<USoundBase>(TEXT("/Game/Snail-0004_Cue")));
  slimeSound->SetSound(objFinder<USoundBase>(TEXT("/Game/Slime_Cue")));
  slimeSound->SetupAttachment(RootComponent);
}

static auto aStar(const Maze &maze, int sx, int sy, int px, int py) -> std::vector<std::pair<int, int>>
{
  if (sx == px && sy == py)
    return {};
  const auto size = maze.getSize();
  if (px < 0 || py < 0 || px >= size || py >= size)
    return {};
  auto map = std::vector<int>{};
  for (auto i = 0; i < size * size; ++i)
    map.push_back(-1);
  auto oldWave = std::vector<std::pair<int, int>>{};
  oldWave.emplace_back(sx, sy);
  auto nstep = 0;
  map[sx + sy * size] = nstep;
  bool done = false;
  while (!oldWave.empty())
  {
    ++nstep;
    auto wave = std::vector<std::pair<int, int>>{};
    for (auto i : oldWave)
    {
      static const auto dirs =
        std::array<std::pair<int, int>, 4>{std::make_pair(0, 1), std::make_pair(0, -1), std::make_pair(1, 0), std::make_pair(-1, 0)};
      for (auto d : dirs)
      {
        const auto nx = i.first + d.first;
        const auto ny = i.second + d.second;
        if (!maze.isOpen(i.first, i.second, nx, ny))
          continue;
        if (map[nx + ny * size] != -1)
          continue;
        wave.push_back(std::make_pair(nx, ny));
        map[nx + ny * size] = nstep;
        if (nx == px && ny == py)
        {
          done = true;
          break;
        }
      }
      if (done)
        break;
    }
    oldWave = wave;
    if (done)
      break;
  }

  if (map[px + py * size] == -1)
    return {};

  auto ret = std::vector<std::pair<int, int>>{};
  auto x = px;
  auto y = py;
  while (map[x + y * size] > 1)
  {
    static const auto dirs =
      std::array<std::pair<int, int>, 4>{std::make_pair(0, 1), std::make_pair(0, -1), std::make_pair(1, 0), std::make_pair(-1, 0)};
    for (auto d : dirs)
    {
      const auto nx = x + d.first;
      const auto ny = y + d.second;
      if (nx < 0)
        continue;
      if (ny < 0)
        continue;
      if (nx >= size)
        continue;
      if (ny >= size)
        continue;
      if (map[x + y * size] - 1 != map[nx + ny * size])
        continue;
      x = nx;
      y = ny;
      ret.push_back(std::make_pair(x, y));
      break;
    }
  }

  LOG("Found path", ret.size(), "steps");
  return ret;
}

auto ASnail::Tick(float dt) -> void
{
  Super::Tick(dt);
  SetActorScale3D(FVector{1.f + .15f * sinf(2.f * GetWorld()->GetTimeSeconds()), 1.f, 1.f});

  auto gameState = GetWorld()->GetGameState<APrjGameStateBase>();
  if (!gameState)
  {
    LOG_ERR("GameSate is not APrjGameStateBase");
    return;
  }
  const auto &maze = gameState->getMaze();
  const auto size = maze.getSize();
  const auto sLoc = GetActorLocation();
  const auto player = UGameplayStatics::GetPlayerCharacter(this, 0);
  const auto pLoc = player->GetActorLocation();

  if (rand() % 250 == 0)
  {
    path.clear();
    auto canSeeFunc = [&](FVector loc) {
      FHitResult hitResult;
      FCollisionQueryParams params;
      params.AddIgnoredActor(this);
      if (GetWorld()->LineTraceSingleByChannel(hitResult, loc, player->GetActorLocation(), ECollisionChannel::ECC_Camera, params))
      {
        const auto actor = hitResult.Actor.Get();
        if (actor == player)
          return true;
      }
      return false;
    };
    const auto canSee = canSeeFunc(sLoc);
    LOG("can see", canSee);
    if (!canSee && (sLoc - pLoc).Size() > 400)
    {
      for (auto i = 0; i < 10; ++i)
      {
        auto x = static_cast<int>(pLoc.X / 200.f) + rand() % 7 - 3;
        auto y = static_cast<int>(pLoc.Y / 200.f) + rand() % 7 - 3;
        if (x < 0)
          x = 0;
        if (x >= size)
          x = size - 1;
        if (y < 0)
          y = 0;
        if (y >= size)
          y = size - 1;
        auto newLoc = FVector{x * 200.f + 100.f, y * 200.f + 100.f, sLoc.Z};
        LOG("Try", x, y, "player loc", pLoc.X / 200.f, pLoc.Y / 200.f);
        if (!canSeeFunc(newLoc))
        {
          LOG("teleport", x, y, "player loc", pLoc.X / 200.f, pLoc.Y / 200.f);
          SetActorLocation(newLoc, false, nullptr, ETeleportType::ResetPhysics);
          LOG("check", GetActorLocation());
          UGameplayStatics::PlaySoundAtLocation(this, teleportCue, newLoc);
          return;
        }
      }
    }
  }

  if (path.empty() && rand() % 25 == 0)
  {
    LOG("player",
        static_cast<int>(pLoc.X / 200.f),
        static_cast<int>(pLoc.Y / 200.f),
        "snail",
        static_cast<int>(sLoc.X / 200.f),
        static_cast<int>(sLoc.Y / 200.f));
    path = aStar(
      maze, static_cast<int>(sLoc.X / 200.f), static_cast<int>(sLoc.Y / 200.f), static_cast<int>(pLoc.X / 200.f), static_cast<int>(pLoc.Y / 200.f));
  }

  if (!path.empty())
  {
    const auto goal = FVector{path.back().first * 200.f + 100.f, path.back().second * 200.f + 100.f, sLoc.Z};
    const auto v = goal - sLoc;
    if (v.Size() < 25)
      path.pop_back();
    SetActorLocation(sLoc + v.GetSafeNormal(1) * 80.f * dt);
    auto angle = FGenericPlatformMath::Atan2(v.Y, v.X);
    SetActorRotation(FRotator{0, angle * 180.f / 3.1415926f, 0});
  }
  else
  {
    const auto goal = FVector{pLoc.X, pLoc.Y, sLoc.Z};
    const auto v = goal - sLoc;
    if (v.Size() < 300)
    {
      SetActorLocation(sLoc + v.GetSafeNormal(1) * 40.f * dt);
      auto angle = FGenericPlatformMath::Atan2(v.Y, v.X);
      SetActorRotation(FRotator{0, angle * 180.f / 3.1415926f, 0});
    }
  }
}

auto ASnail::BeginPlay() -> void
{
  Super::BeginPlay();
  lastTimeSay = 0.f;
  auto gameState = GetWorld()->GetGameState<APrjGameStateBase>();
  if (!gameState)
  {
    LOG_ERR("GameSate is not APrjGameStateBase");
    return;
  }
  const auto &maze = gameState->getMaze();
  const auto size = maze.getSize();
  const auto sLoc = GetActorLocation();
  auto newLoc = FVector{(size - 1) * 200.f + 100.f, (size - 1) * 200.f + 100.f, sLoc.Z};
  SetActorLocation(newLoc, false);
  OnActorHit.AddDynamic(this, &ASnail::onHit);
}

auto ASnail::onHit(AActor *me, AActor *other, FVector impact, const FHitResult &hitResult) -> void
{
  const auto player = UGameplayStatics::GetPlayerCharacter(this, 0);
  if (player == other)
  {
    auto gameState = GetWorld()->GetGameState<APrjGameStateBase>();
    if (!gameState)
    {
      LOG_ERR("GameSate is not APrjGameStateBase");
      return;
    }
    gameState->endGame(EndGameState::died);
    return;
  }
  else if (Cast<AImmortalSnailProjectile>(other))
  {
    const auto t = GetWorld()->GetTimeSeconds();
    if (t - lastTimeSay > 5.f)
    {
      lastTimeSay = t;
      UGameplayStatics::PlaySoundAtLocation(this, onHitSounds[rand() % onHitSounds.size()], GetActorLocation());
    }
  }
}
