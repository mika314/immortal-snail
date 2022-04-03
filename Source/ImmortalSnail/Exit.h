// Copyright (c) 2022 Mika Pi

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Exit.generated.h"

/**
 *
 */
UCLASS()
class IMMORTALSNAIL_API AExit : public ATriggerBox
{
  GENERATED_BODY()
private:
  auto BeginPlay() -> void override;

  UFUNCTION()
  void onBeginOverlap(AActor *me, AActor *other);
};
