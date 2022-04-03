// Copyright (c) 2022 Mika Pi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>

#include "MazeMesh.generated.h"

UCLASS()
class IMMORTALSNAIL_API AMazeMesh : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AMazeMesh();

protected:
  // Called when the game starts or when spawned
  void BeginPlay() override;

public:
  // Called every frame
  void Tick(float DeltaTime) override;

  std::vector<class UStaticMesh *> walls;
  std::vector<class UStaticMesh *> arrows;
};
