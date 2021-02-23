// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionActor.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
// Sets default values
AExplosionActor::AExplosionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	if (GetLocalRole() == ROLE_Authority)
	{
		PrimaryActorTick.bCanEverTick = true;
		this->SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
		RootComponent = SphereComp;
		this->ExplosionAnimationScale = FVector(3.0f,3.0f,3.0f);
		this->Ticks = 0;
		this->ExplosionForce = 100000.0f;
	}
}


void AExplosionActor::Init(AController* controller) {
	instigator = controller;
}

// Called when the game starts or when spawned
void AExplosionActor::BeginPlay()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Super::BeginPlay();
	}	
}

// Called every frame
void AExplosionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(),GetActorRotation(), ExplosionAnimationScale);
	Explode();
	Destroy();

}

void AExplosionActor::Explode_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> ignores;
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped component %s on actor %s"), *OverLap->GetName(),*OverLap->GetOwner()->GetName());
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), SphereComp->GetScaledSphereRadius(), damageType, ignores, this, instigator, false, ECC_WorldDynamic);		
	}
}

bool AExplosionActor::Explode_Validate()
{
	return true;
}

