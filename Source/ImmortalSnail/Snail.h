// Copyright 2022 Mika Pi

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
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
  auto Tick(float) -> void override;
};
