// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGrenadeProjectile::AGrenadeProjectile() {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ExplosionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosionMesh"));
	RootComponent = ExplosionMesh;


	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(0.5f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetupAttachment(ExplosionMesh);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = ExplosionMesh;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	
	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
	TickCount = 1;
	SetReplicates(true);
	SetReplicateMovement(true);	
}

// Called when the game starts or when spawned
void AGrenadeProjectile::BeginPlay() {
	Super::BeginPlay();	
	UE_LOG(LogTemp, Warning, TEXT("AGrenadeProjectile::BeginPlay"));
	GetWorldTimerManager().SetTimer(th_time_between_shots, this, &AGrenadeProjectile::SpawnExplosion, 3 , true, -1);
	
}

// Called every frame
void AGrenadeProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
}

void AGrenadeProjectile::ServerSpawnExplosion_Implementation() {
	this->SpawnExplosion();
}

void AGrenadeProjectile::SpawnExplosion() {

	if(GetLocalRole() != ROLE_Authority) {
		this->ServerSpawnExplosion();
		return;
	}
	FActorSpawnParameters actorSpawnParams;
	actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	//TODO: Figure out how to get 
	actorSpawnParams.Instigator = GetInstigator();
	//Call Explosion
	FVector loc = GetActorLocation();
	FRotator rot = GetActorRotation();
	AActor* explosion = GetWorld()->SpawnActor(ActorToSpawn, &loc, &rot, actorSpawnParams);
	if (explosion){
		UE_LOG(LogTemp, Warning, TEXT("AGrenadeProjectile::explosion_success"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AGrenadeProjectile::explosion_fail"));
	}
	Destroy();
	
}


/*
void AGrenadeProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(ASWeapon, TraceStruct, COND_SkipOwner);

}
*/