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


ASGameMode::ASGameMode() {
    WaveCount = 0;
    NumBotsToSpawn = 0;
    TimeBetweenWaves = 3.0f;
    GameStateClass = ASGameState::StaticClass();
    PlayerStateClass = ASPlayerState::StaticClass();
    
}


void ASGameMode::StartPlay() {
    Super::StartPlay();
    SetWaveState(EWaveState::WaitingToStart);
    GetWorldTimerManager().SetTimer(TimerHandle_CheckWaveState, this, &ASGameMode::CheckWaveState, 3.0f, true, 5.0f);
}

void ASGameMode::BeginPlay() {
    Super::BeginPlay();
    ASGameState* state = GetGameState<ASGameState>();
    if (ensureAlways(state)) {
        state->SetOwner(this);
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


void ASGameMode::CheckWaveState() {
    if (!CheckLivingPlayersState()) {
        SetWaveState(EWaveState::GameOver);
        GetWorldTimerManager().ClearTimer(TimerHandle_CheckWaveState);
        PostGameAiCleanup();

        //TODO: Put in own function 
        for (FConstPlayerControllerIterator ContItr = GetWorld()->GetPlayerControllerIterator(); ContItr; ContItr++) {
            APlayerController* PlayerCont = Cast<APlayerController>(*ContItr);
            if (PlayerCont) { 
                this->SendRespawnRequest(PlayerCont);
            }
        }

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
         UE_LOG(LogTemp, Warning, TEXT("GAME OVER!"));
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

void ASGameMode::SendRespawnRequest(APlayerController* RespawnController) {
    ASGameState* GS = GetGameState<ASGameState>();
    if (ensureAlways(GS)) {
        GS->respawn_replicator.controller = RespawnController;
    }
}

/*void ASGameMode::RespawnCharacter(APlayerController* Controller) {
    if (Controller) {
        this->RestartPlayer(Controller);
    }
}*/