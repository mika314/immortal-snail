// Mika Pi (c) 2022

#include "MazeMesh.h"
#include "Engine/StaticMesh.h"
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

  // for (auto i = 0; i < 8; ++i)
  //   walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/Geometry/Meshes/1M_Cube")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall1")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall2")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall3")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall4")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall5")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall6")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall7")));
  walls.push_back(objFinder<UStaticMesh>(TEXT("/Game/SM_Wall8")));

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

  auto maze = gameState->getMaze();
  const auto size = maze.getSize();

  for (auto y = 0; y < size; ++y)
    for (auto x = 0; x < size - 1; ++x)
    {
      auto wall = maze.getWall(x, y * 2);
      if (wall == 0)
        continue;
      auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
      mesh->RegisterComponent();
      mesh->SetWorldLocation(FVector{(x + 1) * 200.f, y * 200.f, 0.f});
      mesh->SetWorldRotation(FRotator{0.f, 180.f, 0.f});
      mesh->SetStaticMesh(walls[wall - 1]);
      mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
      mesh->SetMobility(EComponentMobility::Movable);
      mesh->SetCollisionProfileName(TEXT("BlockAll"));
    }

  for (auto y = 0; y < size - 1; ++y)
    for (auto x = 0; x < size; ++x)
    {
      auto wall = maze.getWall(x, y * 2 + 1);
      if (wall == 0)
        continue;
      auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
      mesh->RegisterComponent();
      mesh->SetWorldLocation(FVector{x * 200.f, (y + 1) * 200.f, 0.f});
      mesh->SetWorldRotation(FRotator{0.f, 90.f, 0.f});
      mesh->SetStaticMesh(walls[wall - 1]);
      mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules{EAttachmentRule::KeepRelative, false});
      mesh->SetMobility(EComponentMobility::Movable);
      mesh->SetCollisionProfileName(TEXT("BlockAll"));
    }

  {
    auto mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
    mesh->RegisterComponent();
    mesh->SetWorldLocation(FVector{0, 0.f, 0.f});
    mesh->SetWorldRotation(FRotator{0.f, 90.f, 0.f});
    mesh->SetWorldScale3D(FVector{1.f, 1.f * size, 1.f});
    mesh->SetStaticMesh(walls[rand() % walls.size()]);
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
    mesh->SetStaticMesh(walls[rand() % walls.size()]);
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
    mesh->SetStaticMesh(walls[rand() % walls.size()]);
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
    mesh->SetStaticMesh(walls[rand() % walls.size()]);
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
