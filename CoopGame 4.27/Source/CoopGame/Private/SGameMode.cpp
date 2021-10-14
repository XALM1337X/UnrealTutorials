// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "AI/STrackerBotHealthComp.h"
#include "SCharacter.h"
#include "SHealthComp.h"
#include "SGameState.h"
#include "AI/STrackerBot.h"
#include "SPlayerState.h"
#include "SPlayerController.h"



ASGameMode::ASGameMode() {
    WaveCount = 0;
    NumBotsToSpawn = 0;
    TimeBetweenWaves = 3.0f;
    GameStateClass = ASGameState::StaticClass();
    PlayerStateClass = ASPlayerState::StaticClass();
    PlayerControllerClass = ASPlayerController::StaticClass();
    IsGameActive = false;
    
}

//Standard actor events
void ASGameMode::BeginPlay() {
    Super::BeginPlay();
    IsGameActive=true;
}
//////////////////////////////////////////////////////////////


void ASGameMode::StartPlay() {
    Super::StartPlay();
    SetWaveState(EWaveState::WaitingToStart);
    //GetWorldTimerManager().SetTimer(TimerHandle_CheckWaveState, this, &ASGameMode::CheckWaveState, 3.0f, true, 5.0f);
    GetWorldTimerManager().SetTimer(TimerHandle_PlayerRespawnState, this, &ASGameMode::PlayerStateSpawnHandler, 3.0f, true, 0.0f);
}

void ASGameMode::PlayerStateSpawnHandler() {
    ASGameState* GS = GetGameState<ASGameState>();
    if (ensureAlways(GS)) {
        GS->CheckPlayerRespawn();
    }   
}

void ASGameMode::GameRestartHandler() {
    WaveCount = 0;
    for (TActorIterator<ASCharacter> Itr(GetWorld()); Itr; ++Itr) {
         ASCharacter* Player = *Itr; 
         if (Player) {
             APawn* Pawn = Cast<APawn>(Player);
             if (Pawn && Pawn->IsPlayerControlled()) {
                GetWorldTimerManager().SetTimer(TimerHandle_CheckWaveState, this, &ASGameMode::CheckWaveState, 3.0f, true, 5.0f);
                GetWorldTimerManager().ClearTimer(TimerHandle_GameRestartState);
             }
         }
    }
}

void ASGameMode::CheckWaveState() {
    if (!CheckLivingPlayersState()) {
        PostGameAiCleanup();
        IsGameActive = false;
        GetWorldTimerManager().ClearTimer(TimerHandle_CheckWaveState);
        GetWorldTimerManager().SetTimer(TimerHandle_GameRestartState, this, &ASGameMode::GameRestartHandler, 3.0f, true, 5.0f);
        return;
    }

    bool isPreparingWave = GetWorldTimerManager().IsTimerActive(TimerHandle_TimeNextWaveStart);
    if (NumBotsToSpawn > 0 || isPreparingWave) {
        SetWaveState(EWaveState::WaitingToStart);
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
            SetWaveState(EWaveState::WaveActive);
            bIsAnyBotAlive = true;
            break;
        }
    }

    if (!bIsAnyBotAlive) {
        SetWaveState(EWaveState::WaveComplete);
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
         return false;
     }
     return true;
}

void ASGameMode::SetWaveState(EWaveState NewState) {
    ASGameState* GS = GetGameState<ASGameState>();
    if (ensureAlways(GS)) {
        GS->WaveState = NewState;
    }
}


void ASGameMode::PrepareForNextWave() {
    SetWaveState(EWaveState::PreparingWave);
    GetWorldTimerManager().SetTimer(TimerHandle_TimeNextWaveStart, this, &ASGameMode::StartWave,TimeBetweenWaves, false);
}

void ASGameMode::StartWave() {
    SetWaveState(EWaveState::WaitingToStart);
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
    SetWaveState(EWaveState::WaveActive);
    //PrepareForNextWave();
}


void ASGameMode::PostGameAiCleanup() {
    for (TActorIterator<ASTrackerBot> Itr(GetWorld()); Itr; ++Itr) {
        ASTrackerBot* TB = *Itr;
        if (TB) {
            TB->Destroy();
        }
    }
}

void ASGameMode::ScoreBroadcast(AActor* VictimActor, AActor* KillerActor, AController* KillerController) {
    this->OnActorKilled.Broadcast(VictimActor, KillerActor, KillerController);
}

void ASGameMode::SendRespawnRequest(ASPlayerController* RespawnController) {
    ASGameState* GS = GetGameState<ASGameState>();
    if (ensureAlways(GS)) {
        //UE_LOG(LogTemp, Warning, TEXT("HIT-1"));
        GS->respawn_replicator.controller = RespawnController;
        GS->respawn_replicator.repTick += 1;
        if (GS->respawn_replicator.repTick > 1000) {
            GS->respawn_replicator.repTick = 0;
        }
    } 
}

void ASGameMode::RespawnCharacter(ASPlayerController* Controller) {
    if (Controller) {
        this->RestartPlayer(Controller);
    }
}