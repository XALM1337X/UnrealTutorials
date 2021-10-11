// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
enum class EWaveState :  uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_TimeNextWaveStart; 

	FTimerHandle TimerHandle_CheckWaveState; 

	int32 NumBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float TimeBetweenWaves;

protected:

	void PrepareForNextWave();

	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave(); 

	void CheckWaveState();

	bool CheckLivingPlayersState();

	void SetWaveState(EWaveState NewState);

	void PostGameAiCleanup();



public: 

	ASGameMode();
	virtual void StartPlay() override;

	UFUNCTION(Client, reliable)
	void ClientScoreBroadcast(AActor* VictimActor, AActor* KillerActor, AController* KillerController);

	virtual void BeginPlay() override;


public: 
	
	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnActorKilled OnActorKilled;

};
