// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "SGameMode.h"



ASGameState::ASGameState() {

}

void ASGameState::BeginPlay() {
    Super::BeginPlay();
}

void ASGameState::OnRep_WaveState(EWaveState OldState) {
    UE_LOG(LogTemp, Warning, TEXT("ONREP_WAVESTATE"));
    WaveStateChanged(WaveState, OldState);
}

void ASGameState::OnRep_StateRespawnRequest() {
    StateRespawnRequest(respawn_replicator.controller);
}

void ASGameState::TriggerRespawn_Implementation(APlayerController* PlayerCont) {
    if (GetLocalRole() != ROLE_Authority) {
        return;
    }
    ASGameMode* GM = Cast<ASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (ensureAlways(GM)) {
        GM->RestartPlayer(PlayerCont);
    }
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASGameState, WaveState);
    DOREPLIFETIME(ASGameState, respawn_replicator);

}