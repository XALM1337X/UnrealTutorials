// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SGameMode.h"
#include "Kismet/GameplayStatics.h"

ASPlayerController::ASPlayerController() {

}


void ASPlayerController::TriggerRespawn_Implementation(APlayerController* PlayerCont) {
    if (GetLocalRole() != ROLE_Authority) {
        return;
    }
    ASGameMode* GM = Cast<ASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (ensureAlways(GM)) {
        GM->RespawnCharacter(PlayerCont);
    }
}