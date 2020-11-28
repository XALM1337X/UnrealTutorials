// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "kismet/GameplayStatics.h"
#include "FPSGameState.h"
#include "FpsPlayerController.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	//Set Gamestate to use our FPSGameState
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* pawn, bool IsSuccess)
{	
	TArray<AActor*> ret_actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpectatingViewPoints, ret_actors);
	if (ret_actors.Num() > 0)
	{
		AActor* target = ret_actors[0];
		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it) 
		{
			AFPSPlayerController *PC = Cast<AFPSPlayerController>(it->Get());
			if (PC)
			{
				PC->SetViewTargetWithBlend(target, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("SpectatingViewPoint is null"));
	}
	AFPSGameState* GS = GetWorld()->GetGameState<AFPSGameState>();
	//UE_LOG(LogTemp, Warning, TEXT("GameMode_1"));
	if (GS) {
		GS->MulticastMissionCompleted(pawn,IsSuccess);
	}
}
