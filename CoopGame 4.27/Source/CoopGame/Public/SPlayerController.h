// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"


class ASPlayerController;
/**
 * 
 */
UCLASS()
class COOPGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 
	ASPlayerController();

public:
	//UPROPERTY(Replicated)
	//bool IsDead;

protected: //
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void TriggerRespawn(ASPlayerController* PlayerCont);
	
};
