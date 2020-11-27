// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Public/TimerManager.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnsensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnsensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnsensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);
	GuardState = EAIGuardState::Idle;
	bPatrol = true;
	TickCount = 0;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalLookDirection = GetActorRotation();
	if (bPatrol)
	{
		MoveToNextPoint();
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* pawn)
{
	if (pawn == nullptr)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), pawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 5.0f);
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(pawn, false);
	}
	if (GuardState == EAIGuardState::Alerted)
	{
		return;
	}
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
	SetGuardState(EAIGuardState::Alerted);	
}

void AFPSAIGuard::OnNoiseHear(APawn* pawn, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 5.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator look = FRotationMatrix::MakeFromX(Direction).Rotator();
	look.Pitch = 0.0f;
	look.Roll = 0.0f;
	SetActorRotation(look);

	GetWorldTimerManager().ClearTimer(th_rot_reset);
	GetWorldTimerManager().SetTimer(th_rot_reset, this, &AFPSAIGuard::ResetRotation, 3.0f);
	if (GuardState == EAIGuardState::Alerted)
	{
		return;
	}
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
	SetGuardState(EAIGuardState::Suspicious);
}

void AFPSAIGuard::ResetRotation()
{
	SetActorRotation(OriginalLookDirection);
	if (GuardState == EAIGuardState::Alerted)
	{
		return;
	}
	if (bPatrol)
	{
		MoveToNextPoint();
	}
	SetGuardState(EAIGuardState::Idle);
}

void AFPSAIGuard::OnRep_GuardState() {
	OnStateChange(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIGuardState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;
	OnRep_GuardState();
}

void AFPSAIGuard::MoveToNextPoint()
{
	if (CurrentPoint == nullptr || CurrentPoint == SecondPoint)
	{
		CurrentPoint = FirstPoint;
	}
	else
	{
		CurrentPoint = SecondPoint;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TickCount % 500== 0)
	{
		if (CurrentPoint)
		{
			FVector Delta = GetActorLocation() - CurrentPoint->GetActorLocation();
			float DistanceToPoint = Delta.Size();
			if (DistanceToPoint < 70)
			{
				MoveToNextPoint();
			}
		}
	}
	if (TickCount == 50000)
	{
		TickCount = 0;
	}
	TickCount++;
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
//This is a function for replication on clients for the UPROPERTY GuardState for AFPSAIGuard. 
//This does not have a definition in FPSAIGuard.h.
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSAIGuard, GuardState);
}