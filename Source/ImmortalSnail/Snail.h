// Copyright 2022 Mika Pi

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include <vector>

#include "Snail.generated.h"

/**
 *
 */
UCLASS()
class IMMORTALSNAIL_API ASnail : public AStaticMeshActor
{
  GENERATED_BODY()
public:
  ASnail();

private:
  auto BeginPlay() -> void override;
  auto Tick(float) -> void override;

  UFUNCTION()
  void onHit(AActor *me, AActor *other, FVector impact, const FHitResult &hitResult);

  std::vector<class USoundBase *> onHitSounds;
  class USoundBase *teleportCue;
  class UAudioComponent *slimeSound;

  std::vector<std::pair<int, int>> path;

  float lastTimeSay;
};
