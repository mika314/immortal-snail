// Copyright 2022 Mika Pi

#include "Snail.h"
#include "log.h"
#include "obj_finder.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

ASnail::ASnail()
{
  PrimaryActorTick.bCanEverTick = true;
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(objFinder<UStaticMesh>(TEXT("/Game/SM_Snail")));
  mesh->SetMobility(EComponentMobility::Movable);
  mesh->SetCollisionProfileName(TEXT("BlockAll"));
}

auto ASnail::Tick(float dt) -> void
{
  Super::Tick(dt);
  SetActorScale3D(FVector{1.f + .1f * sinf(GetWorld()->GetTimeSeconds()), 1.f, 1.f});
}
