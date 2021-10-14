// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASPlayerController::ASPlayerController() {
    //IsDead = false;
    //SetReplicates(true);
    this->bReplicates = true;
}


void ASPlayerController::TriggerRespawn_Implementation(ASPlayerController* PlayerCont) {
    if (GetLocalRole() != ROLE_Authority) {
        return;
    }
    ASGameMode* GM = Cast<ASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (ensureAlways(GM)) {
        GM->RespawnCharacter(PlayerCont);
    }
}

//void ASPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
    //Super::GetLifetimeReplicatedProps(OutLifetimeProps);
   // DOREPLIFETIME(ASPlayerController, IsDead);
//}