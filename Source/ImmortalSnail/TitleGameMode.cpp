// Copyright (c) 2022 Mika Pi

#include "TitleGameMode.h"
#include "TitlePawn.h"

ATitleGameMode::ATitleGameMode()
{
  DefaultPawnClass = ATitlePawn::StaticClass();
}
