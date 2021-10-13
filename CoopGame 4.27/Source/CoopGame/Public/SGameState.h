// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState : uint8 {
	WaitingToStart, 
	PreparingWave, 
	WaveActive, 
	WaveComplete, 
	GameOver,

};

/**
 * 
 */

USTRUCT() 
struct FRespawnCheck {
	GENERATED_BODY()
	public:
		UPROPERTY()
		APlayerController* controller;
};


UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase {
	GENERATED_BODY()

public: 
	ASGameState();
	virtual void BeginPlay() override;
	

protected: 
	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION()
	void OnRep_StateRespawnRequest();

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void StateRespawnRequest(APlayerController* PC);


public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveState, Category="GameState")
	EWaveState WaveState;
	
	UPROPERTY(ReplicatedUsing=OnRep_StateRespawnRequest)
	FRespawnCheck respawn_replicator;

protected:

	
};
