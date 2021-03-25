// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "GameFramework/Character.h"
#include "AI/STrackerBotHealthComp.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASTrackerBot::ASTrackerBot() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshCompTB"));
	RootComponent = mesh;
	mesh->SetCanEverAffectNavigation(false);  
	bUseVelocityChange = false;
	MovementForce = 1000.0f;
	RequiredDistanceToTarget = 100.0f;
	TrackerBotHealthComp = CreateDefaultSubobject<USTrackerBotHealthComp>(TEXT("TrackerBotHealthComp"));

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay() {
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority) {
		mesh->SetSimulatePhysics(true);
	}	
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority) {
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequiredDistanceToTarget) {
			//UE_LOG(LogTemp, Warning, TEXT("HIT_1"));
			NextPathPoint = GetNextPathPoint();
		} else {
			//UE_LOG(LogTemp, Warning, TEXT("%f : %f"),DistanceToTarget,RequiredDistanceToTarget);
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;
			mesh->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
		}
		DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
	}
}

FVector ASTrackerBot::GetNextPathPoint() {
	ACharacter* MyChar = UGameplayStatics::GetPlayerCharacter(this, 0); 
	if (MyChar) {
		AActor* MyAct = Cast<AActor>(MyChar);
		if (MyAct) {
			UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), MyAct);
			if (NavPath && NavPath->PathPoints.Num() > 1) {
				return NavPath->PathPoints[1];
			}
		}
	}	
	return GetActorLocation();
}

