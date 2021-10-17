// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Net/UnrealNetwork.h"
#include "SGameMode.h"
#include "kismet/GameplayStatics.h"
#include "SCharacter.h"
#include "SPlayerController.h"



ASGameState::ASGameState() {

}

void ASGameState::BeginPlay() {
    Super::BeginPlay();
}

void ASGameState::OnRep_WaveState(EWaveState OldState) {
    WaveStateChanged(WaveState, OldState);
}

void ASGameState::OnRep_StateRespawnRequest() {
    StateRespawnRequest(respawn_replicator.controller);
}

void ASGameState::CheckPlayerRespawn() {
    ASGameMode*  GM = Cast<ASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    for (FConstPlayerControllerIterator ContItr = GetWorld()->GetPlayerControllerIterator(); ContItr; ContItr++) {
        ASPlayerController* PlayerCont = Cast<ASPlayerController>(*ContItr);
        if (PlayerCont) {
            APawn* player_pawn = PlayerCont->GetPawn();
            if (player_pawn) {
                ASCharacter* playerChar = Cast<ASCharacter>(player_pawn);
                if (!playerChar) {            
                    GM->SendRespawnRequest(PlayerCont);
                }
            } else {
                GM->SendRespawnRequest(PlayerCont);
            }
        }
    } 
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASGameState, WaveState);
    DOREPLIFETIME(ASGameState, respawn_replicator);

}