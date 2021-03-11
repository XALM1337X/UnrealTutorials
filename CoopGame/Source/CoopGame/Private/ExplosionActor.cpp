// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionActor.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AExplosionActor::AExplosionActor() {
// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	this->ExplosionAnimationScale = FVector(3.0f,3.0f,3.0f);
	this->Ticks = 0;
	this->ExplosionForce = 100000.0f;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AExplosionActor::BeginPlay()
{
	Super::BeginPlay();		
	UE_LOG(LogTemp,Warning,TEXT("AExplosionActor::BeginPlay"));
	Explode();
	Destroy();
}

// Called every frame
void AExplosionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosionActor::Explode_Implementation()
{
	if (GetLocalRole() == ROLE_Authority) {
		UE_LOG(LogTemp,Warning,TEXT("AExplosionActor::Explode_Implementation"));
		TArray<AActor*> ignores;
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped component %s on actor %s"), *OverLap->GetName(),*OverLap->GetOwner()->GetName());
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), SphereComp->GetScaledSphereRadius(), damageType, ignores, this, GetInstigatorController(), false, ECC_WorldDynamic);		
		this->PlayExplosionEffect();
	}
}

void AExplosionActor::PlayExplosionEffect_Implementation() {
	UE_LOG(LogTemp,Warning,TEXT("AExplosionActor::PlayExplosionEffect_Implementation"));
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(), GetActorRotation(), ExplosionAnimationScale);
}


/*void AExplosionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(ASWeapon, TraceStruct, COND_SkipOwner);	
}*/
