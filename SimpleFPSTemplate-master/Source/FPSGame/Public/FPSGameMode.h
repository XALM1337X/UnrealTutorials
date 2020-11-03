// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category= "Spectating")
	TSubclassOf<AActor> SpectatingViewPoints;

public:

	AFPSGameMode();

	void CompleteMission(APawn* pawn, bool IsSuccess);

	UFUNCTION(BlueprintImplementableEvent , Category = "GameMode")
	void OnGameComplete(APawn* pawn, bool IsSuccess);
};



