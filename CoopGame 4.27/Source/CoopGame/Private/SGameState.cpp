// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Net/UnrealNetwork.h"



ASGameState::ASGameState() {

}

void ASGameState::OnRep_WaveState(EWaveState OldState) {
    UE_LOG(LogTemp, Warning, TEXT("ONREP_WAVESTATE"));
    WaveStateChanged(WaveState, OldState);
}


void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASGameState, WaveState);

}