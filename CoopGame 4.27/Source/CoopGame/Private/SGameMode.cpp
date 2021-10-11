// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "AI/STrackerBotHealthComp.h"
#include "SCharacter.h"
#include "SHealthComp.h"

ASGameMode::ASGameMode() {
    WaveCount = 0;
    NumBotsToSpawn = 0;
    TimeBetweenWaves = 3.0f;
    
}


void ASGameMode::StartPlay() {
    Super::StartPlay();
    PrepareForNextWave();
    GetWorldTimerManager().SetTimer(TimerHandle_CheckWaveState, this, &ASGameMode::CheckWaveState, 3.0f, true, 0.0f);
}

void ASGameMode::PrepareForNextWave() {
    GetWorldTimerManager().SetTimer(TimerHandle_TimeNextWaveStart, this, &ASGameMode::StartWave,TimeBetweenWaves, false);
}

void ASGameMode::StartWave() {
    WaveCount++;
    NumBotsToSpawn =  2 * WaveCount;
    GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
}


void ASGameMode::SpawnBotTimerElapsed() {
    SpawnNewBot();
    NumBotsToSpawn--; 
    if (NumBotsToSpawn <= 0) {
        EndWave();
    }
}

void ASGameMode::EndWave() {
    GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
    //PrepareForNextWave();
}


void ASGameMode::CheckWaveState() {
    if (!CheckLivingPlayersState()) {
        GetWorldTimerManager().ClearTimer(TimerHandle_CheckWaveState);
        return;
    }
    bool isPreparingWave = GetWorldTimerManager().IsTimerActive(TimerHandle_TimeNextWaveStart);
    if (NumBotsToSpawn > 0 || isPreparingWave) {
        return;
    }
    bool bIsAnyBotAlive = false;
     for (TActorIterator<APawn> Itr(GetWorld()); Itr; ++Itr) {
        APawn *TestPawn = *Itr;
        if (TestPawn == nullptr || TestPawn->IsPlayerControlled()) {
            continue;
        }
 
        USTrackerBotHealthComp* HealthComp = Cast<USTrackerBotHealthComp>(TestPawn->GetComponentByClass(USTrackerBotHealthComp::StaticClass()));
        if (HealthComp && HealthComp->GetHealth() > 0.0f) {
            bIsAnyBotAlive = true;
            break;
        }
    }

    if (!bIsAnyBotAlive) {
        UE_LOG(LogTemp, Warning, TEXT("Starting Next Wave."));
        PrepareForNextWave();
    }
}

bool ASGameMode::CheckLivingPlayersState() {
     bool isPlayerStillAlive = false;
     for (TActorIterator<ASCharacter> Itr(GetWorld()); Itr; ++Itr) {
         ASCharacter* Player = *Itr; 
         if (Player) {
             USHealthComp* playerHC = Player->GetHealthComponent();
             if (playerHC->GetHealth() > 0) {
                isPlayerStillAlive = true;
                break;
             }
         } 
     }
     if (!isPlayerStillAlive) {
         UE_LOG(LogTemp, Warning, TEXT("GAME OVER!"));
         return false;
     }
     return true;
}