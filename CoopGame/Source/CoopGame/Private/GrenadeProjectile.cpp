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
		CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeProjectile::OnHit);

		ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollisionSphere"));;
		ExplosionSphere->SetupAttachment(CollisionComp);

		// Use a ProjectileMovementComponent to govern this projectile's movement
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
		ProjectileMovement->UpdatedComponent = CollisionComp;
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
	Super::BeginPlay();
}
void AGrenadeProjectile::ServerExplode_Implementation() { 
	if (GetLocalRole() == ROLE_Authority) {
		UE_LOG(LogTemp, Warning, TEXT("AGrenadeProjectile::Explode-server"));	
		TArray<AActor*> ignores;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), ExplosionSphere->GetScaledSphereRadius(), damageType, ignores, this, GetInstigatorController(), false, ECC_WorldDynamic);
		//GetWorldTimerManager().ClearTimer(this->ExplodeTimer);
	} 
	PlayExplosionEffect();
	Destroy();
}
void AGrenadeProjectile::TriggerTimer_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("AGrenadeProjectile::TriggerTimer"));
	GetWorldTimerManager().SetTimer(this->ExplodeTimer, this, &AGrenadeProjectile::ServerExplode, 3.0f , false);
}

void AGrenadeProjectile::OnRep_FGrenEffectsReplicate() {
	PlayExplosionEffect();
}


void AGrenadeProjectile::PlayExplosionEffect_Implementation() {
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(), GetActorRotation(), this->ExplosionAnimationScale);
}

void AGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	// Only add impulse and destroy projectile if we hit a physics
		UE_LOG(LogTemp,Warning,TEXT("AGrenadeProjectile::OnHit"));
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		if (GetLocalRole() == ROLE_Authority) {
			UWorld* World = GetWorld();
			FVector loc = GetActorLocation();
			FRotator rot = GetActorRotation();
			this->ServerExplode();
		}
	}
}
void AGrenadeProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGrenadeProjectile, grenEffectsRep);

}
