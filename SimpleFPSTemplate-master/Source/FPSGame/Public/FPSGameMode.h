// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category= "Spectating")
	TSubclassOf<AActor> SpectatingViewPoints;
	AFPSGameMode();

	void CompleteMission(APawn* pawn, bool IsSuccess);
};



