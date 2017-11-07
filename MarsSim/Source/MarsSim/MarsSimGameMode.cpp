// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MarsSimGameMode.h"
#include "MarsSimPawn.h"
#include "MarsSimHud.h"

AMarsSimGameMode::AMarsSimGameMode()
{
	DefaultPawnClass = AMarsSimPawn::StaticClass();
	HUDClass = AMarsSimHud::StaticClass();
}
