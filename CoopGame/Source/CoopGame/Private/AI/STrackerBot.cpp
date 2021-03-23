// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "GameFramework/Character.h"

// Sets default values
ASTrackerBot::ASTrackerBot() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshCompTB"));
	RootComponent = mesh;
	mesh->SetCanEverAffectNavigation(false);  

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!GetActorLocation().Equals(NextPathPoint)) {
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= 1000;
		mesh->AddForce(ForceDirection);
	} else {
		NextPathPoint = GetNextPathPoint();
	}

}

FVector ASTrackerBot::GetNextPathPoint() {
	ACharacter* MyChar = UGameplayStatics::GetPlayerCharacter(this, 0); 
	AActor* MyAct = Cast<AActor>(MyChar);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), MyAct);
	if (NavPath->PathPoints.Num() > 1) {
		return NavPath->PathPoints[1];
	}
	
	return GetActorLocation();
}

