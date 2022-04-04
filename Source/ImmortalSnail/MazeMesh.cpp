// Copyright (c) 2022 Mika Pi

#include "MazeMesh.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PrjGameStateBase.h"
#include "log.h"
#include "maze.h"
#include "obj_finder.h"
#include <Engine/World.h>

// Sets default values
AMazeMesh::AMazeMesh()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  {
    walls.emplace_back();
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall101")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall102")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall103")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall104")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall105")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall106")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall107")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall108")));
  }
  {
    walls.emplace_back();
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall201")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall202")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall203")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall204")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall205")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall206")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall207")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall208")));
  }
  {
    walls.emplace_back();
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall301")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall302")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall303")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall304")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall305")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall306")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall307")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall308")));
  }
  {
    walls.emplace_back();
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall401")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall402")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall403")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall404")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall405")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall406")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall407")));
    walls.back().push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall408")));
  }

  arrows.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Arrow1")));
  arrows.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Arrow2")));
  arrows.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Arrow3")));
  arrows.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Arrow4")));
  arrows.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Arrow5")));
  arrows.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Arrow6")));

  auto root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
  SetRootComponent(root);
}

// Called when the game starts or when spawned
void AMazeMesh::BeginPlay()
{
  Super::BeginPlay();

  auto gameState = GetWorld()->GetGameState<APrjGameStateBase>();

  if (!gameState)
  {
    LOG_ERR("GameSate is not APrjGameStateBase");
    return;
  }

  const auto level = [&]() {
    const std::string level = TCHAR_TO_UTF8(*UGameplayStatics::ParseOption(UGameplayStatics::GetGameMode(this)->OptionsString, "level"));
    std::istringstream ss(level);
    auto ret = 0;
    ss >> ret;
    return ret;
  }();

  auto maze = gameState->getMaze();
  const auto size = maze.getSize();

  for (auto y = 0; y < size; ++y)
    for (auto x = 0; x < size - 1; ++x)
    {
      auto wall = maze.getWall(x, y * 2);
      if (wall == 0)
        continue;
      {
        auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
        mesh->RegisterComponent();
        mesh->SetWorldLocation(FVector{(x + 1) * 200.f, y * 200.f, 0.f});
        mesh->SetWorldRotation(FRotator{0.f, 180.f, 0.f});
        mesh->SetStaticMesh(walls[level % walls.size()][wall - 1]);
        mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetCollisionProfileName(TEXT("BlockAll"));
      }

      if (rand() % 4 == 0)
      {
        auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
        mesh->RegisterComponent();
        mesh->SetWorldLocation(FVector{(x + 1) * 200.f, y * 200.f, 0.f});
        mesh->SetWorldRotation(FRotator{0.f, 180.f, 0.f});
        mesh->SetStaticMesh(arrows[rand() % arrows.size()]);
        mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetCollisionProfileName(TEXT("NoCollision"));
      }
    }

  for (auto y = 0; y < size - 1; ++y)
    for (auto x = 0; x < size; ++x)
    {
      auto wall = maze.getWall(x, y * 2 + 1);
      if (wall == 0)
        continue;
      {
        auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
        mesh->RegisterComponent();
        mesh->SetWorldLocation(FVector{x * 200.f, (y + 1) * 200.f, 0.f});
        mesh->SetWorldRotation(FRotator{0.f, 90.f, 0.f});
        mesh->SetStaticMesh(walls[level % walls.size()][wall - 1]);
        mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetCollisionProfileName(TEXT("BlockAll"));
      }
      if (rand() % 4 == 0)
      {
        auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
        mesh->RegisterComponent();
        mesh->SetWorldLocation(FVector{x * 200.f, (y + 1) * 200.f, 0.f});
        mesh->SetWorldRotation(FRotator{0.f, 90.f, 0.f});
        mesh->SetStaticMesh(arrows[rand() % arrows.size()]);
        mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetCollisionProfileName(TEXT("NoCollision"));
      }
    }

  {
    auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
    mesh->RegisterComponent();
    mesh->SetWorldLocation(FVector{0, 0.f, 0.f});
    mesh->SetWorldRotation(FRotator{0.f, 90.f, 0.f});
    mesh->SetWorldScale3D(FVector{1.f, 1.f * size, 1.f});
    const auto wall = rand() % walls[level % walls.size()].size();
    mesh->SetStaticMesh(walls[level % walls.size()][wall]);
    mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
    mesh->SetMobility(EComponentMobility::Movable);
    mesh->SetCollisionProfileName(TEXT("BlockAll"));
  }
  {
    auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
    mesh->RegisterComponent();
    mesh->SetWorldLocation(FVector{0, size * 200.f, 0.f});
    mesh->SetWorldRotation(FRotator{0.f, 90.f, 0.f});
    mesh->SetWorldScale3D(FVector{1.f, 1.f * size, 1.f});
    const auto wall = rand() % walls[level % walls.size()].size();
    mesh->SetStaticMesh(walls[level % walls.size()][wall]);
    mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
    mesh->SetMobility(EComponentMobility::Movable);
    mesh->SetCollisionProfileName(TEXT("BlockAll"));
  }
  {
    auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
    mesh->RegisterComponent();
    mesh->SetWorldLocation(FVector{0, 200.f, 0.f});
    mesh->SetWorldRotation(FRotator{0.f, 180.f, 0.f});
    mesh->SetWorldScale3D(FVector{1.f, 1.f * size - 1.f, 1.f});
    const auto wall = rand() % walls[level % walls.size()].size();
    mesh->SetStaticMesh(walls[level % walls.size()][wall]);
    mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
    mesh->SetMobility(EComponentMobility::Movable);
    mesh->SetCollisionProfileName(TEXT("BlockAll"));
  }
  {
    auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
    mesh->RegisterComponent();
    mesh->SetWorldLocation(FVector{size * 200.f, 0.f, 0.f});
    mesh->SetWorldRotation(FRotator{0.f, 180.f, 0.f});
    mesh->SetWorldScale3D(FVector{1.f, 1.f * size - 1.f, 1.f});
    const auto wall = rand() % walls[level % walls.size()].size();
    mesh->SetStaticMesh(walls[level % walls.size()][wall]);
    mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
    mesh->SetMobility(EComponentMobility::Movable);
    mesh->SetCollisionProfileName(TEXT("BlockAll"));
  }
}

// Called every frame
void AMazeMesh::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}
