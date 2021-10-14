// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
enum class EWaveState :  uint8;
class ASPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_TimeNextWaveStart; 

	FTimerHandle TimerHandle_CheckWaveState; 

	FTimerHandle TimerHandle_PlayerRespawnState;

	FTimerHandle TimerHandle_GameRestartState;

	int32 NumBotsToSpawn;

	int32 WaveCount;

	bool IsGameActive;

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

	void PlayerStateSpawnHandler();

	void GameRestartHandler();



public: 

	ASGameMode();
	virtual void StartPlay() override;


	void ScoreBroadcast(AActor* VictimActor, AActor* KillerActor, AController* KillerController);


	void SendRespawnRequest(ASPlayerController* RespawnController);

	virtual void BeginPlay() override;

	
	UFUNCTION(BlueprintCallable, Category="GameMode")
	void RespawnCharacter(ASPlayerController* Controller);
	

public: 
	
	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnActorKilled OnActorKilled;

};
