// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

// Sets default values
AGrenadeProjectile::AGrenadeProjectile() {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//if (GetLocalRole() == ROLE_Authority) {
		CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
		RootComponent = CollisionComp;
		CollisionComp->InitSphereRadius(0.5f);
		CollisionComp->SetCollisionProfileName("Projectile");
		CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
		CollisionComp->CanCharacterStepUpOn = ECB_No;
		//CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeProjectile::OnHit);
		
		GrenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenMesh"));
		GrenMesh->SetupAttachment(CollisionComp);
		ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollisionSphere"));
		ExplosionSphere->SetupAttachment(CollisionComp);

		// Use a ProjectileMovementComponent to govern this projectile's movement
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
		ProjectileMovement->UpdatedComponent = CollisionComp;
		ProjectileMovement->InitialSpeed = 1500.f;
		ProjectileMovement->MaxSpeed = 1500.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;
		this->ExplosionForce = 1000.0f;
		this->ExplosionDamage = 50.0f;
		this->ExplosionAnimationScale = FVector(3.0f,3.0f,3.0f);
		//this->SetCollisionResponseToChannel

		SetReplicates(true);
		//SetReplicateMovement(true);	<<-- Notice I did comment this out as suggested, and disabled it in my blueprint.
	//}
}

// Called when the game starts or when spawned
void AGrenadeProjectile::BeginPlay() {
	Super::BeginPlay();
	TriggerTimer_Implementation();
}

void AGrenadeProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());
}

void AGrenadeProjectile::ServerExplode_Implementation() { 
	if (GetLocalRole() == ROLE_Authority) {
		TArray<AActor*> ignores;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionSphere->GetScaledSphereRadius(), damageType, ignores, this, GetInstigatorController(), false, ECC_WorldDynamic);
		GetWorldTimerManager().ClearTimer(this->ExplodeTimer);
	} 
	PlayExplosionEffect();
	Destroy();
}
void AGrenadeProjectile::TriggerTimer_Implementation() {
	//NOTE: FOR THE LOVE OF GOD, MAKE SURE THE ACTORS INITIAL LIFESPAN 
	//LASTS LONGER THAN THE TIMER. IF THEY ARE EQUAL IT WILL FUCK UP EVERYTHING!
	GetWorld()->GetTimerManager().SetTimer(this->ExplodeTimer, this, &AGrenadeProjectile::ServerExplode, 2.0f , false);
}

void AGrenadeProjectile::PlayExplosionEffect_Implementation() {
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(), GetActorRotation(), this->ExplosionAnimationScale);
	if(ExplosionSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
}
