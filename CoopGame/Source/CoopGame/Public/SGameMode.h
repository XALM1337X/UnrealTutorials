// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_BotSpawner;

	int32 NumBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float TimeBetweenWaves;

protected:


	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave(); 

	void PrepareForNextWave();

public: 

	ASGameMode();
	virtual void StartPlay() override;
};