// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MuzzleSocketName = "MuzzleSocket";
}

void ASWeapon::Fire_Implementation()
{

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector EyeLocation; 
		FRotator EyeRotation;
		
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEndPos = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(MyOwner);
		queryParams.AddIgnoredActor(this);
		queryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEndPos;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEndPos, ECC_Visibility))
		{
			AActor* HitActor = hit.GetActor();
			// Process damage and such

			UGameplayStatics::ApplyPointDamage(HitActor,20.0f, ShotDirection, hit, MyOwner->GetInstigatorController(), this, DamageType);
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
			}
			TraceEndPoint = hit.ImpactPoint;
		}

		if (MuzzleEffect) 
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}		


		if (TracerEffect) 
		{
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if (TracerComp)
			{
				TracerComp->SetVectorParameter("BeamEnd",TraceEndPoint);
			}
			//DrawDebugLine(GetWorld(), EyeLocation, TraceEndPos, FColor::White, false, 1.0f, 0, 1.0f);
		}

	}
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

