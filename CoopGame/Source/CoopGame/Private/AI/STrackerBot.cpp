// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AI/STrackerBotHealthComp.h"
#include "Components/SphereComponent.h"
#include "SCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASTrackerBot::ASTrackerBot() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshCompTB"));
	RootComponent = mesh;
	mesh->SetCanEverAffectNavigation(false);
	ExplosionRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TrackerBotExplosionRadiusSphere"));
	ExplosionRadiusSphere->SetupAttachment(mesh);
	ExplosionDetinationRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TrackerBotExplosionDetinationRangeSphere"));
	ExplosionDetinationRangeSphere->SetupAttachment(mesh);
	bUseVelocityChange = false;
	MovementForce = 1000.0f;
	RequiredDistanceToTarget = 100.0f;
	TrackerBotHealthComp = CreateDefaultSubobject<USTrackerBotHealthComp>(TEXT("TrackerBotHealthComp"));
	ExplosionForce = 500.0f;
	ExplosionDamage = 25;
	ExplosionAnimationScale = FVector(1.0f,1.0f,1.0f);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay() {
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority) {
		mesh->SetSimulatePhysics(true);
		ExplosionDetinationRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASTrackerBot::HandleOverlap);
	}	
}
void ASTrackerBot::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ASCharacter::StaticClass())) {
		this->Explode();
		Destroy();
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

void ASTrackerBot::ServerExplode_Implementation() {
	this->Explode();
}

void ASTrackerBot::Explode() {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerExplode();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("TB_EXPLODE"));
	//Apply Radial Damage. 
	TArray<AActor*> ignores;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadiusSphere->GetScaledSphereRadius(), damageType, ignores, this, GetInstigatorController(), false, ECC_WorldDynamic);
	PlayExplosionEffect();
}


void ASTrackerBot::PlayExplosionEffect_Implementation() {
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation(), ExplosionAnimationScale);
}

void ASTrackerBot::PlayImpulseEffect_Implementation()  {
	if (this->MatInst == nullptr) {
		this->MatInst = mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, mesh->GetMaterial(0));
	} else {
		this->MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
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
