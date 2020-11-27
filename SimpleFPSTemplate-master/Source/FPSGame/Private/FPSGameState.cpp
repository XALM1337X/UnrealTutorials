// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "EngineUtils.h"
#include "FPSPlayerController.h"






void AFPSGameState::MulticastMissionCompleted_Implementation(APawn* pawn_param, bool bMissionSuccess)  
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it) 
	{
		AFPSPlayerController *PC = Cast<AFPSPlayerController>(it->Get());
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(pawn_param, bMissionSuccess);
			APawn* MyChar = PC->GetPawn();
			if (MyChar)
			{
				MyChar->DisableInput(nullptr);
			}
		}
	}
}

	//Proper way to iterate over pawns as of > 4.24
	/*for (TActorIterator<APawn> Itr(GetWorld()); Itr; ++Itr)
	{
		if (Itr->IsA(APawn::StaticClass()))
		{
            APawn* Pawn = *Itr;
            if (Pawn && Pawn->IsLocallyControlled() && pawn_param == Pawn)
            {
                Pawn->DisableInput(nullptr);
            }
		}
	}*/