// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGrenadeProjectile::AGrenadeProjectile() {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	if (GetLocalRole() == ROLE_Authority) {
		GrenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenMesh"));
		RootComponent = GrenMesh;


		CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
		CollisionComp->InitSphereRadius(0.5f);
		CollisionComp->SetCollisionProfileName("Projectile");
		CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
		CollisionComp->CanCharacterStepUpOn = ECB_No;
		CollisionComp->SetupAttachment(GrenMesh);

		ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollisionSphere"));;
		ExplosionSphere->SetupAttachment(GrenMesh);

		// Use a ProjectileMovementComponent to govern this projectile's movement
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
		ProjectileMovement->UpdatedComponent = GrenMesh;
		ProjectileMovement->InitialSpeed = 1500.f;
		ProjectileMovement->MaxSpeed = 1500.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;
		this->ExplosionForce = 100000.0f;
		this->ExplosionAnimationScale = FVector(3.0f,3.0f,3.0f);

		SetReplicates(true);
		SetReplicateMovement(true);	
	}
}

// Called when the game starts or when spawned
void AGrenadeProjectile::BeginPlay() {
	if (GetLocalRole() == ROLE_Authority) {
		Super::BeginPlay();
	}
}


void AGrenadeProjectile::ServerExplode_Implementation() {
	this->Explode();
}

void AGrenadeProjectile::Explode() {
	if(GetLocalRole() != ROLE_Authority) {
		this->ServerExplode();
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GrenadeProjectile::Explode"));
	TArray<AActor*> ignores;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), ExplosionSphere->GetScaledSphereRadius(), damageType, ignores, this, GetInstigatorController(), false, ECC_WorldDynamic);
	//THIS IS NETMULTICASTED
	PlayExplosionEffect();///
	/////////////////////////
	Destroy();
}


void AGrenadeProjectile::PlayExplosionEffect_Implementation() {
	//UE_LOG(LogTemp,Warning,TEXT("AGrenadeProjectile::PlayExplosionEffect_Implementation"));
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(), GetActorRotation(), ExplosionAnimationScale);
}


void AGrenadeProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AGrenadeProjectile, ActorToSpawn);
	DOREPLIFETIME(AGrenadeProjectile, ExplodeTimer);
	DOREPLIFETIME(AGrenadeProjectile, ExplosionForce);
	DOREPLIFETIME(AGrenadeProjectile, ExplosionSphere);
	DOREPLIFETIME(AGrenadeProjectile, Explosion);

}
