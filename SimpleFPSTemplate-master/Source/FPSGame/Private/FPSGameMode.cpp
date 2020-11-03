// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* pawn, bool IsSuccess)
{	
	if (pawn)
	{
		pawn->DisableInput(nullptr);

		if (SpectatingViewPoints)
		{
			TArray<AActor*> ret_actors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewPoints, ret_actors);

			if (ret_actors.Num() > 0)
			{
				AActor* target = ret_actors[0];
				APlayerController* controller = Cast<APlayerController>(pawn->GetController());
				if (controller)
				{
					controller->SetViewTargetWithBlend(target, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewPoint is null"));
		}
	}
	OnGameComplete(pawn, IsSuccess);
}
